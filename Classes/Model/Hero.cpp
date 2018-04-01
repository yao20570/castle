#include "Hero.h"
#include "UI/BattleScene/AIManager.h"
#include "UI/BattleScene/BattleMapLayer.h"
#include "Dlg/Fight/AIMgr.h"
#include "Utils/ConfigMgr.h"

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
	:_ai(nullptr)
{

}

Hero::~Hero() 
{
	
}

bool Hero::init(int ID, AIMgr* ai, int camp)
{
    if ( !BaseSprite::init() ) {
        return false;
    }
	_radius = 40;
	_camp = camp;
	_ai = ai;
	_ai->addHero(this, _camp);
    _id = ID;
    _target = nullptr;
    _isSelect = true;
    _state = 0;
    
    loadData();
    showUI();
    addHPBar();
    
    addTouch();
    
    schedule(schedule_selector(Hero::update), 0.81f);
    
    return true;
}


void Hero::loadData()
{
    ValueMap& data = *(CFG()->getHeroInfoById(_id));
    
    _heroID         = data["HeroID"].asInt();
    _type           = data["Type"].asInt();
    _level          = data["Level"].asInt();
    _name           = data["Name"].asInt();
    
    _healthPoint    = data["HealthPoint"].asInt();
    _totalHP        = _healthPoint;
    _damage         = data["Damage"].asInt();
    _attackSpeed    = data["AttackSpeed"].asInt();
    _shootRange     = data["ShootRange"].asInt();
    
    _isbroken       = false;
    _expReward      = 0;
}


void Hero::showUI()
{
	int y = _camp == 1 ? -1 : 1;
	_dir = GM()->getDir(Vec2(0, y));
    _arm = Armature::create(ANIM_NAME_ARAGORN);
    _arm->getAnimation()->play("idle" + GM()->getIntToStr(_dir));
	_arm->setPositionY(20);
	_arm->pause();

    this->addChild(_arm);
    
    _skill1 = Armature::create(ANIM_NAME_SKILL_1);
    _skill2 = Armature::create(ANIM_NAME_SKILL_2);
    _skill1->setVisible(false);
    _skill2->setVisible(false);
    this->addChild(_skill1);
    this->addChild(_skill2);
	this->setLocalZOrder((int)_pos.x + (int)_pos.y * 10000);
	this->setScale(0.8);

	//ѡ�п�
	auto scaleUp = ScaleTo::create(0.3f, 1.1f);
	auto scaleDown = ScaleTo::create(0.3f, 1.0f);
    _circle = Sprite::create(IMG_CIRCLE);
    _circle->runAction(RepeatForever::create(Sequence::create(scaleUp, scaleDown, nullptr)));
	_circle->setVisible(false);
	_circle->pause();
    this->addChild(_circle, -1);    
}


void Hero::addHPBar()
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


void Hero::idle()
{
    _state = STATE_IDLE;
    _arm->getAnimation()->play("idle" + GM()->getIntToStr(_dir));
}


void Hero::atk(BaseSprite* target)
{
    if (_isbroken == true || _healthPoint <= 0) {
        return;
    }
    
    _expReward += _damage/50.0;
    target->hurt(_damage);
}


// ����
void Hero::hurt(int x)
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


//void Hero::setSelect(bool select)
//{
//    _isSelect = select;
//}


