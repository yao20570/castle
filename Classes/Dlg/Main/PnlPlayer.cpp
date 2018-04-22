
#include "Utils/PublicDefault.h"
#include "PnlPlayer.h"


#include "Utils/ConfigMgr.h"
#include "Dlg/Main/DlgMain.h"

USING_NS_CC;



PnlPlayer::PnlPlayer()	
{
	memset(this->pnl_part, 0, 6 * sizeof(Layout*));
}

PnlPlayer::~PnlPlayer()
{

}

bool PnlPlayer::init(DlgBase* dlg)
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

void PnlPlayer::load()
{
	auto lay_root = GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/pnl_player.json");
	this->addChild(lay_root);

	Layout* pnl_close = (Layout*)Helper::seekWidgetByName(lay_root, "pnl_close");
	pnl_close->addTouchEventListener(CC_CALLBACK_2(PnlPlayer::onCloseUI, this));

	this->sv_equip = (ScrollView*)Helper::seekWidgetByName(lay_root, "sv_equip");

	for (int i = 0; i < 6; ++i) {
		char str[256] = "\0";
		sprintf(str, "pnl_equip%d", i + 1);
		Layout* p = (Layout*)Helper::seekWidgetByName(lay_root, str);
		this->pnl_part[i] = (Layout*)GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/item_obj.json");
		this->pnl_part[i]->setVisible(false);

		Layout* pnl_select = (Layout*)Helper::seekWidgetByName(this->pnl_part[i], "pnl_select");
		pnl_select->setVisible(false);

		p->addChild(this->pnl_part[i]);

	}
}

void PnlPlayer::updateUI()
{
	this->updateSelectList();
	this->updateSettingPanel();
}

void PnlPlayer::updateSelectList() {
	int i = 0;
	ValueVector myEquips = DBM()->getMyEquips();
	for (auto& it : myEquips) {
		//数据
		auto& row = it.asValueMap();
		auto& cfg = *(CFG()->getEquipInfoById(row["EquipId"].asInt()));

		Layout* lay_root = (Layout*)this->sv_equip->getChildByTag(i);
		if (lay_root == nullptr) {
			//添加item
			lay_root = (Layout*)GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/item_obj.json");
			lay_root->setSwallowTouches(false);
			lay_root->setPositionX(i * 150);
			lay_root->setTag(i);
			lay_root->setUserData((void*)(row["ID"].asInt()));
			lay_root->addTouchEventListener(CC_CALLBACK_2(PnlPlayer::onSelectEquip, this));
			this->sv_equip->addChild(lay_root);
		}

		ImageView* img_icon = (ImageView*)Helper::seekWidgetByName(lay_root, "img_icon");
		Text* txt_name = (Text*)Helper::seekWidgetByName(lay_root, "txt_name");
		Layout* pnl_select = (Layout*)Helper::seekWidgetByName(lay_root, "pnl_select");

		bool b = row["IsWear"].asBool();
		pnl_select->setVisible(b);

		img_icon->loadTexture(cfg["Icon"].asString());
		txt_name->setString(cfg["Name"].asString());

		++i;
	}

	this->sv_equip->setInnerContainerSize(Size(i * 150, 150));
}


void PnlPlayer::updateSettingPanel() {

	for (int i = 0; i < 6; ++i) {		
		this->pnl_part[i]->setVisible(false);
		this->pnl_part[i]->setUserData(nullptr);
	}

	ValueVector myEquips = DBM()->getMyEquips();
	for (auto& it : myEquips) {
		//数据
		auto& row = it.asValueMap();
		bool isWear = row["IsWear"].asBool();
		int id = row["ID"].asInt();
		if (!isWear) {
			continue;
		}

		auto& cfg = *(CFG()->getEquipInfoById(row["EquipId"].asInt()));
		int part = cfg["Part"].asInt();
		Layout* pnlPart = this->pnl_part[part - 1];
		pnlPart->setVisible(true);
		pnlPart->setUserData((void*)id);
		
		ImageView* img_icon = (ImageView*)Helper::seekWidgetByName(pnlPart, "img_icon");
		Text* txt_name = (Text*)Helper::seekWidgetByName(pnlPart, "txt_name");
		
		CCLOG(cfg["Name"].asString().c_str());
		img_icon->loadTexture(cfg["Icon"].asString());
		txt_name->setString(cfg["Name"].asString());

	}
}


void PnlPlayer::onCloseUI(Ref* sender, Widget::TouchEventType type) {
	((DlgMain*)this->hostDlg)->showPanel(PanelType::Setting);
}

void PnlPlayer::onSelectEquip(Ref* sender, Widget::TouchEventType type) {
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		//获取装备数据id
		int id = (int)(((Layout*)sender)->getUserData());
		//获取装备数据
		ValueVector equips = DBM()->getMyEquipsById(id);
		ValueMap equip = equips[0].asValueMap();
		//是否已穿戴
		bool isWear = equip["IsWear"].asBool();

		
		if (!isWear) {
			//获取配置
			auto& cfg = *(CFG()->getEquipInfoById(equip["EquipId"].asInt()));
			//获取部位
			int part = cfg["Part"].asInt();
			Layout* pnlPart = this->pnl_part[part - 1];
			int preEquipId = (int)(pnlPart->getUserData());

			//御下前一件装备
			DBM()->updateMyEquip(preEquipId, false);
		}
		
		//穿上当前装备
		DBM()->updateMyEquip(id, !isWear);

		this->updateUI();

		break;
	}	
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
	
}