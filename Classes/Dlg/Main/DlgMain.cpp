#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocostudio;
using namespace cocos2d::ui;

#include "DlgMain.h"

USING_NS_CC;

DlgMain::DlgMain()
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
		CCLOG("%s","打开主公功能");
	}
}
//打开搜索功能
void DlgMain::onSearch(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {
		CCLOG("打开搜索功能");
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
		CCLOG("打开布阵功能");
	}
}
//打开关卡功能
void DlgMain::onChapter(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {
		CCLOG("打开关卡功能");
	}
}