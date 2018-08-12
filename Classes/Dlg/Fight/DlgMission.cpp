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
	: DlgFight()
	, _select_obj(nullptr)
	, _round(1)
	, _missionSubId(0)
{
	_dlg_type = ENUM_DLG_TYPE::Full;
	_dlg_name = "DlgMission";
	_state = MapState::Fight;
}

DlgMission::~DlgMission()
{
	
}

bool DlgMission::init(StateBase* gameState)
{

	//////////////////////////////
	// 1. super init first
	//if (!PanZoomLayer::init())
	if (!DlgFight::init(gameState))
	{
		return false;
	}

	
	return true;
}
void DlgMission::load()
{
	auto lay_root = GUIReader::getInstance()->widgetFromJsonFile("UI/DlgFight/dlg_battle.json");
	this->addChild(lay_root);


	_lay_floor = (Layout*)Helper::seekWidgetByName(lay_root, "lay_floor");
	_lay_sky = (Layout*)Helper::seekWidgetByName(lay_root, "lay_sky");
	_lay_fight = (Layout*)Helper::seekWidgetByName(lay_root, "lay_fight");
	_lay_fight->addTouchEventListener(CC_CALLBACK_2(DlgMission::onMapTouch, this));

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
				addHero(pos, camp, objInfo);
				break;
			case 1:
				//士兵
				addSoilder(pos, camp, objInfo);
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
				addHero(pos, camp, objInfo);
				break;
			case 1:
				//士兵
				addSoilder(pos, camp, objInfo);
				break;
			case 3:
				break;
			}
		}
		//addPlayer(Vec2(320, 940), camp);
	}

	this->triggerSkill(SkillTriggerType::RoundStart);
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