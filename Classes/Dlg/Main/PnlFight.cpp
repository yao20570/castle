
#include "Utils/PublicDefault.h"
#include "PnlFight.h"


#include "Utils/ConfigMgr.h"
#include "Dlg/Main/DlgMain.h"

USING_NS_CC;



PnlFight::PnlFight()
	:pnl_setting(nullptr)
	, pnl_start(nullptr)
{
	
}

PnlFight::~PnlFight()
{

}

bool PnlFight::init(DlgBase* dlg)
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

void PnlFight::load()
{
	char str[255] = "\0";

	auto lay_root = GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/pnl_fight.json");
	this->addChild(lay_root);

	//设置面板
	this->pnl_setting = (Layout*)Helper::seekWidgetByName(lay_root, "pnl_setting");

	//开始按钮
	this->pnl_start = (Layout*)Helper::seekWidgetByName(lay_root, "pnl_start");
	this->pnl_start->addTouchEventListener(CC_CALLBACK_2(PnlFight::onStart, this));

	
}

void PnlFight::setVisible(bool b) {
	Node::setVisible(b);

	this->updateSettingPanel();
}

void PnlFight::onStart(Ref* sender, Widget::TouchEventType type) {
	showDlg("DlgFight");
}

void PnlFight::updateUI()
{

}

void PnlFight::updateSettingPanel() {
	this->pnl_setting->removeAllChildren();

	ValueVector datas = DBM()->getMySetting(0);
	for (Value& it : datas) {
		ValueMap& row = it.asValueMap();
		int x = row["x"].asInt();
		int y = row["y"].asInt();
		int objId = row["ObjId"].asInt();

		auto cfgPtr = CFG()->getObjInfoById(objId);
		auto& cfg = *cfgPtr;

		Layout* settingObj = (Layout*)GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/item_obj.json");
		settingObj->setSwallowTouches(false);
		settingObj->setAnchorPoint(Vec2(0.5, 0.5));
		settingObj->setPositionX(x);
		settingObj->setPositionY(y);
		settingObj->setLocalZOrder(100000000 - y * 10000 + x);
		settingObj->setTag(objId);
		settingObj->setUserData(cfgPtr);
		((Layout*)settingObj->getChildByName("pnl_select"))->setVisible(false);
		((ImageView*)settingObj->getChildByName("img_icon"))->loadTexture(cfg["Head"].asString());
		((Text*)settingObj->getChildByName("txt_name"))->setString(cfg["Name"].asString());
		this->pnl_setting->addChild(settingObj);
	}
}