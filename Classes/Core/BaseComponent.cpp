

#include "BaseComponent.h"

#include "Core/DlgBase.h"

USING_NS_CC;


BaseComponent::BaseComponent()	
{
	Layer::Layer();
}

BaseComponent::~BaseComponent()
{

}

bool BaseComponent::init(DlgBase* dlg)
{
	this->hostDlg = dlg;
	//////////////////////////////
	// 1. super init first
	//if (!PanZoomLayer::init())
	if (!Layout::init())
	{
		return false;
	}

	this->load();
	return true;
}

DlgBase* BaseComponent::showDlg(const string& dlgName)
{
	return this->hostDlg->showDlg(dlgName);
}

void BaseComponent::hideDlg(const string& dlgName)
{
	this->hostDlg->hideDlg(dlgName);
}

void BaseComponent::load()
{
}

void BaseComponent::updateUI()
{
	
}

void BaseComponent::showTip(char* tip) {
	this->hostDlg->showTip(tip);
}
void BaseComponent::showMessage(char* message, std::function<void()>& sureCallback, std::function<void()>& cancelCallback) {
	this->hostDlg->showMessage(message, sureCallback, cancelCallback);
}