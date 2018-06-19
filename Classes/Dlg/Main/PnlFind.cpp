
#include "Utils/PublicDefault.h"
#include "PnlFind.h"


#include "Utils/ConfigMgr.h"
#include "Utils/UIUtils.h"
#include "Dlg/Main/DlgMain.h"

USING_NS_CC;


PnlFind::PnlFind()
	:pnl_reward_container(nullptr)
	, pnl_gold_find(nullptr)
	, pnl_diamond_find(nullptr)
	, findCount(0)
{
	memset(pnl_items, 0, 25 * sizeof(Layout*));
}

PnlFind::~PnlFind()
{

}

bool PnlFind::init(DlgBase* dlg)
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

void PnlFind::load()
{
	char str[255] = "\0";

	auto lay_root = GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/pnl_find.json");
	this->addChild(lay_root);

	this->pnl_reward_container = (Layout*)Helper::seekWidgetByName(lay_root, "pnl_reward_container");
	this->pnl_get = (Layout*)Helper::seekWidgetByName(lay_root, "pnl_get");
	this->pnl_get->setVisible(false);
	Layout* pnl_btn_ok = (Layout*)Helper::seekWidgetByName(this->pnl_get, "pnl_btn_ok");
	pnl_btn_ok->addTouchEventListener(CC_CALLBACK_2(PnlFind::onClose, this));

	txt_tip1 = Text::create("恭喜主公,发现", FONT_ARIAL, 36);
	txt_tip1->setAnchorPoint(Vec2(0, 1));
	txt_tip1->setPosition(Vec2(120, 700));
	this->pnl_get->addChild(txt_tip1);

	txt_tip2 = Text::create("xxxx", FONT_ARIAL, 36);
	txt_tip2->setAnchorPoint(Vec2(0, 1));
	txt_tip2->setPosition(Vec2(120, 700));
	this->pnl_get->addChild(txt_tip2);

	txt_tip3 = Text::create("将军,", FONT_ARIAL, 36);
	txt_tip3->setAnchorPoint(Vec2(0, 1));
	txt_tip3->setPosition(Vec2(120, 700));
	this->pnl_get->addChild(txt_tip3);

	txt_tip4 = Text::create("我军实力大增!", FONT_ARIAL, 36);
	txt_tip4->setAnchorPoint(Vec2(0, 1));
	txt_tip4->setPosition(Vec2(120, 660));
	this->pnl_get->addChild(txt_tip4);

	this->img_half = (ImageView*)Helper::seekWidgetByName(this->pnl_get, "img_half");
	this->txt_name = (Text*)Helper::seekWidgetByName(this->pnl_get, "txt_name");

	this->pnl_gold_find = (Layout*)Helper::seekWidgetByName(lay_root, "pnl_gold_find");
	this->pnl_gold_find->addTouchEventListener(CC_CALLBACK_2(PnlFind::onGoldFind, this));

	this->pnl_diamond_find = (Layout*)Helper::seekWidgetByName(lay_root, "pnl_diamond_find");
	this->pnl_diamond_find->addTouchEventListener(CC_CALLBACK_2(PnlFind::onDiamondFind, this));

	for (int i = 1; i < 26; ++i) {
		sprintf(str, "pnl_item_%d", i);
		this->pnl_items[i] = (Layout*)Helper::seekWidgetByName(this->pnl_reward_container, str);
		this->pnl_items[i]->setVisible(false);
	}

	srand(time(NULL));
}

void PnlFind::setVisible(bool b) {
	Node::setVisible(b);


}

void PnlFind::onClose(Ref* sender, Widget::TouchEventType type) {
	this->pnl_get->setVisible(false);
}

void PnlFind::updateUI()
{

}

