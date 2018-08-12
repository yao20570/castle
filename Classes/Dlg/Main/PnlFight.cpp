
#include "Utils/PublicDefault.h"
#include "PnlFight.h"


#include "Utils/ConfigMgr.h"
#include "Utils/UIUtils.h"
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
	if (b){
		this->updateSettingPanel();
	}
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
	for (cocos2d::Value& it : datas) {
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

		ImageView* img_icon = (ImageView*)settingObj->getChildByName("img_icon");
		img_icon->loadTexture(cfg["Head"].asString());
		img_icon->setVisible(false);

		Text* txt_name = (Text*)settingObj->getChildByName("txt_name");
		txt_name->setString(cfg["Name"].asString());
		setTextColorByQuality(txt_name, cfg["Quality"].asInt());

		string animaName = cfg["Anima"].asString();
		char str[128] = { 0 };
		sprintf(str, "animation/%s/%s.ExportJson", animaName.c_str(), animaName.c_str());
		ArmatureDataManager::getInstance()->addArmatureFileInfo(str);
		int _dir = GM()->getDir(Vec2(0, -1));
		Armature* _arm = Armature::create(animaName);
		_arm->getAnimation()->play("idle" + GM()->getIntToStr(_dir));
		_arm->setPosition(img_icon->getPosition());
		settingObj->addChild(_arm);

		int kind = cfg["Kind"].asInt();
		const string& strKind = CFG()->getWord(1 + kind);
		Text* txt_kind = Text::create(strKind, FONT_ARIAL, 18);
		txt_kind->setPosition(Vec2(_arm->getPositionX(), _arm->getContentSize().height * 2 / 3 ));
		settingObj->addChild(txt_kind);
		setTextColorByKind(txt_kind, cfg["Kind"].asInt());

		this->pnl_setting->addChild(settingObj);


		
	}
}