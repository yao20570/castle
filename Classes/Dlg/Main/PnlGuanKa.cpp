
#include "Utils/PublicDefault.h"
#include "PnlGuanKa.h"


#include "Utils/ConfigMgr.h"
#include "Dlg/Main/DlgMain.h"
#include "Dlg/Fight/DlgMission.h"
#include "Dlg/Fight/AIMgr.h"

USING_NS_CC;



PnlGuanKa::PnlGuanKa()
	:sv_guanka(nullptr)
	, lay_select(nullptr)
	, layouts()
{
}

PnlGuanKa::~PnlGuanKa()
{

}

bool PnlGuanKa::init(DlgBase* dlg)
{
	//////////////////////////////
	// 1. super init first
	//if (!PanZoomLayer::init())
	if (!BaseComponent::init(dlg))
	{
		return false;
	}

	return true;
}

void PnlGuanKa::load()
{
	auto lay_root = GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/pnl_guanka.json");
	this->addChild(lay_root);

	this->sv_guanka = (ScrollView*)Helper::seekWidgetByName(lay_root, "sv_guanka");

	this->pnl_start = (ScrollView*)Helper::seekWidgetByName(lay_root, "pnl_start");
	this->pnl_start->addTouchEventListener(CC_CALLBACK_2(PnlGuanKa::onStart, this));

	ValueMap myMission = DBM()->getMyMission()[0].asValueMap();
	int missionMain = myMission["MissionMain"].asInt();
	int missionSub = myMission["MissionSub"].asInt();
	int temp = missionMain * 10 + missionSub;
	map<int, ValueMap>& mapMain = *(CFG()->loadConfig("Config/MissionMain.csv", "ID"));
	map<int, ValueMap>& mapSub = *(CFG()->loadConfig("Config/MissionSub.csv", "ID"));

	Size svSize = this->sv_guanka->getContentSize();

	

	this->sv_guanka->setInnerContainerSize(Size(svSize.width, 270 * mapMain.size()));
	Size innerSize = this->sv_guanka->getInnerContainerSize();
	for (auto& it : mapMain) {
		auto item_root = GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/pnl_guanka_item.json");
		Size itemSize = item_root->getContentSize();
		int mainId = it.second["ID"].asInt();
		item_root->setTag(mainId);
		item_root->setPosition(Vec2(0, innerSize.height - mainId * itemSize.height));
		this->sv_guanka->addChild(item_root);

		//主标题
		auto lab_mission_main = (Text*)Helper::seekWidgetByName(item_root, "lab_mission_main");
		lab_mission_main->setString(it.second["title"].asString());

		//子标题
		for (auto& subIt : mapSub) {
			auto subData = subIt.second;
			if (subData["mianId"].asInt() == mainId) {

				bool open = subData["open"].asBool();
				

				int index = subData["sort"].asInt();
				char str[128] = "\0";
				sprintf(str, "pnl_mission_sub%d", index);
				Layout* pnl_mission_sub = (Layout*)Helper::seekWidgetByName(item_root, str);
				if (open){
					pnl_mission_sub->addTouchEventListener(CC_CALLBACK_2(PnlGuanKa::onSelectMissionSub, this));
				}

				Text* lab_mission_sub = (Text*)Helper::seekWidgetByName(pnl_mission_sub, "lab_mission_sub");
				lab_mission_sub->setString(subData["title"].asString());
								
				int tag = mainId * 10 + index;
				pnl_mission_sub->setTag(tag);
				pnl_mission_sub->setUserData((void*)subData["ID"].asInt());

				if (tag == 11) {
					//pnl_mission_sub->setScale(1.2);
					lab_mission_sub->setTextColor(Color4B(255, 0, 0, 255));
					this->lay_select = pnl_mission_sub;
				}


				this->layouts.push_back(pnl_mission_sub);
			}
		}
	}

}

void PnlGuanKa::updateUI()
{
	ValueMap myMission = DBM()->getMyMission()[0].asValueMap();
	int missionMain = myMission["MissionMain"].asInt();
	int missionSub = myMission["MissionSub"].asInt();
	int temp = missionMain * 10 + missionSub;

	for (auto it : this->layouts) {
		int tag = it->getTag();

		bool isEnabled = (tag <= temp);

	}
}




void PnlGuanKa::onSelectMissionSub(Ref* sender, Widget::TouchEventType type) {
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		Layout* pnl = (Layout*)sender;
		ValueMap& subData = *((ValueMap*)pnl->getUserData());
		int mainId = pnl->getTag() / 10;
		int subId = pnl->getTag() % 10;


		ValueMap myMission = DBM()->getMyMission()[0].asValueMap();
		int missionMain = myMission["MissionMain"].asInt();
		int missionSub = myMission["MissionSub"].asInt();
		int temp = missionMain * 10 + missionSub;
		temp = temp == 0 ? 11 : temp;
		bool isEnabled = (pnl->getTag() <= temp);
		if (isEnabled == true) {

			Text* lab_mission_sub = (Text*)Helper::seekWidgetByName(this->lay_select, "lab_mission_sub");
			lab_mission_sub->setTextColor(Color4B(255, 255, 255, 255));

			this->lay_select = pnl;
			
			lab_mission_sub = (Text*)Helper::seekWidgetByName(this->lay_select, "lab_mission_sub");
			lab_mission_sub->setTextColor(Color4B(255, 0, 0, 255));
		}
		else{
			//this->showTip("剧情还没激活");
		}
		

		break;
	}
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}

}

void PnlGuanKa::onStart(Ref* sender, Widget::TouchEventType type) {
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		Layout* pnl = this->lay_select;
		int missionSubId = (int)pnl->getUserData();
		int tempKey = pnl->getTag();
		DlgMission* dlg = (DlgMission*)this->showDlg("DlgMission");
		dlg->setObjPosition(missionSubId, tempKey);
		dlg->_ai->start();
		break;
	}
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}