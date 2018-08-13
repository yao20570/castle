#include "Soilder.h"
#include "Model/BulletSprite.h"
#include "Dlg/Fight/AIMgr.h"
#include "Utils/ConfigMgr.h"
#include "Utils/UIUtils.h"
#include "Skill/SkillMgr.h"

Soilder* Soilder::create(int soilderID, AIMgr* ai, int camp)
{
    Soilder *pRet = new(std::nothrow) Soilder();
    if (pRet && pRet->init(soilderID, ai, camp)) {
        pRet->autorelease();
        return pRet;
    }
    else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

Soilder::Soilder()
{
}

Soilder::~Soilder() 
{
	
}

bool Soilder::init(int soilderID, AIMgr* ai, int camp)
{
    if ( !BaseSprite::init() ) {
        return false;
    }
	this->_txtName = nullptr;
	_objType = 1;
	_id = soilderID;
    _target = nullptr;
	_camp = camp;
    _ai = ai;
	_ai->addObj(this, _camp);
	_radius = 25;
    
	this->_mgr_skill = new SkillMgr(this, _ai);

    loadData();
    showUI();
	addTouch();
    schedule(schedule_selector(Soilder::update));
    
    return true;
}


void Soilder::loadData()
{
	ValueMap& data = *CFG()->getSoilderInfoById(_id);
    
    _type           = data["Type"].asInt();
    _level          = data["Level"].asInt();

	_speed			= data["Move"].asInt();
	_shootType		= data["Shoot"].asInt();
    _healthPoint    = data["HealthPoint"].asInt();
    _totalHP        = _healthPoint;
    _damage         = data["Damage"].asInt();
	_def			= data["Def"].asInt();
    _attackSpeed    = data["AttackSpeed"].asInt();
    _shoot_range     = data["ShootRange"].asInt();
    _isbroken       = false;
}


void Soilder::showUI()
{
	ValueMap& objInfo = *(CFG()->getObjInfoByType(1, _id));
	
	_is_boos = objInfo["IsBoss"].asBool();

	//动画
	_animaName	= objInfo["Anima"].asString();
	_arm = Armature::create(_animaName);
	int y = _camp == 1 ? -1 : 1;
	_dir = GM()->getDir(Vec2(0, y));
	_arm->getAnimation()->play("idle" + GM()->getIntToStr(_dir));
	_arm->setPositionY(20); 
	_arm->pause();
	_arm->getAnimation()->setMovementEventCallFunc(this, SEL_MovementEventCallFunc(&Soilder::atk));
    this->addChild(_arm);    
    this->setLocalZOrder((int)_pos.x + (int)_pos.y * 10000);    

	//技能效果
	_skill1 = Armature::create("NewAnimation");
	_skill1->getAnimation()->pause();
	_skill1->setPositionY(60);
	this->addChild(_skill1);


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

	//血条
    addHPBar();
}
void Soilder::addTouch()
{
	auto dispatcher = this->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Soilder::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Soilder::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Soilder::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

bool Soilder::onTouchBegan(Touch* pTouch, Event* pEvent)
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

void Soilder::onTouchMoved(Touch* pTouch, Event* pEvent)
{
	//    _delta += GM()->getDistance(Vec2(0, 0), pTouch->getDelta());
}

void Soilder::onTouchEnded(Touch* pTouch, Event* pEvent)
{
	//  if (_delta <= LIMIT_DELTA) {
	//BattleMapLayer* layer = (BattleMapLayer*)this->getParent();
	//      
	//      auto p = layer->convertToNodeSpace(pTouch->getLocation());
	//      Vec2 pos = GM()->getTiledPos(p);
	//      
	//      // 移动
	//      _target = _ai->getTarget(pos);
	//      
	//      if (_target == nullptr) {
	//          _state = STATE_RUN;
	//          _targetPos = pos;
	//      }
	//      
	//      // 锁定目标
	//      else {
	//          _dir = GM()->getDir(_pos, pos);
	//          if (_target->isDeath() == true) {
	//              _state = STATE_IDLE;
	//          }
	//          else {
	//              _state = STATE_ATK;
	//          }
	//      }
	//  }
}

void Soilder::addHPBar()
{
	auto bg = Sprite::create(IMG_BUILD_PRO_BK);	
	if (_camp == 1){
		_hpBar = LoadingBar::create(IMG_BUILD_PRO);
	}
	else{
		_hpBar = LoadingBar::create(IMG_BUILD_PRO_ENEMY);
	}

	bg->setPosition(0, _arm->getContentSize().height / 2 - 20);
    _hpBar->setPosition(bg->getContentSize()/2);
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


//void Soilder::update(float dt)
//{
//	if (_isbroken == true) {
//		this->unscheduleAllCallbacks();
//		return;
//	}
//
//	if (_obj_states.count(EffectState::Yun) > 0){
//		_arm->getAnimation()->play("idle" + GM()->getIntToStr(_dir));
//	}
//	else{
//		switch (_state)
//		{
//			case STATE_IDLE:// 悠闲
//			{
//				setState(STATE_IDLE, _dir);
//				break;
//			}
//			case STATE_RUN:// 走路		
//			{
//				int minDis = 100000;
//				_target = nullptr;
//
//
//				switch (_camp)
//				{
//					case 1:
//						for (auto it : _ai->_objEnemy) {
//							if (it->isDeath()) {
//								continue;
//							}
//							int dis = (int)it->getPosition().getDistance(this->getPosition());
//							if (dis < minDis) {
//								minDis = dis;
//								_target = it;
//							}
//						}
//						break;
//					case 2:
//						for (auto it : _ai->_objSelf) {
//							if (it->isDeath()) {
//								continue;
//							}
//							int dis = (int)it->getPosition().getDistance(this->getPosition());
//							if (dis < minDis) {
//								minDis = dis;
//								_target = it;
//							}
//						}
//						break;
//				}
//
//
//				if (_target == nullptr) {
//					setState(STATE_IDLE, _dir);
//				}
//				else if (_target->_isbroken == false && _ai->isWithinShootRange(getPosition(), _target->getPosition(), _shoot_range)) {
//
//					int tempDir = GM()->getDir(getPosition(), _target->getPosition());
//					setState(STATE_ATK, tempDir);
//				}
//				else {
//					int tempDir = GM()->getDir(getPosition(), _target->getPosition());
//					setState(STATE_RUN, tempDir);
//
//					Vec2 disPos = _target->getPosition() - getPosition();
//
//					Vec2 nextPos(getPositionX() + disPos.x * getSpeed() / minDis / 60, getPositionY() + disPos.y * getSpeed() / minDis / 60);
//
//					_ai->setObjPos(this, nextPos);
//				}
//				break;
//			}
//
//			case STATE_ATK:// 攻击
//			{
//				if (_target == nullptr || _target->isDeath()) {
//					// 失去目标，变成悠闲
//					_target = nullptr;
//
//					setState(STATE_RUN, _dir);
//				}
//
//				// 向目标移动、或攻击目标
//				else {
//					int tempDir = GM()->getDir(getPosition(), _target->getPosition());
//					// 攻击
//					if (_ai->isWithinShootRange(getPosition(), _target->getPosition(), _shoot_range)) {
//						setState(STATE_ATK, tempDir);
//					}
//					// 走路
//					else {
//						setState(STATE_RUN, tempDir);
//					}
//				}
//				break;
//			}
//
//		}
//	}
//	BaseSprite::update(dt);
//}


void Soilder::atk(Armature* arm, MovementEventType eventType, const std::string& str)
{

	int x = _arm->getAnimation()->getCurrentMovementID().find("atk");
	if (x >= 0) {
		if (_target == nullptr){
			return;
		}
		if (_target->_isbroken == true || _target->_healthPoint <= 0) {
			return;
		}
		if (eventType == START) {
			this->_mgr_skill->triggerSkill(SkillTriggerType::Atk, _target->getPosition());
		}
		else if (eventType == LOOP_COMPLETE) {
			if (_shootType == 2) {
				Vec2 src = getPosition() + Vec2(0, 60);
				Vec2 des = _target->getPosition() + Vec2(0, 60);
				auto bullet = BulletSprite::create(src, des, getDamage(), this, _target, IMG_BULLET_ARROW, 1);
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
					_target->hurt(1, 1, this);
				}
				else {
					_target->hurt(1, getDamage(), this);
				}
				//SimpleAudioEngine::getInstance()->playEffect("music/near_atk_effect.mp3", false);
			}

			if (_target->_objType == 3) {
				_healthPoint = 0;
				_arm->getAnimation()->stop();
				_ai->setObjDead(this);
				setVisible(false);
				_isbroken = true;
			}
		}
		return;
	}
	x = arm->getAnimation()->getCurrentMovementID().find("dead");
	if (x >= 0) {
		if (eventType == LOOP_COMPLETE)
		{
			setVisible(false);
			_arm->getAnimation()->stop();
		}
	}
}


//// 受伤
//void Soilder::hurt(int hurtType, int x, BaseSprite* atk)
//{
//    if (_isbroken == true || _healthPoint <= 0) {
//		//_arm->getAnimation()->stop();
//		_ai->setObjDead(this);
//		_isbroken = true;
//        return;
//    }
//    
//	int temp = 0;
//	switch (hurtType){
//		case 1:
//		{
//			//伤害-防御
//			temp = x - getDef();
//			//不能破防减1血
//			temp = temp <= 0 ? 1 : temp;
//			_healthPoint -= ((1.0 + this->_hurt_more / 100) * temp);
//			break;
//		}
//		case 2:
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
//    if (_healthPoint <= 0) {
//        _isbroken = true;
//        //this->setVisible(false);
//		_ai->setObjDead(this);
//		setState(STATE_DEATH, _dir);
//        //_arm->getAnimation()->stop();
//    }
//    else {
//        _hpBar->setPercent(100.0 * _healthPoint / _totalHP);
//		_txt_hp->setString(cocos2d::Value(_healthPoint).asString());
//    }
//
//	//飘字
//	Vec2 txtPos = Vec2(40, _arm->getContentSize().height / 2 - 20);
//	Color4B txtColor(255, 0, 0, 255);
//	if (temp<0) {
//		txtColor = Color4B(0, 255, 0, 255);
//	}
//	Text* txtHurt = this->flyHurtNum(temp, txtPos);
//	txtHurt->setTextColor(txtColor);
//}

void Soilder::hurtEffect(int x, int bulletType) {
	_skill1->getAnimation()->play("idle" + GM()->getIntToStr(x), -1, 0);
}


// 是否死亡
bool Soilder::isDeath()
{
    return _isbroken;
}

void Soilder::setSelect(bool b) {
	_circle->setVisible(b);
	if (b) {
		_circle->resume();
	}
	else {
		_circle->pause();
	}
}

void Soilder::setObjName(string name) {

	if (this->_txtName == nullptr) {
		this->_txtName = Text::create("名称", FONT_ARIAL, 20);
		_txtName->setName("txtName");
		this->addChild(_txtName);
	}
	_objname = name;
	_txtName->setString(name);

	setTextColorByQuality(_txtName, _quality);
}

void Soilder::setState(int state, int dir)
{
	if (_state == state && _dir == dir) {
		return;
	}
	_state = state; 
	_dir = dir;

	string dirCmd = GM()->getIntToStr(_dir);
	string animaCmd = "run";
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
	case STATE_DEATH:
		animaCmd = "dead";
		break;
	}

	_arm->getAnimation()->play(animaCmd + GM()->getIntToStr(_dir));
}