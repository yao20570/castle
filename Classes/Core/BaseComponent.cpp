

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

void BaseComponent::load()
{
}

void BaseComponent::updateUI()
{
	
}