void PnlFind::updateGetPanel(int id) {
	char str[256] = "\0";
	ValueMap& myObj = *(DBM()->getMyObjById(id));

	//基本信息
	ValueMap& objBaseCfg = *(CFG()->getObjInfoById(id));
	this->txt_name->setString(objBaseCfg["Name"].asString());
	setTextColor(this->txt_name, objBaseCfg["Quality"].asInt());

	//this->img_half->loadTexture(objBaseCfg["Icon"].asString());
	this->img_half->setVisible(false);
	auto lay_root = this->img_half->getParent();
	//动画
	string animaName = objBaseCfg["Anima"].asString();
	auto arm = lay_root->getChildByTag(999);
	if (!arm || arm->getName() != animaName) {
		if (arm) {
			arm->removeFromParent();
		}

		Armature* _arm = Armature::create(animaName);
		_arm->getAnimation()->play("atk0");
		_arm->setPosition(this->img_half->getPosition());
		_arm->setTag(999);
		_arm->setScale(2.4);
		_arm->getAnimation()->setMovementEventCallFunc(this, SEL_MovementEventCallFunc(&PnlFind::atk));
		lay_root->addChild(_arm);
	}

	sprintf(str, "%s", objBaseCfg["Name"].asString().c_str());
	txt_tip2->setString(str);
	setTextColor(txt_tip2, objBaseCfg["Quality"].asInt());
	txt_tip2->setPositionX(txt_tip1->getPositionX() + txt_tip1->getContentSize().width);

	txt_tip3->setPositionX(txt_tip2->getPositionX() + txt_tip2->getContentSize().width);
}

void PnlFind::atk(Armature* arm, MovementEventType eventType, const std::string& str) {

	if (eventType == LOOP_COMPLETE) {

		int x = arm->getAnimation()->getCurrentMovementID().find("atk");
		if (x >= 0) {
			arm->getAnimation()->play("idle0");
		}
	}
}


void PnlFind::onGoldFind(Ref* sender, Widget::TouchEventType type) {
	if (type != Widget::TouchEventType::ENDED) {
		return;
	}

	if (this->findCount == 25) {
		this->findCount = 0;
		for (int i = 1; i < 26; ++i) {
			this->pnl_items[i]->setVisible(false);
		}
	}

	++this->findCount;

	int num = rand() % 5;
	CCLOG("random num : %d", num);
	switch (num)
	{
	case 0:
	case 1:
	case 2:
	{
		//装备
		map<int, ValueMap>* table = CFG()->loadConfig("Config/EquipInfo.csv", "ID");
		int num = rand() % table->size() + 1;

		ValueMap* cfgPtr = CFG()->getEquipInfoById(num);
		ValueMap& cfg = *cfgPtr;
		if (cfgPtr) {
			DBM()->insertMyEquip(cfg["ID"].asInt());
			this->setEquipItemInfo(this->findCount, cfgPtr);
		}
	}
		break;
	case 3:
	{
		//武将
		map<int, ValueMap>* table = CFG()->loadConfig("Config/ObjInfo.csv", "ID");

		while (true)
		{
			int num = rand() % table->size() + 1;

			ValueMap* cfgPtr = CFG()->getObjInfoById(num);
			ValueMap& cfg = *cfgPtr;
			if (cfg["Quality"].asInt() >5) {
				continue;
			}

			if (cfgPtr) {
				DBM()->insertMyObj(cfg["ID"].asInt(), 1);
				this->setHeroItemInfo(this->findCount, cfgPtr);
				if (cfg["ObjType"].asInt() == 2) {
					this->pnl_get->setVisible(true);
					this->updateGetPanel(cfg["ID"].asInt());
				}
				break;
			}
		}

	}
		break;
	case 4:
	case 5:
	case 6:
	case 7:
	{
		//金币
		int num = rand() % 200 + 1;
		setGoldItemInfo(this->findCount, num);
	}
		break;
	case 8:
	case 9:
	{
		//钻石
		int num = rand() % 20 + 1;
		setDiamondItemInfo(this->findCount, num);
	}
		break;
	default:
		break;
	}


}

