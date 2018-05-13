
#include "Utils/PublicDefault.h"
#include "PnlHero.h"


#include "Utils/ConfigMgr.h"
#include "Dlg/Main/DlgMain.h"

USING_NS_CC;



PnlHero::PnlHero()
{

}

PnlHero::~PnlHero()
{

}

bool PnlHero::init(DlgBase* dlg)
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

void PnlHero::load()
{
	auto lay_root = GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/pnl_hero.json");
	this->addChild(lay_root);

	this->pnl_upgrade = (Layout*)Helper::seekWidgetByName(lay_root, "pnl_upgrade");
	this->pnl_upgrade->setVisible(false);

	this->pnl_btn_close = (Layout*)Helper::seekWidgetByName(lay_root, "pnl_btn_close");
	this->pnl_btn_close->addTouchEventListener(CC_CALLBACK_2(PnlHero::onCloseUpgradePanel, this));

	this->pnl_btn_upgrade = (Layout*)Helper::seekWidgetByName(lay_root, "pnl_btn_upgrade");
	this->pnl_btn_upgrade->addTouchEventListener(CC_CALLBACK_2(PnlHero::onUpgrade, this));

	this->pnl_btn_ok = (Layout*)Helper::seekWidgetByName(lay_root, "pnl_btn_ok");
	this->pnl_btn_ok->addTouchEventListener(CC_CALLBACK_2(PnlHero::onOk, this));

	this->pnl_btn_cancel = (Layout*)Helper::seekWidgetByName(lay_root, "pnl_btn_cancel");
	this->pnl_btn_cancel->addTouchEventListener(CC_CALLBACK_2(PnlHero::onCancel, this));

	this->sv_hero = (ScrollView*)Helper::seekWidgetByName(lay_root, "sv_hero");




	this->img_half = (ImageView*)Helper::seekWidgetByName(lay_root, "img_half");
	this->txt_level = (Text*)Helper::seekWidgetByName(lay_root, "txt_level");
	this->txt_name = (Text*)Helper::seekWidgetByName(lay_root, "txt_name");
	this->lab_attr_1 = (Text*)Helper::seekWidgetByName(lay_root, "lab_attr_1");
	this->lab_attr_2 = (Text*)Helper::seekWidgetByName(lay_root, "lab_attr_2");
	this->lab_attr_3 = (Text*)Helper::seekWidgetByName(lay_root, "lab_attr_3");
	this->lab_attr_4 = (Text*)Helper::seekWidgetByName(lay_root, "lab_attr_4");
	this->lab_attr_5 = (Text*)Helper::seekWidgetByName(lay_root, "lab_attr_5");
	this->lab_attr_6 = (Text*)Helper::seekWidgetByName(lay_root, "lab_attr_6");
	this->lab_attr_7 = (Text*)Helper::seekWidgetByName(lay_root, "lab_attr_7");
	this->lab_attr_8 = (Text*)Helper::seekWidgetByName(lay_root, "lab_attr_8");
	this->lab_attr_9 = (Text*)Helper::seekWidgetByName(lay_root, "lab_attr_9");
	this->lab_skill_1 = (Text*)Helper::seekWidgetByName(lay_root, "lab_skill_1");
	this->lab_skill_2 = (Text*)Helper::seekWidgetByName(lay_root, "lab_skill_2");
	this->lab_skill_3 = (Text*)Helper::seekWidgetByName(lay_root, "lab_skill_3");


	this->lab_attr_4_0 = (Text*)Helper::seekWidgetByName(lay_root, "lab_attr_4_0");
	this->lab_attr_5_0 = (Text*)Helper::seekWidgetByName(lay_root, "lab_attr_5_0");
	this->lab_attr_7_0 = (Text*)Helper::seekWidgetByName(lay_root, "lab_attr_7_0");
}

void PnlHero::updateUI()
{
	this->updateSvHero();
}

