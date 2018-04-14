
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocostudio;
using namespace cocos2d::ui;

#include "StatePlay.h"
#include "Core/StateEnum.h"
#include "Dlg/Main/DlgMain.h"
#include "Dlg/Fight/DlgFight.h"

#include "Utils/ClassFactory.h"


USING_NS_CC;


StatePlay::StatePlay()
{		
}

StatePlay::~StatePlay()
{
}

bool StatePlay::init()
{
	return StateBase::init();
}

void StatePlay::load()
{
	StateBase::load();

	showDlg("DlgMain");
}

void StatePlay::regDlg()
{
	_class_factory->regClass("DlgMain", new ClassCreator<DlgMain>());
	_class_factory->regClass("DlgFight", new ClassCreator<DlgFight>());
}

void StatePlay::showDlg(const string& dlgName)
{
	StateBase::showDlg(dlgName);
}

void StatePlay::hideDlg(const string& dlgName)
{
	StateBase::hideDlg(dlgName);
}

