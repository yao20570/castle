
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocostudio;
using namespace cocos2d::ui;

#include "StateBase.h"
#include "StateEnum.h"

#include "Utils/ClassFactory.h"
#include "Dlg/DlgBase.h"


USING_NS_CC;


StateBase::StateBase()
	:_is_init(false)
	, _cur_dlg(nullptr)
{
	_open_dlgs = vector<string>();
	_layers = vector<Layer*>();
	_class_factory = new ClassFactory();
}

StateBase::~StateBase()
{
	CC_SAFE_DELETE(_class_factory);
}

bool StateBase::init()
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

void StateBase::load()
{
	for (int i = 1; i < ENUM_STATE_LAYER::CONUT; ++i) {
		Layer* lay = Layer::create();
		addChild(lay, i);
		_layers.push_back(lay);
	}
}

void StateBase::regDlg()
{
	//子类继承覆盖
	//_class_factory->regClass("DlgMain", new ClassCreator<DlgMain>);
	//_class_factory->regClass("DlgFight", new ClassCreator<DlgFight>);
}

DlgBase* StateBase::creataeDlg(const string& dlgName)
{
	return _class_factory->createClass(this, dlgName);
}

void StateBase::addDlg(DlgBase* dlg) {
	ENUM_DLG_TYPE dlgType = dlg->getDlgType();
	switch (dlgType)
	{
	case ENUM_DLG_TYPE::Full:
		this->_layers[ENUM_STATE_LAYER::DLG_FULL]->addChild(dlg);
		break;
	case ENUM_DLG_TYPE::Half:
		this->_layers[ENUM_STATE_LAYER::DLG_HALG]->addChild(dlg);
		break;
	default:
		break;
	}
}

void StateBase::showDlg(const string& dlgName)
{
	if (!_open_dlgs.empty()) {
		const string& lastDlgName = _open_dlgs.back();
		if (lastDlgName == dlgName) {
			//重复打开同一个窗口，则返回
			return;
		}
	}

	if (_cur_dlg != nullptr) {
		//旧窗口移除
		//_cur_dlg->retain();
		//_cur_dlg->autorelease();
		//_cur_dlg->removeFromParent();
		//_cur_dlg = nullptr;
	}

	//打开新窗口
	_open_dlgs.push_back(dlgName);
	_cur_dlg = creataeDlg(dlgName);
	addDlg(_cur_dlg);
}

void StateBase::hideDlg(const string& dlgName)
{
	if (_cur_dlg != nullptr) {
		//移除旧窗口
		_open_dlgs.pop_back();
		_cur_dlg->retain();
		_cur_dlg->autorelease();
		_cur_dlg->removeFromParent();
		_cur_dlg = nullptr;
	}

	if (!_open_dlgs.empty()) {
		//打开前一个窗口
		const string& lastDlgName = _open_dlgs.back();
		_cur_dlg = creataeDlg(lastDlgName);
		addDlg(_cur_dlg);
	}
}
