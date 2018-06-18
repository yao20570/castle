#include "Soilder.h"
#include "UI/BattleScene/AIManager.h"
#include "Model/BulletSprite.h"
#include "Dlg/Fight/AIMgr.h"
#include "Utils/ConfigMgr.h"

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
	:_ai(nullptr)	
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
	this->txtName = nullptr;
	_objType = 1;
	_speed = 50;
    _soilderID = soilderID;
    _target = nullptr;
	_camp = camp;
    _ai = ai;
	_ai->addSoilder(this, _camp);
	_radius = 25;
    
    loadData();
    showUI();
    addHPBar();
	addTouch();
    schedule(schedule_selector(Soilder::update));
    
    return true;
}


void Soilder::loadData()
{
    ValueMap& data = *CFG()->getSoilderInfoById(_soilderID);
    
    _type           = data["Type"].asInt();
    _level          = data["Level"].asInt();
    _name           = data["Name"].asInt();

	_shootType		= data["Shoot"].asInt();
    _healthPoint    = data["HealthPoint"].asInt();
    _totalHP        = _healthPoint;
    _damage         = data["Damage"].asInt();
    _attackSpeed    = data["AttackSpeed"].asInt();
    _shootRange     = data["ShootRange"].asInt();
    _isbroken       = false;
}


void Soilder::showUI()
{
	ValueMap& objInfo = *(CFG()->getObjInfoByType(1, _soilderID));
	_animaName = objInfo["Anima"].asString();
	_arm = Armature::create(_animaName);
	

    //switch (_type) {
    //    case SOILDER_TYPE_FIGHTER: {
    //        _arm = Armature::create(ANIM_NAME_FIGHTER);
    //    }
    //        break;
    //    case SOILDER_TYPE_BOWMAN: {
    //        _arm = Armature::create(ANIM_NAME_BOWMAN);
    //    }
    //        break;
    //    case SOILDER_TYPE_GUNNER: {
    //        _arm = Armature::create(ANIM_NAME_GUNNER);
    //    }
    //        break;
    //    case SOILDER_TYPE_MEAT: {
    //        _arm = Armature::create(ANIM_NAME_MEATSHIELD);
    //    }
    //        break;
    //    default:
    //        break;
    //}

	int y = _camp == 1 ? -1 : 1;
	_dir = GM()->getDir(Vec2(0, y));
	_arm->getAnimation()->play("idle" + GM()->getIntToStr(_dir));
	_arm->setPositionY(20); 
	_arm->pause();
	_arm->getAnimation()->setMovementEventCallFunc(this, SEL_MovementEventCallFunc(&Soilder::atk));
    this->addChild(_arm);    
    this->setLocalZOrder((int)_pos.x + (int)_pos.y * 10000);    

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
	int dis = locationInNode.getDistance(Vec2(0, 0));
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
    _hpBar = LoadingBar::create(IMG_BUILD_PRO);
    bg->setPosition(0, _arm->getContentSize().height/2 + 30);
    _hpBar->setPosition(bg->getContentSize()/2);
    bg->addChild(_hpBar);
    this->addChild(bg, 9, "Bar");
    
    bg->setScale(0.7);
    _hpBar->setPercent(100.0 * _healthPoint / _totalHP);
}