void PnlHero::updateSvHero() {
	map<int, ValueMap >& myObj = *(DBM()->getMyObj());
	int i = 0;
	int rowCount = ceil(myObj.size() / 3);
	Size svInnerSize(640, (200 + 10) * rowCount + 10);
	this->sv_hero->setInnerContainerSize(svInnerSize);
	svInnerSize = this->sv_hero->getInnerContainerSize();

	for (auto& it : myObj) {
		//数据
		auto& row = it.second;
		auto& cfg = *(CFG()->getObjInfoById(row["ID"].asInt()));


		Layout* lay_root = (Layout*)this->sv_hero->getChildByTag(i);
		if (lay_root == nullptr) {
			//添加item
			lay_root = (Layout*)GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/pnl_hero_item.json");
			lay_root->setSwallowTouches(false);
			lay_root->setAnchorPoint(Vec2(0, 1));
			lay_root->setPositionX(10 + 210 * (i % 3));
			lay_root->setPositionY(svInnerSize.height - 10 - (i / 3) * 210);
			lay_root->setTag(i);
			lay_root->setUserData((void*)(row["ID"].asInt()));
			lay_root->addTouchEventListener(CC_CALLBACK_2(PnlHero::onOpenUpgradePanel, this));
			this->sv_hero->addChild(lay_root);
		}

		ImageView* img_icon = (ImageView*)Helper::seekWidgetByName(lay_root, "img_icon");
		Text* txt_name = (Text*)Helper::seekWidgetByName(lay_root, "txt_name");

		Text* txt_lv = (Text*)Helper::seekWidgetByName(lay_root, "txt_lv");

		img_icon->loadTexture(cfg["Icon"].asString());
		txt_name->setString(cfg["Name"].asString());
		txt_lv->setString(string("Lv.") + row["Lv"].asString());
		++i;
	}

}


void PnlHero::updateUpgradePanel(int id, bool isNext) {

	pnl_btn_upgrade->setVisible(!isNext);
	pnl_btn_ok->setVisible(isNext);
	pnl_btn_cancel->setVisible(isNext);

	char str[128] = "\0";
	ValueMap& myObj = *(DBM()->getMyObjById(id));

	//基本信息
	ValueMap& objBaseCfg = *(CFG()->getObjInfoById(id));
	this->img_half->loadTexture(objBaseCfg["Icon"].asString());
	this->txt_name->setString(objBaseCfg["Name"].asString());
	this->txt_level->setString(string("Lv.") + myObj["Lv"].asString());


	//属性
	ValueMap* objAttrCfgPtr = nullptr;
	if (objBaseCfg["ObjType"].asInt() == 1) {
		//士兵
		objAttrCfgPtr = (CFG()->getSoilderInfoById(objBaseCfg["SubType"].asInt()));
		this->lab_attr_1->setVisible(false);
		this->lab_attr_2->setVisible(false);
		this->lab_attr_3->setVisible(false);
	}
	else {
		//武将
		objAttrCfgPtr = (CFG()->getHeroInfoById(objBaseCfg["SubType"].asInt()));
		this->lab_attr_1->setVisible(true);
		this->lab_attr_2->setVisible(true);
		this->lab_attr_3->setVisible(true);


		sprintf(str, "统御:%d", (*objAttrCfgPtr)["tongyu"].asInt());
		this->lab_attr_1->setString(str);
		sprintf(str, "武力:%d", (*objAttrCfgPtr)["wuli"].asInt());
		this->lab_attr_2->setString(str);
		sprintf(str, "智力:%d", (*objAttrCfgPtr)["zhili"].asInt());
		this->lab_attr_3->setString(str);
	}
	auto& objAttrCfg = *objAttrCfgPtr;

	//当前属性
	sprintf(str, "%d", objAttrCfg["HealthPoint"].asInt() + myObj["Lv"].asInt() * objAttrCfg["HealthPointUP"].asInt());
	this->lab_attr_4->setString(str);
	sprintf(str, "%d", objAttrCfg["Def"].asInt() + myObj["Lv"].asInt() * objAttrCfg["DefUP"].asInt());
	this->lab_attr_5->setString(str);
	sprintf(str, "%d", objAttrCfg["ShootRange"].asInt());
	this->lab_attr_6->setString(str);

	sprintf(str, "%d", objAttrCfg["Damage"].asInt() + myObj["Lv"].asInt() * objAttrCfg["DamageUP"].asInt());
	this->lab_attr_7->setString(str);
	sprintf(str, "%d", objAttrCfg["AttackSpeed"].asInt());
	this->lab_attr_8->setString(str);
	sprintf(str, "%d", objAttrCfg["Speed"].asInt());
	this->lab_attr_9->setString(str);

	sprintf(str, "%s", objAttrCfg["Skill1"].asString().c_str());
	this->lab_skill_1->setString(str);
	sprintf(str, "%s", objAttrCfg["Skill2"].asString().c_str());
	this->lab_skill_2->setString(str);
	sprintf(str, "%s", objAttrCfg["Skill3"].asString().c_str());
	this->lab_skill_3->setString(str);

	//升级增加属性
	this->lab_attr_4_0->setVisible(isNext);
	this->lab_attr_5_0->setVisible(isNext);
	this->lab_attr_7_0->setVisible(isNext);
	if (isNext) {
		sprintf(str, "+%d", objAttrCfg["HealthPointUP"].asInt());
		this->lab_attr_4_0->setPositionX(this->lab_attr_4->getPositionX() + this->lab_attr_4->getContentSize().width + 20);
		this->lab_attr_4_0->setString(str);

		sprintf(str, "+%d", objAttrCfg["DefUP"].asInt());
		this->lab_attr_5_0->setPositionX(this->lab_attr_5->getPositionX() + this->lab_attr_5->getContentSize().width + 20);
		this->lab_attr_5_0->setString(str);

		sprintf(str, "+%d", objAttrCfg["DamageUP"].asInt());
		this->lab_attr_7_0->setPositionX(this->lab_attr_7->getPositionX() + this->lab_attr_7->getContentSize().width + 20);
		this->lab_attr_7_0->setString(str);
	}


}


