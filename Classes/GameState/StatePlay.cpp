
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocostudio;
using namespace cocos2d::ui;

#include "StatePlay.h"
#include "Dlg/Main/DlgMain.h"
#include "Dlg/Fight/DlgFight.h"

#include "Utils/ClassFactory.h"

USING_NS_CC;


StatePlay::StatePlay()
	:_is_init(false)
	,_cur_dlg(nullptr)
{
	_class_factory = new ClassFactory();
}

StatePlay::~StatePlay()
{

}

bool StatePlay::init()
{
	if (this->_is_init == true) {
		return true;
	}
	this->_is_init = true;

	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	regDlg();

	load();
	
	return true;
}

void StatePlay::load() 
{
	//this->_main_layer = DlgMain::create();
	//this->addChild(this->_main_layer, 1);

	showDlg("DlgMain");
}

void StatePlay::regDlg()
{
	_class_factory->regClass("DlgMain", new ClassCreator<DlgMain>);
	_class_factory->regClass("DlgFight", new ClassCreator<DlgFight>);
}

Node* StatePlay::creataeDlg(const string& dlgName)
{
	return _class_factory->createClass(dlgName);
}

void StatePlay::showDlg(const string& dlgName)
{
	if (!_open_dlgs.empty()) {
		const string& lastDlgName = _open_dlgs.back();
		if (lastDlgName == dlgName) {
			return;
		}
	}	
	
	if (_cur_dlg != nullptr) {
		_cur_dlg->removeFromParent();
	}

	_open_dlgs.push_back(dlgName);
	this->addChild(creataeDlg(dlgName));
}

void StatePlay::hideDlg()
{
	if (_cur_dlg != nullptr) {
		_open_dlgs.pop_back();
		_cur_dlg->removeFromParent();
	}

	const string& lastDlgName = _open_dlgs.back();
	this->addChild(creataeDlg(lastDlgName));
}


void StatePlay::show()
{
	this->setVisible(true);
}

void StatePlay::hide()
{
	this->setVisible(false);
}