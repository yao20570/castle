#include "BattleMapLayer.h"


bool BattleMapLayer::init()
{
    if ( !PanZoomLayer::init() )
    {
        return false;
    }
    
    this->loadData();
    this->addAI();
    this->showUI();
    this->addMap();
    
    this->addTouch();
    
    this->enterAction();
    
    return true;
}

void BattleMapLayer::loadData()
{
    
}


void BattleMapLayer::addAI()
{
    _ai = AIManager::create();
    this->addChild(_ai);
    _ai->setMapLayer(this);
}


void BattleMapLayer::showUI()
{
    auto bg = Sprite::create(IMG_CHAPTER_BG);
    bg->setAnchorPoint(Vec2(0, 0));
    bg->setPosition(Vec2(0, 0));
    this->addChild(bg, -1);
    
    _mapSize = bg->getContentSize();
    
    this->setPanBoundsRect(Rect(_mapSize.width * -1.0, _mapSize.height * -1.0,
                                _mapSize.width * 1.0, _mapSize.height * 1.0));
    this->setMinScale(0.5f);
    this->setMaxScale(1.5f);
    this->setZoomDelta(0.0f, 0.1f);
    this->setScale(0.5f);
}


void BattleMapLayer::addMap()
{
    // ���ռ�����
    GM()->clearCovered();

    ValueVector vv = DM()->getBattleBuilding(GM()->getBattleTownID());
    
    // ��ӵз���ʩ
    for (int i = 1; i < vv.size(); i++) {
        auto building = Building::create(i, _ai);
        this->addChild(building);
        _ai->_buildings.pushBack(building);
    }
}


void BattleMapLayer::addHero(Vec2 pos)
{
    // ����Ӣ��
    if (_ai->_hero == nullptr) {
        ValueMap& map = DM()->getHero(1);
        auto hero = Hero::create(map["ID"].asInt(), pos);
        this->addChild(hero);
        _ai->_hero = hero;
    }
}


void BattleMapLayer::addSoilder(Vec2 pos)
{
    // ���ϰ���
    if (GM()->isCovered(pos)) {
        auto notice = Notice::create("�˴��޷�����ʿ��");
        this->getParent()->addChild(notice, 999);
    }
    
    // ʿ������
    else if(_hudLayer->updateCount() == false) {
        auto notice = Notice::create("��ʿ��������");
        this->getParent()->addChild(notice, 999);
    }
    
    // ����ʿ��
    else {
        ValueMap& map = DM()->getSoilder(_hudLayer->_select);
        auto s = Soilder::create(map["SoilderID"].asInt(), pos, _ai);
        this->addChild(s);
        _ai->_soilders.pushBack(s);
    }
}


void BattleMapLayer::setHudLayer(Layer* layer)
{
    _hudLayer = (BattleHudLayer*)layer;
    _ai->setHudLayer(_hudLayer);
}


// �����¼�
void BattleMapLayer::addTouch()
{
    auto dispatcher = this->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(BattleMapLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(BattleMapLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(BattleMapLayer::onTouchEnded, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool BattleMapLayer::onTouchBegan(Touch* pTouch, Event* pEvent)
{
    _delta = 0.0;
    return true;
}

void BattleMapLayer::onTouchMoved(Touch* pTouch, Event* pEvent)
{
    _delta += GM()->getDistance(Vec2(0, 0), pTouch->getDelta());
}

void BattleMapLayer::onTouchEnded(Touch* pTouch, Event* pEvent)
{
    if (_delta <= LIMIT_DELTA) {
        
        // �ű��������Ӣ�ۣ��ƶ�������������
        auto p = this->convertToNodeSpace(pTouch->getLocation());
        Vec2 pos = GM()->getTiledPos(p);
        
        // Ӣ��
        if (_hudLayer->_select == 0) {
            this->addHero(pos);
        }
        
        // ʿ��
        else {
            this->addSoilder(pos);
        }
    }
}


// ���ǳ��ƶ�����Ļ����, ��΢���µ�λ��
void BattleMapLayer::moveToCenter(Vec2 pos, float delta)
{
    Size WIN_SIZE = Director::getInstance()->getWinSize();
    
    float scale = this->getScale();
    pos = Vec2(pos.x/scale, pos.y/scale);
    Vec2 worldPos = this->convertToWorldSpace(pos);
    
    Vec2 diff = Vec2(WIN_SIZE.width/2 - worldPos.x, (WIN_SIZE.height/2) - worldPos.y);
    
    auto scaleTo = ScaleTo::create(delta, 1.0);
    auto moveBy = MoveBy::create(delta, diff);
    this->runAction(scaleTo);
    this->runAction(moveBy);
}


// ����ʱ����
void BattleMapLayer::enterAction()
{
    Size WIN_SIZE = Director::getInstance()->getWinSize();
    
    Vec2 pos = Vec2((WIN_SIZE.width - _mapSize.width)/2, (WIN_SIZE.height - _mapSize.height)/2);
    this->setPosition(pos);
    
    moveToCenter(Vec2(_mapSize.width/2, _mapSize.height/2), 0.5);
}