void PnlHero::onOpenUpgradePanel(Ref* sender, Widget::TouchEventType type) {

	if (type != Widget::TouchEventType::ENDED) {
		return;
	}
	
	this->pnl_upgrade->setVisible(true);

	this->obj_id = (int)((Layout*)sender)->getUserData();
	this->updateUpgradePanel(this->obj_id, false);
}

void PnlHero::onCloseUpgradePanel(Ref* sender, Widget::TouchEventType type) {
	if (type != Widget::TouchEventType::ENDED) {
		return;
	}

	this->pnl_upgrade->setVisible(false);

	this->updateSvHero();
}

void PnlHero::onUpgrade(Ref* sender, Widget::TouchEventType type) {

	if (type != Widget::TouchEventType::ENDED) {
		return;
	}

	this->pnl_btn_upgrade->setVisible(false);
	this->pnl_btn_ok->setVisible(true);
	this->pnl_btn_cancel->setVisible(true);

	this->updateUpgradePanel(this->obj_id, true);
}

void PnlHero::onOk(Ref* sender, Widget::TouchEventType type) {

	if (type != Widget::TouchEventType::ENDED) {
		return;
	}

	this->pnl_btn_upgrade->setVisible(true);
	this->pnl_btn_ok->setVisible(false);
	this->pnl_btn_cancel->setVisible(false);

	ValueMap& myObj = *(DBM()->getMyObjById(this->obj_id));
	DBM()->updatMyObj(this->obj_id, myObj["Lv"].asInt() + 1);
	this->updateUpgradePanel(this->obj_id, false);
}

void PnlHero::onCancel(Ref* sender, Widget::TouchEventType type) {

	if (type != Widget::TouchEventType::ENDED) {
		return;
	}

	this->pnl_btn_upgrade->setVisible(true);
	this->pnl_btn_ok->setVisible(false);
	this->pnl_btn_cancel->setVisible(false);

	this->updateUpgradePanel(this->obj_id, false);
}