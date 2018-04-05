#include "Soilder.h"
#include "UI/BattleScene/AIManager.h"
#include "Model/BulletSprite.h"
#include "Dlg/Fight/AIMgr.h"

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
    schedule(schedule_selector(Soilder::update), 0.8f);
    
    return true;
}


void Soilder::loadData()
{
    ValueMap& data = DM()->getSoilderInfo(_soilderID);
    
    _type           = data["Type"].asInt();
    _level          = data["Level"].asInt();
    _name           = data["Name"].asInt();
    
    _healthPoint    = data["HealthPoint"].asInt();
    _totalHP        = _healthPoint;
    _damage         = data["Damage"].asInt();
    _attackSpeed    = data["AttackSpeed"].asInt();
    _shootRange     = data["ShootRange"].asInt();
    
    _isbroken       = false;
}


void Soilder::showUI()
{
    switch (_type) {
        case SOILDER_TYPE_FIGHTER: {
            _arm = Armature::create(ANIM_NAME_FIGHTER);
        }
            break;
        case SOILDER_TYPE_BOWMAN: {
            _arm = Armature::create(ANIM_NAME_BOWMAN);
        }
            break;
        case SOILDER_TYPE_GUNNER: {
            _arm = Armature::create(ANIM_NAME_GUNNER);
        }
            break;
        case SOILDER_TYPE_MEAT: {
            _arm = Armature::create(ANIM_NAME_MEATSHIELD);
        }
            break;
        default:
            break;
    }

	int y = _camp == 1 ? -1 : 1;
	_dir = GM()->getDir(Vec2(0, y));
	_arm->getAnimation()->play("run" + GM()->getIntToStr(_dir));
	_arm->setPositionY(20);
    this->addChild(_arm);    
    this->setLocalZOrder((int)_pos.x + (int)_pos.y * 10000);    
    this->setScale(0.6);


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
    
    //if (_target == nullptr || _target->isDeath()) {
    //    this->stopAllActions();
    //    _target = _ai->getTargetEnemy(_pos);
    //}
    //if (_target == nullptr) {
    //    _arm->getAnimation()->play("run0");
    //    _arm->getAnimation()->stop();
    //    return;
    //}
    //
    //// 攻击
    //if (_ai->isWithinShootRange(_pos, _target->_pos, _shootRange)) {
    //    _dir = GM()->getDir(_pos, _target->_pos);
    //    
    //    _arm->getAnimation()->play("atk" + GM()->getIntToStr(_dir));
    //    
    //    auto delay = DelayTime::create(0.7f);
    //    auto func = CallFunc::create(CC_CALLBACK_0(Soilder::atk, this));
    //    this->runAction(Sequence::create(delay, func, nullptr));
    //}
    //
    //// 走路
    //else {
    //    Vec2 pos = _ai->getNextPos(_pos, _target->_pos, false);
    //    _dir = GM()->getDir(pos);
    //    
    //    this->runAction(MoveBy::create(1.0f, GM()->getMapDelta(_dir)));
    //    _arm->getAnimation()->play("run" + GM()->getIntToStr( _dir <= 7 ? _dir : 1));
    //    
    //    _pos += pos;
    //    this->setLocalZOrder((int)_pos.x + (int)_pos.y);
    //}
}


void Soilder::run()
{
    
}


void Soilder::atk()
{
    if (_isbroken == true || _healthPoint <= 0) {
        return;
    }
    if (_type == SOILDER_TYPE_BOWMAN) {
        Vec2 src = GM()->getMapPos(_pos);
        Vec2 des = GM()->getMapPos(_target->_pos);
        auto bullet = BulletSprite::create(src, des, _damage, _target, IMG_BULLET_ARROW);
        this->getParent()->addChild(bullet, 99);
    }
    else if(_type == SOILDER_TYPE_GUNNER) {
        Vec2 src = GM()->getMapPos(_pos);
        Vec2 des = GM()->getMapPos(_target->_pos);
        auto bullet = BulletSprite::create(src, des, _damage, _target, IMG_BULLET_SHELL);
        this->getParent()->addChild(bullet, 99);
    }
    else {
        _target->hurt(_damage);
    }
}


// 受伤
void Soilder::hurt(int x)
{
    if (_isbroken == true || _healthPoint <= 0) {
        _isbroken = true;
        return;
    }
    
    _healthPoint -= x;
    if (_healthPoint <= 0) {
        _isbroken = true;
        this->setVisible(false);
        _arm->getAnimation()->stop();
    }
    else {
        _hpBar->setPercent(100.0 * _healthPoint / _totalHP);
    }
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

void Soilder::setState(int state, int _dir) {

}