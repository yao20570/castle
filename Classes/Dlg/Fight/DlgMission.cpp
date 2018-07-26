#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocostudio;
using namespace cocos2d::ui;


#include "DlgMission.h"
#include "AIMgr.h"
#include "Utils/DataManager.h"
#include "Utils/DBManager.h"
#include "Utils/GlobalValue.h"
#include "Utils/ConfigMgr.h"
#include "Model/Hero.h"
#include "Model/Soilder.h"
#include "Model/PlayerObj.h"
#include "Dlg/Main/DlgMain.h"

USING_NS_CC;


//static const char* PathChaterCfg = "Config/Chater.csv";
//static const char* PathFormatChaterObjCfg = "Config/Chater/%s.csv";
static const char* PathObjPosition = "Config/ObjPosition.csv";
static const char* PathHeroInfo = "Config/HeroInfo.csv";
static const char* PathSoilderInfo = "Config/SoilderInfo.csv";


DlgMission::DlgMission()
	:_objPosCfg(nullptr)
	, _map(nullptr)
	, _select_obj(nullptr)
	, _round(1)
	, _missionSubId(0)
{
	_dlg_type = ENUM_DLG_TYPE::Full;
	_dlg_name = "DlgMission";
}

DlgMission::~DlgMission()
{
	CC_SAFE_DELETE(this->_ai);
}

bool DlgMission::init(StateBase* gameState)
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
	schedule(schedule_selector(DlgMission::update));
	this->load();
	
	return true;
}
static int DlgMission_resule_delay = 60;
void DlgMission::update(float dt) {
	_ai->update(dt);
	
	if (_ai->isOver(2)) {

		//if (_round >= 0) {
			this->lay_result->setVisible(true);
			if (_ai->isWin(2)) {

				if (DlgMission_resule_delay == 60) {
					this->txt_result->setString("Win");

					ValueMap myMission = DBM()->getMyMission()[0].asValueMap();
					int missionMain = myMission["MissionMain"].asInt();
					int missionSub = myMission["MissionSub"].asInt();
					int tempKey = missionMain * 10 + missionSub;
					if (tempKey == this->_tempKey) {

						missionSub++;
						if (missionSub > 3) {
							missionMain++;
							missionSub = 1;
						}

						DBM()->updateMyMission(missionMain, missionSub);


					}
				}
				--DlgMission_resule_delay;
				
				if (DlgMission_resule_delay == 0) {
					DlgBase* dlgMain = showDlg("DlgMain");
					((DlgMain*)dlgMain)->showPanel(PanelType::Chater);
				}
				
				
			}
			else {
				this->txt_result->setString("lose");
			}
		//}
		//else {
		//	++_round;
		//	_ai->reset();
		//	setObjPosition(this->_missionSubId);
		//	_ai->start();
		//}
	}
	else {
		DlgMission_resule_delay = 60;
	}
}

void DlgMission::load()
{
	auto lay_root = GUIReader::getInstance()->widgetFromJsonFile("UI/DlgFight/dlg_battle.json");
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

	//setObjPosition();

	//开始
	auto btnStart = (Layout*)Helper::seekWidgetByName(lay_root, "lay_btn_1");
	btnStart->addTouchEventListener(CC_CALLBACK_2(DlgMission::onStart, this));
	//关闭
	auto btnClose = (Layout*)Helper::seekWidgetByName(lay_root, "lay_btn_5");
	btnClose->addTouchEventListener(CC_CALLBACK_2(DlgMission::onClose, this));
	//关闭
	this->lay_result = (Layout*)Helper::seekWidgetByName(lay_root, "lay_result");
	this->txt_result = (Text*)Helper::seekWidgetByName(lay_root, "txt_result");

	this->lay_result->setVisible(false);

}

