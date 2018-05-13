#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocostudio;
using namespace cocos2d::ui;

#include "DlgMain.h"
#include "Utils/ConfigMgr.h"
#include "Core/BaseComponent.h"
#include "DLG/Main/PnlSetting.h"
#include "DLG/Main/PnlPlayer.h"
#include "DLG/Main/PnlHero.h"
#include "DLG/Main/PnlFind.h"

USING_NS_CC;




DlgMain::DlgMain()
	: _lay_mid(nullptr)
	, _pnl_setting(nullptr)
	, _pnls()
	, _cur_pnl(nullptr)
{
	_dlg_type = ENUM_DLG_TYPE::Full;
	_dlg_name = "DlgFight";
}

DlgMain::~DlgMain()
{

}

bool DlgMain::init(StateBase* gameState)
{
	if (this->_is_init == true) {
		return true;
	}
	this->_is_init = true;
	this->_game_state = gameState;
	//////////////////////////////
	// 1. super init first
	//if (!PanZoomLayer::init())
	if (!Layer::init())
	{
		return false;
	}

	this->load();
	return true;
}

void DlgMain::load()
{
	auto lay_root = GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/dlg_mian.json");
	CCLOG("=============>%p", lay_root);
	this->addChild(lay_root);
	this->setName("dlg_mian");


	//panel容器
	this->_lay_mid = (Layout*)Helper::seekWidgetByName(lay_root, "lay_mid");

	//主公
	auto btnLord = (Button*)Helper::seekWidgetByName(lay_root, "lay_btn_1");
	btnLord->addTouchEventListener(CC_CALLBACK_2(DlgMain::onLord, this));

	//查找
	auto btnSearch = (Button*)Helper::seekWidgetByName(lay_root, "lay_btn_2");
	btnSearch->addTouchEventListener(CC_CALLBACK_2(DlgMain::onSearch, this));

	//战斗
	auto btnFight = (Button*)Helper::seekWidgetByName(lay_root, "lay_btn_3");
	btnFight->addTouchEventListener(CC_CALLBACK_2(DlgMain::onFight, this));

	//布阵
	auto btnSetting = (Button*)Helper::seekWidgetByName(lay_root, "lay_btn_4");
	btnSetting->addTouchEventListener(CC_CALLBACK_2(DlgMain::onSetting, this));

	//关卡
	auto btnChapter = (Button*)Helper::seekWidgetByName(lay_root, "lay_btn_5");
	btnChapter->addTouchEventListener(CC_CALLBACK_2(DlgMain::onChapter, this));

}

void DlgMain::showPanel(PanelType type) {

	BaseComponent* comp = this->getPanel(type);

	//同一个面板，则返回
	if (comp == this->_cur_pnl) {
		return;
	}

	//已有显示的面板，则隐藏
	if (this->_cur_pnl != nullptr) {
		this->_cur_pnl->setVisible(false);
	}

	//设置当前面板并显示更新
	this->_cur_pnl = comp;
	this->_cur_pnl->setVisible(true);
	this->_cur_pnl->updateUI();
}

BaseComponent* DlgMain::getPanel(PanelType type) {
	BaseComponent* comp = nullptr;
	auto it = this->_pnls.find(type);
	if (it == this->_pnls.end()) {
		switch (type)
		{
		case PanelType::Setting:
			comp = PnlSetting::create(this);
			break;
		case PanelType::Player:
			comp = PnlPlayer::create(this);
			break; 
		case PanelType::HeroPnl:
			comp = PnlHero::create(this);
			break;
		case PanelType::Find:
			comp = PnlFind::create(this);
			break;
		case PanelType::Chater:
			//comp = PnlHero::create(this);
			break;
		}
		_pnls[type] = comp;
		this->_lay_mid->addChild(comp);
	}
	else {
		comp = it->second;
	}

	return comp;
}

void DlgMain::showDlg(const string& dlgName)
{
	DlgBase::showDlg(dlgName);
}

void DlgMain::hideDlg(const string& dlgName)
{
	DlgBase::hideDlg(dlgName);
}

//打开主公功能
void DlgMain::onLord(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {
		this->showPanel(PanelType::HeroPnl);
	}
}
//打开搜索功能
void DlgMain::onSearch(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {
		CCLOG("打开搜索功能");
		
		
		this->showPanel(PanelType::Find);

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
}
//打开战斗功能
void DlgMain::onFight(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {
		CCLOG("打开战斗功能");
		showDlg("DlgFight");
	}
}
//打开布阵功能
void DlgMain::onSetting(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {
		this->showPanel(PanelType::Setting);
	}
}
//打开关卡功能
void DlgMain::onChapter(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {
		CCLOG("打开关卡功能");
	}
}