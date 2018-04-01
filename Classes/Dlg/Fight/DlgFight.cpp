#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocostudio;
using namespace cocos2d::ui;


#include "DlgFight.h"
#include "AIMgr.h"
#include "Utils/DataManager.h"
#include "Utils/DBManager.h"
#include "Utils/GlobalValue.h"
#include "Utils/ConfigMgr.h"
#include "Model/Hero.h"
#include "Model/Soilder.h"

USING_NS_CC;


//static const char* PathChaterCfg = "Config/Chater.csv";
//static const char* PathFormatChaterObjCfg = "Config/Chater/%s.csv";
static const char* PathObjPosition	= "Config/ObjPosition.csv";
static const char* PathHeroInfo		= "Config/HeroInfo.csv";
static const char* PathSoilderInfo	= "Config/SoilderInfo.csv";

DlgFight::DlgFight()	
	:_objPosCfg(nullptr)
	, _map(nullptr)
{
	_dlg_type = ENUM_DLG_TYPE::Full;
	_dlg_name = "DlgFight";
}

DlgFight::~DlgFight()
{
	CC_SAFE_DELETE(this->_ai);
}

bool DlgFight::init(StateBase* gameState)
{
	if (this->_is_init == true) {
		return true;
	}
	this->_is_init = true;
	this->_game_state = gameState;
	this->_ai = new AIMgr();
	this->addTouch();
	//////////////////////////////
	// 1. super init first
	//if (!PanZoomLayer::init())
	if (!Layer::init())
	{
		return false;
	}
	schedule(schedule_selector(DlgFight::update), 0.81f);
	this->load();
	return true;
}

void DlgFight::update(float dt) {
	_ai->update(dt);
}

void DlgFight::load()
{
	auto lay_root = GUIReader::getInstance()->widgetFromJsonFile("UI/DlgFight/dlg_fight.json");
	this->addChild(lay_root);


	_map = (Layout*)lay_root->getChildByName("lay_fight");
	
	////获取所有剧情
	//map<int, ValueMap>* chaterCfg = DM()->loadCsvData(PathChaterCfg, "ID");

	////当前剧情
	//auto it = chaterCfg->find(CUR_CHATER_ID);
	//if (it == chaterCfg->end()) {
	//	//没有剧情则关闭
	//	hideDlg(_dlg_name);
	//	assert(false, "找不到剧情配置");
	//	return;
	//}
	//ValueMap pCurChater = it->second;

	////剧情对应位置的配置文件
	//char strPathOjbCfg[100];
	//sprintf(strPathOjbCfg, PathFormatChaterObjCfg, pCurChater["DetailKey"].asString());
	//map<int, ValueMap>* objPosCfg = DM()->loadCsvData(PathObjPosition, "ID");
	
	setObjPosition();

	//关卡
	auto btnChapter = (Button*)Helper::seekWidgetByName(lay_root, "lay_btn_5");
	btnChapter->addTouchEventListener(CC_CALLBACK_2(DlgFight::onClose, this));
}

void DlgFight::addTouch()
{
	auto dispatcher = this->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(DlgFight::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(DlgFight::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(DlgFight::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

bool DlgFight::onTouchBegan(Touch* pTouch, Event* pEvent) 
{
	auto selectObj = _ai->getSelectObj();
	if (selectObj == nullptr) {
		return false;
	}

	auto posInMap = _map->convertToNodeSpace(pTouch->getLocation());
	int radius = selectObj->_radius;
	auto mapSize = _map->getContentSize();
	Rect rect(radius / 2, radius / 2, mapSize.width - radius, mapSize.height/2 - radius);
	if (rect.containsPoint(posInMap) == false) {
		return false;
	}
	
	for (auto it : _ai->_soildersSelf) 
	{
		if (it == selectObj) {
			continue;
		}		
		if (GM()->isPointInCircle(it->getPosition(), it->_radius + selectObj->_radius, posInMap)) {
			return false;
		}
		else {
			selectObj->setPosition(posInMap);
		}
	}
	
	return false;
}

void DlgFight::onTouchMoved(Touch* pTouch, Event* pEvent) {
	int i = 2;
}

void DlgFight::onTouchEnded(Touch* pTouch, Event* pEvent) {
	int i = 3;
}

void DlgFight::setObjPosition()
{
	_objPosCfg = CFG()->getObjPos();
	for (auto it : *_objPosCfg) {
		ValueMap& row = it.second;
		Vec2 pos(row["PositionX"].asInt(), row["PositionY"].asInt());
		int objId = row["ObjectId"].asInt();
		int camp = row["Camp"].asInt();

		int objType = row["ObjectType"].asInt();
		switch (objType)
		{
		case 1:
			//武将			
			addHero(objId, pos, camp);
			break;
		case 2:
			//士兵
			addSoilder(objId, pos, camp);
			break;
		case 3:
			//建筑/障碍等
			break;
		}
	}
}

void DlgFight::addHero(int heroId, Vec2 pos, int camp)
{
	Hero* hero = Hero::create(heroId, _ai, camp);
	hero->setPosition(pos);
	this->_map->addChild(hero);
}

void DlgFight::addSoilder(int soilderId, Vec2 pos, int camp)
{
	Soilder* soilder = Soilder::create(soilderId, _ai, camp);
	soilder->setPosition(pos);
	this->_map->addChild(soilder);
}

void DlgFight::showDlg(const string& dlgName)
{
	DlgBase::showDlg(dlgName);
}

void DlgFight::hideDlg(const string& dlgName)
{
	DlgBase::hideDlg(dlgName);
}

void DlgFight::onClose(Ref* sender, Widget::TouchEventType type)
{
	hideDlg(this->getDlgName());
}

void DlgFight::onResetObjPos(Ref* sender, Widget::TouchEventType type)
{
	//
	int i = 1;
}