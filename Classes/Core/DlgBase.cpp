#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocostudio;
using namespace cocos2d::ui;

#include "DlgBase.h"


USING_NS_CC;

//static DlgBase* create(StateBase* gameState, ENUM_DLG_TYPE dlgType)
//{
//	DlgBase *pRet = new(std::nothrow) DlgBase(gameState, dlgType);
//	if (pRet && pRet->init())
//	{
//		pRet->autorelease();
//		return pRet;
//	}
//	else
//	{
//		delete pRet;
//		pRet = nullptr;
//		return nullptr;
//	}
//}

DlgBase::DlgBase()
	:_is_init(false)
	, _dlg_type(ENUM_DLG_TYPE::Full)
	, _dlg_name("DlgBase")
{
	Layer::Layer();
}

DlgBase::~DlgBase()
{

}

ENUM_DLG_TYPE DlgBase::getDlgType() {
	return _dlg_type;
}

string DlgBase::getDlgName() {
	return _dlg_name;
}

bool DlgBase::init(StateBase* gameState)
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

void DlgBase::load()
{
}

DlgBase* DlgBase::showDlg(const string& dlgName)
{
	return _game_state->showDlg(dlgName);
}

void DlgBase::hideDlg(const string& dlgName)
{
	_game_state->hideDlg(dlgName);
}

void DlgBase::showTip(char* tip) {
	_game_state->showTip(tip);
}

void DlgBase::showMessage(char* message, std::function<void()>& sureCallback, std::function<void()>& cancelCallback) {
	_game_state->showMessage(message, sureCallback, cancelCallback);
}