void PnlFind::onDiamondFind(Ref* sender, Widget::TouchEventType type) {
	if (type != Widget::TouchEventType::ENDED) {
		return;
	}

	if (this->findCount == 25) {
		this->findCount = 0;
		for (int i = 1; i < 26; ++i) {
			this->pnl_items[i]->setVisible(false);
		}
	}

	++this->findCount;

	int num = rand() % 10;
	CCLOG("random num : %d", num);
	switch (num)
	{
	case 0:
	case 1:
	case 2:
	{
		//装备
		map<int, ValueMap>* table = CFG()->loadConfig("Config/EquipInfo.csv", "ID");
		int num = rand() % table->size() + 1;

		ValueMap* cfgPtr = CFG()->getEquipInfoById(num);
		ValueMap& cfg = *cfgPtr;
		if (cfgPtr) {
			DBM()->insertMyEquip(cfg["ID"].asInt());
			this->setEquipItemInfo(this->findCount, cfgPtr);
		}
	}
		break;
	case 3:
	case 4:
	case 5:
	case 6:
	{
		//武将
		map<int, ValueMap>* table = CFG()->loadConfig("Config/ObjInfo.csv", "ID");


		int num = rand() % table->size() + 1;

		ValueMap* cfgPtr = CFG()->getObjInfoById(num);
		ValueMap& cfg = *cfgPtr;


		if (cfgPtr) {
			DBM()->insertMyObj(cfg["ID"].asInt(), 1);
			this->setHeroItemInfo(this->findCount, cfgPtr);
			if (cfg["ObjType"].asInt() == 2) {
				this->pnl_get->setVisible(true);
				this->updateGetPanel(cfg["ID"].asInt());
			}
			break;
		}


	}
		break;
	case 7:
	{
		//金币
		int num = rand() % 200 + 1;
		setGoldItemInfo(this->findCount, num);
	}
		break;
	case 8:
	case 9:
	{
		//钻石
		int num = rand() % 20 + 1;
		setDiamondItemInfo(this->findCount, num);
	}
		break;
	default:
		break;
	}
}

void PnlFind::setEquipItemInfo(int index, ValueMap* equipCfg) {
	this->pnl_items[index]->setVisible(true);

	auto& cfg = *equipCfg;
	ImageView* img_icon = (ImageView*)Helper::seekWidgetByName(this->pnl_items[index], "img_icon");
	Text* txt_name = (Text*)Helper::seekWidgetByName(this->pnl_items[index], "txt_name");

	img_icon->loadTexture(cfg["Icon"].asString());
	txt_name->setString(cfg["Name"].asString());
	setTextColor(txt_name, cfg["Quality"].asInt());
}

void PnlFind::setSoilderItemInfo(int index, ValueMap* equipCfg) {
	this->pnl_items[index]->setVisible(true);

	auto& cfg = *equipCfg;
	ImageView* img_icon = (ImageView*)Helper::seekWidgetByName(this->pnl_items[index], "img_icon");
	Text* txt_name = (Text*)Helper::seekWidgetByName(this->pnl_items[index], "txt_name");

	img_icon->loadTexture(cfg["Item"].asString());
	txt_name->setString(cfg["Name"].asString());
	setTextColor(txt_name, cfg["Quality"].asInt());
}

void PnlFind::setHeroItemInfo(int index, ValueMap* equipCfg) {
	this->pnl_items[index]->setVisible(true);

	auto& cfg = *equipCfg;
	ImageView* img_icon = (ImageView*)Helper::seekWidgetByName(this->pnl_items[index], "img_icon");
	Text* txt_name = (Text*)Helper::seekWidgetByName(this->pnl_items[index], "txt_name");

	img_icon->loadTexture(cfg["Item"].asString());
	txt_name->setString(cfg["Name"].asString());
	setTextColor(txt_name, cfg["Quality"].asInt());
}

void PnlFind::setGoldItemInfo(int index, int goldNum) {
	this->pnl_items[index]->setVisible(true);

	ImageView* img_icon = (ImageView*)Helper::seekWidgetByName(this->pnl_items[index], "img_icon");
	Text* txt_name = (Text*)Helper::seekWidgetByName(this->pnl_items[index], "txt_name");

	img_icon->loadTexture("images/item/gold.png");
	char str[200] = "\0";
	sprintf(str, "元宝:%d", goldNum);

	txt_name->setString(str);
}

void PnlFind::setDiamondItemInfo(int index, int diamondNum) {
	this->pnl_items[index]->setVisible(true);

	ImageView* img_icon = (ImageView*)Helper::seekWidgetByName(this->pnl_items[index], "img_icon");
	Text* txt_name = (Text*)Helper::seekWidgetByName(this->pnl_items[index], "txt_name");

	img_icon->loadTexture("images/item/diamond.png");
	char str[200] = "\0";
	sprintf(str, "钻石:%d", diamondNum);
	txt_name->setString(str);
}

