#include "Hero.h"
#include "Dlg/Fight/AIMgr.h"
#include "Utils/ConfigMgr.h"
#include "Utils/UIUtils.h"
#include "Skill/SkillMgr.h"

Hero* Hero::create(int ID, AIMgr* ai, int camp)
{
	Hero *pRet = new(std::nothrow) Hero();
	if (pRet && pRet->init(ID, ai, camp)) {
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

Hero::Hero()
	:BaseSprite()
{

}

Hero::~Hero()
{
}

bool Hero::init(int ID, AIMgr* ai, int camp)
{
	if (!BaseSprite::init()) {
		return false;
	}

	this->_txtName = nullptr;
	_objType = 2;
	_prePosList = list<Vec2>();
	_isbroken = false;
	_healthPoint = 0;
	_dotX = -1;
	_dotY = -1;

	
	_radius = 40;
	_camp = camp;
	_ai = ai;
	_ai->addObj(this, _camp);
	_id = ID;
	_target = nullptr;
	_isSelect = true;
	_state = STATE_IDLE;
	this->_mgr_skill = new SkillMgr(this, _ai);


	loadData();
	showUI();

	addTouch();

	//schedule(schedule_selector(Hero::update), 0.81f);
	schedule(schedule_selector(Hero::update));

	return true;
}


void Hero::loadData()
{
	ValueMap& data = *(CFG()->getHeroInfoById(_id));

	_heroID = data["HeroID"].asInt();
	_type = data["Type"].asInt();
	_level = data["Level"].asInt();
	_speed = data["Move"].asInt();
	_shootType = data["Shoot"].asInt();
	_healthPoint = data["HealthPoint"].asInt();
	_totalHP = _healthPoint;
	_damage = data["Damage"].asInt();
	_def = data["Def"].asInt();
	_attackSpeed = data["AttackSpeed"].asInt();
	_shoot_range = data["ShootRange"].asInt();

	
	//this->_mgr_skill->addSkill(6);
	this->_mgr_skill->addSkill(data["Skill1"].asInt());
	this->_mgr_skill->addSkill(data["Skill2"].asInt());
	this->_mgr_skill->addSkill(data["Skill3"].asInt());
	
	_isbroken = false;
	_expReward = 0;
}


void Hero::showUI()
{
	ValueMap& objInfo = *(CFG()->getObjInfoByType(2, _heroID));
	_is_boos = objInfo["IsBoss"].asBool();
	//����
	string animaName = objInfo["Anima"].asString();
	int y = _camp == 1 ? -1 : 1;
	_dir = GM()->getDir(Vec2(0, y));
	_arm = Armature::create(animaName);
	_arm->getAnimation()->play("idle" + GM()->getIntToStr(_dir));
	_arm->setPositionY(20);
	_arm->pause();
	_arm->getAnimation()->setMovementEventCallFunc(this, SEL_MovementEventCallFunc(&Soilder::atk));
	this->addChild(_arm);

	//����Ч��
	_skill1 = Armature::create("NewAnimation");
	_skill1->getAnimation()->pause();
	_skill1->setPositionY(60);
	this->addChild(_skill1);

	//λ��
	this->setLocalZOrder((int)_pos.x + (int)_pos.y * 10000);
	//����
	this->setScale(0.8);
	
	//ѡ�п�
	auto scaleUp = ScaleTo::create(0.3f, 1.1f);
	auto scaleDown = ScaleTo::create(0.3f, 1.0f);
	_circle = Sprite::create(IMG_CIRCLE);
	_circle->runAction(RepeatForever::create(Sequence::create(scaleUp, scaleDown, nullptr)));
	_circle->setVisible(false);
	_circle->pause();
	this->addChild(_circle, -1);

	//��������
    _objname    = objInfo["Name"].asString();
	_quality	= objInfo["Quality"].asInt();
	this->setObjName(_objname);

	//���
	_kind  = objInfo["Kind"].asInt();

	//Ѫ��
	addHPBar();
}


void Hero::addHPBar()
{
	auto bg = Sprite::create(IMG_BUILD_PRO_BK);
	if (_camp == 1){
		_hpBar = LoadingBar::create(IMG_BUILD_PRO);
	}
	else{
		_hpBar = LoadingBar::create(IMG_BUILD_PRO_ENEMY);
	}
	
	bg->setPosition(0, _arm->getContentSize().height / 2 - 20);
	_hpBar->setPosition(bg->getContentSize() / 2);
	bg->addChild(_hpBar);
	this->addChild(bg, 9, "Bar");

	_hpBar->setPercent(100.0 * _healthPoint / _totalHP);

	char str[128] = { 0 };
	sprintf(str, "%d", _healthPoint);
	_txt_hp = Text::create(str, FONT_ARIAL, 18);
	_txt_hp->setPosition(bg->getPosition());
	_txt_hp->setLocalZOrder(100000);
	this->addChild(_txt_hp);

	
}

void Hero::changeHpBar(){
	if (_hpBar){		
		if (_camp == 1){
			_hpBar->loadTexture(IMG_BUILD_PRO);
		}
		else{
			_hpBar->loadTexture(IMG_BUILD_PRO_ENEMY);
		}
	}
}

void Hero::idle()
{
	_state = STATE_IDLE;
	_arm->getAnimation()->play("idle" + GM()->getIntToStr(_dir));
}


void Hero::atk(Armature* arm, MovementEventType eventType, const std::string& str)
{
	int x = arm->getAnimation()->getCurrentMovementID().find("atk");
	if (x >= 0) {
		if (_target == nullptr){
			return;
		}
		if (_target->_isbroken == true || _target->_healthPoint <= 0) {
			return;
		}

		this->_mgr_skill->triggerSkill(SkillTriggerType::Atk, _target->getPosition());

		if (eventType == LOOP_COMPLETE) {
			if (_shootType == 2) {
				Vec2 src = getPosition() + Vec2(0, 60);
				Vec2 des = _target->getPosition() + Vec2(0, 60);
				auto bullet = BulletSprite::create(src, des, getDamage(), this, _target, IMG_BULLET_ARROW, 2);
				bullet->setScale(getScale());
				this->getParent()->addChild(bullet, 9999999);
				//SimpleAudioEngine::getInstance()->playEffect("music/far_gongjian_effect.mp3", false);
			}
			else if (_shootType == 3) {
				Vec2 src = getPosition() + Vec2(0, 60);
				Vec2 des = _target->getPosition() + Vec2(0, 60);
				auto bullet = BulletSprite::create(src, des, getDamage(), this, _target, "images/bullet/fashu.png", 2);
				bullet->setScale(getScale());
				this->getParent()->addChild(bullet, 9999999);
				//SimpleAudioEngine::getInstance()->playEffect("music/far_fashu_effect.mp3", false);
			}
			else {
				if (_target->_objType == 3) {
					_target->hurt(1, 2, this);
				}
				else {
					_target->hurt(1, getDamage(), this);
				}

				//SimpleAudioEngine::getInstance()->playEffect("music/near_atk_effect.mp3", false);
			}

			if (_target->_objType == 3) {
				//��������������ʧ
				this->death();
			}
		}
		return;
	}
	x = arm->getAnimation()->getCurrentMovementID().find("dead");
	if (x >= 0) {
		if (eventType == LOOP_COMPLETE) {
			this->death();

			this->_mgr_skill->triggerSkill(SkillTriggerType::Dead, this->getPosition());
		}
	}
}


// //����
//void Hero::hurt(int hurtType, int x, BaseSprite* atk)
//{
//	if (_isbroken == true || _healthPoint <= 0) {
//		//_arm->getAnimation()->stop();
//		_ai->setObjDead(this);
//		_isbroken = true;
//		return;
//	}
//
//	int temp = 0;
//	switch (hurtType){
//		case 1:	//�����˺�
//		{
//			//�˺�-����
//			temp = x - getDef();
//			//�����Ʒ���1Ѫ
//			temp = temp <= 0 ? 1 : temp;
//
//			atk->hurt(3, temp, nullptr);
//
//			_healthPoint -= ((1.0 + this->_hurt_more / 100) * temp);
//			break;
//		}
//		case 2://�����˺�
//		{
//			temp = x;
//			_healthPoint -= ((1.0 + this->_hurt_more / 100) * temp);			
//			_healthPoint = min(_healthPoint, _totalHP);
//			break;
//		}
//		case 3://��Ѫ
//		{
//			if (this->_xixue <= 0) {
//				return;
//			}
//			temp =  - x * this->_xixue / 100;
//			_healthPoint -= ((1.0 + this->_hurt_more / 100) * temp);
//			_healthPoint = min(_healthPoint, _totalHP);
//			break;
//		}
//	}
//
//
//
//	if (_healthPoint <= 0) {
//		_isbroken = true;
//		//this->setVisible(false);
//		_ai->setObjDead(this);
//		setState(STATE_DEATH, _dir);
//		//_arm->getAnimation()->stop();
//	}
//	else {
//		_hpBar->setPercent(100.0 * _healthPoint / _totalHP);
//		_txt_hp->setString(cocos2d::Value(_healthPoint).asString());
//	}
//
//	//Ʈ��
//	Vec2 txtPos = Vec2(40, _arm->getContentSize().height / 2 - 20);
//	Color4B txtColor(255, 0, 0, 255);
//	if (temp < 0) {
//		txtColor = Color4B(0, 255, 0, 255);
//	}
//	Text* txtHurt = this->flyHurtNum(temp, txtPos);
//	txtHurt->setTextColor(txtColor);
//
//	this->_mgr_skill->triggerSkill(SkillTriggerType::Hurt, this->getPosition());
//}

void Hero::hurtEffect(int x) {
	_skill1->getAnimation()->play("idle" + GM()->getIntToStr(x), -1, 0);
}

void Hero::death()
{
	_healthPoint = 0;
	_arm->getAnimation()->stop();
	_ai->setObjDead(this);
	setVisible(false);
	_isbroken = true;

	//�Ƴ�����Ч��
	for (auto& it : this->_skill_effects){
		CC_SAFE_DELETE(it.second);
	}
	this->_skill_effects.clear();
}

// ���ܽ���
void Hero::finishSkill(Armature* arm, int state)
{
	_state = state;
	arm->setVisible(false);
	arm->getAnimation()->stop();
}

// �ͷż���
void Hero::putSkill(int type)
{
	int state = _state;
	_state = STATE_SKILL;
	// ����1
	if (type == 1) {
		_skill1->setVisible(true);
		_skill1->getAnimation()->play("Skill");
		auto delay = DelayTime::create(1.0f);
		auto func = CallFunc::create(CC_CALLBACK_0(Hero::finishSkill, this, _skill1, state));
		this->runAction(Sequence::create(delay, func, nullptr));
	}
	// ����2
	else {
		_skill2->setVisible(true);
		_skill2->getAnimation()->play("Skill");
		auto delay = DelayTime::create(1.0f);
		auto func = CallFunc::create(CC_CALLBACK_0(Hero::finishSkill, this, _skill2, state));
		this->runAction(Sequence::create(delay, func, nullptr));
	}
}

// �Ƿ�����
bool Hero::isDeath()
{
	return _isbroken;
}

void Hero::addTouch()
{
	//auto dispatcher = this->getEventDispatcher();
	//auto listener = EventListenerTouchOneByOne::create();
	//listener->onTouchBegan = CC_CALLBACK_2(Hero::onTouchBegan, this);
	//listener->onTouchMoved = CC_CALLBACK_2(Hero::onTouchMoved, this);
	//listener->onTouchEnded = CC_CALLBACK_2(Hero::onTouchEnded, this);
	//dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

bool Hero::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	// ��ȡ�¼����󶨵� target, ͨ����cc.Node�������� 
	auto target = pEvent->getCurrentTarget();

	// ��ȡ��ǰ����������ڰ�ť���ڵ�����	
	auto locationInNode = target->convertToNodeSpace(pTouch->getLocation());
	int dis = locationInNode.getDistance(Vec2(0, 60));
	if (dis < _radius) {
		_ai->setSelectObj(this);
	}

	return true;
}

void Hero::onTouchMoved(Touch* pTouch, Event* pEvent)
{
	//    _delta += GM()->getDistance(Vec2(0, 0), pTouch->getDelta());
}

void Hero::onTouchEnded(Touch* pTouch, Event* pEvent)
{
	this->_mgr_skill->triggerSkill(SkillTriggerType::Hand, this->getPosition());
}


//void Hero::update(float dt)
//{
//	if (_isbroken == true) {
//		this->unscheduleAllCallbacks();
//		return;
//	}
//
//
//	switch (_state)
//	{
//		case STATE_WIN:// 
//		{
//			int tempDir = GM()->getDir(getPosition(), _target->getPosition());
//			setState(STATE_IDLE, _dir);
//			break;
//		}
//		case STATE_YUN:// ѣ��
//		{
//			setState(STATE_YUN, _dir);
//			break;
//		}
//		case STATE_IDLE:// ����
//		{
//			setState(STATE_IDLE, _dir);
//			break;
//		}
//		case STATE_RUN:// ��·		
//		{
//			int minDis = 100000;
//			_target = nullptr;
//
//
//			switch (_camp)
//			{
//				case 1:
//					for (auto it : _ai->_objEnemy) {
//						if (it->isDeath()) {
//							continue;
//						}
//						int dis = (int)it->getPosition().getDistance(this->getPosition());
//						if (dis < minDis) {
//							minDis = dis;
//							_target = it;
//						}
//					}
//					break;
//				case 2:
//					for (auto it : _ai->_objSelf) {
//						if (it->isDeath()) {
//							continue;
//						}
//						int dis = (int)it->getPosition().getDistance(this->getPosition());
//						if (dis < minDis) {
//							minDis = dis;
//							_target = it;
//						}
//					}
//					break;
//			}
//
//
//			if (_target == nullptr) {
//				setState(STATE_IDLE, _dir);
//			}
//			else if (_target->_isbroken == false && _ai->isWithinShootRange(getPosition(), _target->getPosition(), _shoot_range)) {
//
//				int tempDir = GM()->getDir(getPosition(), _target->getPosition());
//				setState(STATE_ATK, tempDir);
//			}
//			else {
//				int tempDir = GM()->getDir(getPosition(), _target->getPosition());
//				setState(STATE_RUN, tempDir);
//
//				Vec2 disPos = _target->getPosition() - getPosition();
//				int speed = this->getSpeed();
//				Vec2 nextPos(getPositionX() + disPos.x * speed / minDis / 60, getPositionY() + disPos.y * speed / minDis / 60);
//
//				_ai->setObjPos(this, nextPos);
//			}
//			break;
//		}
//
//		case STATE_ATK:// ����
//		{
//			if (_target == nullptr || _target->isDeath()) {
//				// ʧȥĿ�꣬�������
//				_target = nullptr;
//
//				setState(STATE_RUN, _dir);
//			}
//
//			// ��Ŀ���ƶ����򹥻�Ŀ��
//			else {
//				int tempDir = GM()->getDir(getPosition(), _target->getPosition());
//				// ����
//				if (_ai->isWithinShootRange(getPosition(), _target->getPosition(), _shoot_range)) {
//					setState(STATE_ATK, tempDir);
//				}
//				// ��·
//				else {
//					setState(STATE_RUN, tempDir);
//				}
//			}
//			break;
//		}
//
//	}
//
//	BaseSprite::update(dt);
//}


void Hero::setSelect(bool b) {
	_circle->setVisible(b);
	if (b) {
		_circle->resume();
	}
	else {
		_circle->pause();
	}
}


void Hero::setObjName(string name) {

	if (this->_txtName == nullptr) {
		this->_txtName = Text::create("����", FONT_ARIAL, 20);
		_txtName->setName("txtName");
		this->addChild(_txtName);	
	}
	_objname = name;
	_txtName->setString(name);
	
	setTextColorByQuality(_txtName, _quality);
}

void Hero::setState(int state, int dir)
{
	if (_state == STATE_DEATH){
		return;
	}
	if (_state == state && _dir == dir) {
		return;
	}
	_state = state;
	_dir = dir;

	string dirCmd = GM()->getIntToStr(_dir);
	string animaCmd = "idle";
	switch (_state)
	{
		case STATE_IDLE:
			animaCmd = "idle";
			break;
		case STATE_RUN:
			animaCmd = "run";
			break;
		case STATE_ATK:
			animaCmd = "atk";
			break;
		case STATE_WIN:
			animaCmd = "run";
			break;
		case STATE_DEATH:
			animaCmd = "dead";
			break;
		case STATE_YUN:
			animaCmd = "idle";
			break;
	}

	_arm->getAnimation()->play(animaCmd + GM()->getIntToStr(_dir));
}