void Soilder::update(float dt)
{
    if (_isbroken == true) {
        this->unscheduleAllCallbacks();
        return;
    }
    
	switch (_state)
	{
	case STATE_IDLE:// 悠闲
	{
		setState(STATE_IDLE, _dir);
		break;
	}
	case STATE_RUN:// 走路		
	{
		int minDis = 100000;
		_target = nullptr;


		switch (_camp)
		{
		case 1:
			for (auto it : _ai->_objEnemy) {
				if (it->isDeath()) {
					continue;
				}
				int dis = (int)it->getPosition().getDistance(this->getPosition());
				if (dis < minDis) {
					minDis = dis;
					_target = it;
				}
			}
			break;
		case 2:
			for (auto it : _ai->_objSelf) {
				if (it->isDeath()) {
					continue;
				}
				int dis = (int)it->getPosition().getDistance(this->getPosition());
				if (dis < minDis) {
					minDis = dis;
					_target = it;
				}
			}
			break;
		}


		if (_target == nullptr) {
			setState(STATE_IDLE, _dir);
			_target = nullptr;
		}
		else if (_target->_isbroken ==false && _ai->isWithinShootRange(getPosition(), _target->getPosition(), _shootRange)) {

			int tempDir = GM()->getDir(getPosition(), _target->getPosition());
			setState(STATE_ATK, tempDir);
		}
		else {
			int tempDir = GM()->getDir(getPosition(), _target->getPosition());
			setState(STATE_RUN, tempDir);

			Vec2 disPos = _target->getPosition() - getPosition();

			Vec2 nextPos(getPositionX() + disPos.x * _speed / minDis / 60, getPositionY() + disPos.y * _speed / minDis / 60);

			_ai->setObjPos(this, nextPos);
		}
		break;
	}

	case STATE_ATK:// 攻击
	{
		if (_target == nullptr || _target->isDeath()) {
			// 失去目标，变成悠闲
			_target = nullptr;

			setState(STATE_RUN, _dir);
		}

		// 向目标移动、或攻击目标
		else {
			int tempDir = GM()->getDir(getPosition(), _target->getPosition());
			// 攻击
			if (_ai->isWithinShootRange(getPosition(), _target->getPosition(), _shootRange)) {
				setState(STATE_ATK, tempDir);
			}
			// 走路
			else {
				setState(STATE_RUN, tempDir);
			}
		}
		break;
	}

	}
}


void Soilder::run()
{
    
}


void Soilder::atk(Armature* arm, MovementEventType eventType, const std::string& str)
{
	if (eventType == LOOP_COMPLETE)
	{
		int x = _arm->getAnimation()->getCurrentMovementID().find("atk");
		if (x >= 0) {
			if (_shootType == 2) {
				Vec2 src = getPosition() + Vec2(0, 60);
				Vec2 des = _target->getPosition() + Vec2(0, 60);
				auto bullet = BulletSprite::create(src, des, _damage, _target, IMG_BULLET_ARROW, 1);
				bullet->setScale(getScale());
				this->getParent()->addChild(bullet, 9999999);
				SimpleAudioEngine::getInstance()->playEffect("music/far_gongjian_effect.mp3", false);
			}
			else if (_shootType == 3) {
				Vec2 src = getPosition() + Vec2(0, 60);
				Vec2 des = _target->getPosition() + Vec2(0, 60);
				auto bullet = BulletSprite::create(src, des, _damage, _target, "images/bullet/fashu.png", 2);
				bullet->setScale(getScale());
				this->getParent()->addChild(bullet, 9999999);
				SimpleAudioEngine::getInstance()->playEffect("music/far_fashu_effect.mp3", false);
			}
			else {
				if (_target->_objType == 3) {
					_target->hurt(1);
				}
				else {
					_target->hurt(_damage);
				}
				SimpleAudioEngine::getInstance()->playEffect("music/near_atk_effect.mp3", false);
			}

			if (_target->_objType == 3) {
				_healthPoint = 0;
				_arm->getAnimation()->stop();
				_ai->setObjDead(this);
				setVisible(false);
				_isbroken = true;
			}
			return;
		}
		x = arm->getAnimation()->getCurrentMovementID().find("dead");
		if (x >= 0) {
			setVisible(false);
			_arm->getAnimation()->stop();
		}
	}
}


// 受伤
void Soilder::hurt(int x)
{
    if (_isbroken == true || _healthPoint <= 0) {
		//_arm->getAnimation()->stop();
		_ai->setObjDead(this);
		_isbroken = true;
        return;
    }
    
    _healthPoint -= x;
    if (_healthPoint <= 0) {
        _isbroken = true;
        //this->setVisible(false);
		_ai->setObjDead(this);
		setState(STATE_DEATH, _dir);
        //_arm->getAnimation()->stop();
    }
    else {
        _hpBar->setPercent(100.0 * _healthPoint / _totalHP);
    }
}

void Soilder::hurtEffect(int x) {
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

	if (this->txtName == nullptr) {
		this->txtName = Text::create("名称", FONT_ARIAL, 20);
		txtName->setName("txtName");
		this->addChild(txtName);
	}
	_objname = name;
	txtName->setString(name);
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