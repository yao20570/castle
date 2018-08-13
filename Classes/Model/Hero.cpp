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
	//动画
	string animaName = objInfo["Anima"].asString();
	int y = _camp == 1 ? -1 : 1;
	_dir = GM()->getDir(Vec2(0, y));
	_arm = Armature::create(animaName);
	_arm->getAnimation()->play("idle" + GM()->getIntToStr(_dir));
	_arm->setPositionY(20);
	_arm->pause();
	_arm->getAnimation()->setMovementEventCallFunc(this, SEL_MovementEventCallFunc(&Soilder::atk));
	this->addChild(_arm);

	//技能效果
	//_skill1 = Armature::create("NewAnimation");
	//_skill1->getAnimation()->pause();
	//_skill1->setPositionY(60);
	//this->addChild(_skill1);

	//位置
	this->setLocalZOrder((int)_pos.x + (int)_pos.y * 10000);
	//缩放
	this->setScale(0.8);
	
	//选中框
	auto scaleUp = ScaleTo::create(0.3f, 1.1f);
	auto scaleDown = ScaleTo::create(0.3f, 1.0f);
	_circle = Sprite::create(IMG_CIRCLE);
	_circle->runAction(RepeatForever::create(Sequence::create(scaleUp, scaleDown, nullptr)));
	_circle->setVisible(false);
	_circle->pause();
	this->addChild(_circle, -1);

	//设置名称
    _objname    = objInfo["Name"].asString();
	_quality	= objInfo["Quality"].asInt();
	this->setObjName(_objname);

	//类别
	_kind  = objInfo["Kind"].asInt();

	//血条
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


	if (this->_camp == 1){
		//技能冷却
		vector<Skill*>* skills = this->_mgr_skill->getSkills(SkillTriggerType::Hand);
		if (skills->empty() == false){

			auto cdbg = Sprite::create(IMG_BUILD_PRO_BK);
			cdbg->setPosition(0, _arm->getContentSize().height / 2 - 30);
			this->addChild(cdbg);

			_cdBar = LoadingBar::create("images/pro_cd.png");
			_cdBar->setPercent(100.0);
			_cdBar->setPosition(cdbg->getContentSize() / 2);
			cdbg->addChild(_cdBar);
		}
	}
	
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
			else if (_shootType == 4) {
				Vec2 src = getPosition() + Vec2(0, 60);
				Vec2 des = _target->getPosition() + Vec2(0, 60);
				auto bullet = BulletSprite::create(src, des, getDamage(), this, _target, "images/bullet/sima.png", 2);
				bullet->setScale(getScale());
				this->getParent()->addChild(bullet, 9999999);
				//SimpleAudioEngine::getInstance()->playEffect("music/far_fashu_effect.mp3", false);
			}
			else if (_shootType == 5) {
				Vec2 src = getPosition() + Vec2(0, 60);
				Vec2 des = _target->getPosition() + Vec2(0, 60);
				auto bullet = BulletSprite::create(src, des, getDamage(), this, _target, "images/bullet/zhouyu.png", 2);
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
				//攻击完主公后消失
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


// //受伤
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
//		case 1:	//物理伤害
//		{
//			//伤害-防御
//			temp = x - getDef();
//			//不能破防减1血
//			temp = temp <= 0 ? 1 : temp;
//
//			atk->hurt(3, temp, nullptr);
//
//			_healthPoint -= ((1.0 + this->_hurt_more / 100) * temp);
//			break;
//		}
//		case 2://法术伤害
//		{
//			temp = x;
//			_healthPoint -= ((1.0 + this->_hurt_more / 100) * temp);			
//			_healthPoint = min(_healthPoint, _totalHP);
//			break;
//		}
//		case 3://吸血
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
//	//飘字
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

void Hero::hurtEffect(int x, int bulletType) {
	Armature* skillAnim = nullptr;
	switch (bulletType)
	{
		case 3:
			skillAnim = Armature::create("NewAnimation");
			break;
		case 4:
			skillAnim = Armature::create("NewAnimation_simayupugong");
			break;
		case 5:
			skillAnim = Armature::create("NewAnimation_zhouyupugong");
			break;
	}

	float angle = 0;
	switch (x)
	{
		case 0: angle = 90;			break;
		case 1: angle = 45;			break;
		case 2: angle = 0;			break;
		case 3: angle = -45;			break;
		case 4: angle = -90;			break;
		case 5: angle = -135;			break;
		case 6: angle = -180;			break;
		case 7: angle = -225;			break;
	}
	skillAnim->setRotation(angle);
	skillAnim->getAnimation()->pause();
	skillAnim->setPositionY(60);
	skillAnim->getAnimation()->play("Animation1");
	skillAnim->getAnimation()->setMovementEventCallFunc([this](Armature* arm, MovementEventType eventType, const std::string& str){
		if (eventType == LOOP_COMPLETE) {
			arm->runAction(RemoveSelf::create());
		}
	});

	
	this->addChild(skillAnim);
}

void Hero::death()
{
	_healthPoint = 0;
	_arm->getAnimation()->stop();
	_ai->setObjDead(this);
	setVisible(false);
	_isbroken = true;

	//移除技能效果
	for (auto& it : this->_skill_effects){
		CC_SAFE_DELETE(it.second);
	}
	this->_skill_effects.clear();
}

// 技能结束
void Hero::finishSkill(Armature* arm, int state)
{
	_state = state;
	arm->setVisible(false);
	arm->getAnimation()->stop();
}

// 释放技能
void Hero::putSkill(int type)
{
	//int state = _state;
	//_state = STATE_SKILL;
	//// 技能1
	//if (type == 1) {
	//	_skill1->setVisible(true);
	//	_skill1->getAnimation()->play("Skill");
	//	auto delay = DelayTime::create(1.0f);
	//	auto func = CallFunc::create(CC_CALLBACK_0(Hero::finishSkill, this, _skill1, state));
	//	this->runAction(Sequence::create(delay, func, nullptr));
	//}
	//// 技能2
	//else {
	//	_skill2->setVisible(true);
	//	_skill2->getAnimation()->play("Skill");
	//	auto delay = DelayTime::create(1.0f);
	//	auto func = CallFunc::create(CC_CALLBACK_0(Hero::finishSkill, this, _skill2, state));
	//	this->runAction(Sequence::create(delay, func, nullptr));
	//}
}

// 是否死亡
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
	// 获取事件所绑定的 target, 通常是cc.Node及其子类 
	auto target = pEvent->getCurrentTarget();

	// 获取当前触摸点相对于按钮所在的坐标	
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


void Hero::update(float dt)
{
	BaseSprite::update(dt);

	if (_cdBar){
		vector<Skill*>* skills = this->_mgr_skill->getSkills(SkillTriggerType::Hand);
		if (skills->empty()){
			return;
		}

		Skill* skill = (*skills)[0];

		//技能效果开始,结束时间
		INT64 curMTimeStamp = GM()->getMTimeStamp();
		if (curMTimeStamp >= skill->m_CDMTimestamp){
			_cdBar->setPercent(100.0);
			if (_is_show_cd_anim == false){
				_is_show_cd_anim = true;

				Armature* skillAnim = Armature::create("NewAnimation_jineng");
				skillAnim->setPositionY(60);
				skillAnim->getAnimation()->play("Animation1");
				skillAnim->getAnimation()->setMovementEventCallFunc([this](Armature* arm, MovementEventType eventType, const std::string& str){
					if (eventType == LOOP_COMPLETE) {
						arm->runAction(RemoveSelf::create());
					}
				});
				this->addChild(skillAnim);
			}
		}
		else{
			float p = (skill->cd - (skill->m_CDMTimestamp - curMTimeStamp)) * 100 / skill->cd;
			_cdBar->setPercent(p);
			_is_show_cd_anim = false;
		}
	}
}


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
		this->_txtName = Text::create("名称", FONT_ARIAL, 20);
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