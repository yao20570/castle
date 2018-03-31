#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocostudio;
using namespace cocos2d::ui;

#include "DlgFight.h"

USING_NS_CC;

DlgFight::DlgFight()	
{
	_dlg_type = ENUM_DLG_TYPE::Full;
	_dlg_name = "DlgFight";
}

DlgFight::~DlgFight()
{

}

bool DlgFight::init(StateBase* gameState)
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

void DlgFight::load()
{
	auto lay_root = GUIReader::getInstance()->widgetFromJsonFile("UI/DlgFight/dlg_fight.json");
	this->addChild(lay_root);

}

void DlgFight::showDlg(const string& dlgName)
{
	DlgBase::showDlg(dlgName);
}

void DlgFight::hideDlg(const string& dlgName)
{
	DlgBase::hideDlg(dlgName);
}