void DlgMission::addTouch()
{
	auto dispatcher = this->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(DlgMission::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(DlgMission::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(DlgMission::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool DlgMission::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	if (_ai->getSelectObj() == nullptr) {
		return true;
	}

	//第一次选中
	if (_select_obj != _ai->getSelectObj()) {
		_select_obj = _ai->getSelectObj();
		return true;
	}

	//是否点在可布置范围
	auto posInMap = _map->convertToNodeSpace(pTouch->getLocation());
	int radius = _select_obj->_radius;
	auto mapSize = _map->getContentSize();
	Rect rect(radius / 2, radius / 2, mapSize.width - radius, mapSize.height / 2 - radius);
	if (rect.containsPoint(posInMap) == false) {
		return false;
	}

	//是否点在其它的对象上
	bool isCan = true;
	for (auto it : _ai->_objSelf)
	{
		if (it == _select_obj) {
			continue;
		}
		if (GM()->isPointInCircle(it->getPosition(), MAX(it->_radius, _select_obj->_radius), posInMap)) {
			isCan = false;
			break;
		}
	}

	_ai->setObjPos(_select_obj, posInMap);

	return true;
}

void DlgMission::onTouchMoved(Touch* pTouch, Event* pEvent) {
}

void DlgMission::onTouchEnded(Touch* pTouch, Event* pEvent) {
}

void DlgMission::setObjPosition(int missionSubId, int tempKey)
{
	this->_missionSubId = missionSubId;
	this->_tempKey = tempKey;

	ValueVector datas = DBM()->getMySetting(0);
	{
		//自己
		int camp = 1;
		for (cocos2d::Value& it : datas) {
			ValueMap& row = it.asValueMap();
			int x = row["x"].asInt();
			int y = row["y"].asInt();

			Vec2 pos(x, y);
			int objId = row["ObjId"].asInt();

			ValueMap* objInfo = CFG()->getObjInfoById(objId);
			int objType = (*objInfo)["ObjType"].asInt();
			int subType = (*objInfo)["SubType"].asInt();
			switch (objType)
			{
			case 2:
				//武将			
				addHero(subType, pos, camp);
				break;
			case 1:
				//士兵
				addSoilder(subType, pos, camp);
				break;
			case 3:
				//建筑/障碍等

				break;
			}
		}

		//addPlayer(Vec2(320, 40), camp);
	}

	{
		//敌人
		int camp = 2;



		map<int, ValueMap>& missionObjs = *(CFG()->loadConfig("Config/MissionObj.csv", "ID"));



		for (auto& it : missionObjs) {
			ValueMap& row = it.second;

			if (row["MissionSubId"].asInt() != missionSubId) {
				continue;
			}

			int x = row["x"].asInt();
			int y = row["y"].asInt();

			Vec2 pos(x, y);
			int objId = row["ObjId"].asInt();

			ValueMap* objInfo = CFG()->getObjInfoById(objId);
			int objType = (*objInfo)["ObjType"].asInt();
			int subType = (*objInfo)["SubType"].asInt();
			switch (objType)
			{
			case 2:
				//武将			
				addHero(subType, pos, camp);
				break;
			case 1:
				//士兵
				addSoilder(subType, pos, camp);
				break;
			case 3:
				break;
			}
		}
		//addPlayer(Vec2(320, 940), camp);
	}

}




static int i = 0;
void DlgMission::addHero(int heroId, Vec2 pos, int camp)
{
	Hero* hero = Hero::create(heroId, _ai, camp);
	this->_map->addChild(hero);
	_ai->setObjPos(hero, pos);

}

void DlgMission::addSoilder(int soilderId, Vec2 pos, int camp)
{
	Soilder* soilder = Soilder::create(soilderId, _ai, camp);
	this->_map->addChild(soilder);
	_ai->setObjPos(soilder, pos);
}

void DlgMission::addPlayer(Vec2 pos, int camp)
{
	PlayerObj* player = PlayerObj::create(1, _ai, camp);
	this->_map->addChild(player);
	_ai->setObjPos(player, pos);
}

DlgBase* DlgMission::showDlg(const string& dlgName)
{
	return DlgBase::showDlg(dlgName);
}

void DlgMission::hideDlg(const string& dlgName)
{
	DlgBase::hideDlg(dlgName);
	_ai->close();
}

void DlgMission::onStart(Ref* sender, Widget::TouchEventType type)
{
	_ai->start();
}

void DlgMission::onClose(Ref* sender, Widget::TouchEventType type)
{
	hideDlg(this->getDlgName());
}

void DlgMission::onResetObjPos(Ref* sender, Widget::TouchEventType type)
{
	//
	int i = 1;
}