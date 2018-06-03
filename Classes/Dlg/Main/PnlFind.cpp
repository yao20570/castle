
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
	char str[128] = "\0";
	ValueMap& myObj = *(DBM()->getMyObjById(id));
	
	//基本信息
	ValueMap& objBaseCfg = *(CFG()->getObjInfoById(id));
	this->img_half->loadTexture(objBaseCfg["Icon"].asString());
	this->txt_name->setString(objBaseCfg["Name"].asString());	
	setTextColor(this->txt_name, objBaseCfg["Quality"].asInt());
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
	case 1: 
	case 2:
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
	case 3:
	{
		//金币
		int num = rand() % 200 + 1;
		setGoldItemInfo(this->findCount, num);
	}
	break;
	case 4:
	{
		//钻石
		int num = rand() % 20 + 1;
		setDiamondItemInfo(this->findCount, num);
	}
	break;
	default:
		break;
	}

	//int num = rand() % 20;
	//int num = rand() % 20;

	//ValueMap* cfgPtr = CFG()->getEquipInfoById(num);
	//ValueMap& cfg = *cfgPtr;

	//if (cfgPtr) {
	//	char str[256] = "\0";
	//	sprintf(str, "搜索获得装备 - %s", cfg["Name"].asString().c_str());
	//	this->showTip(str);

	//	DBM()->insertMyEquip(cfg["ID"].asInt());
	//}
	//else {
	//	this->showTip("搜索失败");
	//}
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

	int num = rand() % 5;
	CCLOG("random num : %d", num);
	switch (num)
	{
	case 0:
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
	case 1:
	case 2:
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
	case 3:
	{
		//金币
		int num = rand() % 200 + 1;
		setGoldItemInfo(this->findCount, num);
	}
	break;
	case 4:
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