void Hero::addTouch()
{
    auto dispatcher = this->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(Hero::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(Hero::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(Hero::onTouchEnded, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

bool Hero::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	// ��ȡ�¼����󶨵� target, ͨ����cc.Node�������� 
	auto target = pEvent->getCurrentTarget();
	
	// ��ȡ��ǰ����������ڰ�ť���ڵ�����	
	auto locationInNode = target->convertToNodeSpace(pTouch->getLocation());
	int dis = locationInNode.getDistance(Vec2(0, 0));
	if (dis < _radius) {
		_ai->setSelectObj(this);
		return true;
	}
		
    return false;
}

void Hero::onTouchMoved(Touch* pTouch, Event* pEvent)
{
//    _delta += GM()->getDistance(Vec2(0, 0), pTouch->getDelta());
}

void Hero::onTouchEnded(Touch* pTouch, Event* pEvent)
{
  //  if (_delta <= LIMIT_DELTA) {
		//BattleMapLayer* layer = (BattleMapLayer*)this->getParent();
  //      
  //      auto p = layer->convertToNodeSpace(pTouch->getLocation());
  //      Vec2 pos = GM()->getTiledPos(p);
  //      
  //      // �ƶ�
  //      _target = _ai->getTarget(pos);
  //      
  //      if (_target == nullptr) {
  //          _state = STATE_RUN;
  //          _targetPos = pos;
  //      }
  //      
  //      // ����Ŀ��
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


void Hero::update(float dt)
{
    //if (_isbroken == true) {
    //    this->unscheduleAllCallbacks();
    //    return;
    //}

    //switch (_state) {
    //    
    //    // ����
    //    case STATE_IDLE: {
    //        _target = nullptr;
    //    }
    //        break;
    //    
    //    // ��·
    //    case STATE_RUN: {
    //        _target = nullptr;
    //        
    //        // ����Ŀ�ĵ�
    //        if (fabs(_pos.x - _targetPos.x) <= 1e-3 && fabs(_pos.y - _targetPos.y) <= 1e-3) {
    //            _state = STATE_IDLE;
    //            _arm->getAnimation()->play("idle" + GM()->getIntToStr(_dir));
    //        }
    //        
    //        // �����ƶ�
    //        else {
    //            _state = STATE_RUN;
    //                            
    //            Vec2 pos = _ai->getNextPos(_pos, _targetPos, true);
    //            _dir = GM()->getDir(pos);
    //            
    //            this->runAction(MoveBy::create(0.8f, GM()->getMapDelta(_dir)));
    //            _arm->getAnimation()->play("run" + GM()->getIntToStr( _dir <= 7 ? _dir : 1));
    //            
    //            _pos += pos;
    //            this->setLocalZOrder((int)_pos.x + (int)_pos.y);
    //        }
    //    }
    //        break;
    //        
    //    // ����
    //    case STATE_ATK: {
    //        
    //        // ʧȥĿ�꣬�������
    //        if (_target == nullptr || _target->isDeath()) {
    //            _target = nullptr;
    //            _state = STATE_IDLE;
    //            _arm->getAnimation()->play("idle" + GM()->getIntToStr(_dir));
    //        }
    //        
    //        // ��Ŀ���ƶ����򹥻�Ŀ��
    //        else {
    //            _state = STATE_ATK;
    //            
    //            // ����
    //            if (_ai->isWithinShootRange(_pos, _target->_pos, _shootRange)) {
    //                _dir = GM()->getDir(_pos, _target->_pos);
    //                
    //                _arm->getAnimation()->play("atk" + GM()->getIntToStr(_dir));
    //                
    //                auto delay = DelayTime::create(0.6f);
    //                auto func = CallFunc::create(CC_CALLBACK_0(Hero::atk, this, _target));
    //                this->runAction(Sequence::create(delay, func, nullptr));
    //            }
    //            
    //            // ��·
    //            else {
    //                Vec2 pos = _ai->getNextPos(_pos, _target->_pos, true);
    //                _dir = GM()->getDir(pos);
    //                
    //                this->runAction(MoveBy::create(0.8f, GM()->getMapDelta(_dir)));
    //                _arm->getAnimation()->play("run" + GM()->getIntToStr( _dir <= 7 ? _dir : 1));
    //                
    //                _pos += pos;
    //                this->setLocalZOrder((int)_pos.x + (int)_pos.y);
    //            }
    //        }
    //    }
    //    default:
    //        break;
    //}
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