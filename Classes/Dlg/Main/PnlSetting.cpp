#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocostudio;
using namespace cocos2d::ui;

#include "PnlSetting.h"

USING_NS_CC;

PnlSetting::PnlSetting()
{
}

PnlSetting::~PnlSetting()
{

}

bool PnlSetting::init()
{
	//////////////////////////////
	// 1. super init first
	//if (!PanZoomLayer::init())
	if (!BaseComponent::init())
	{
		return false;
	}

	this->load();
	return true;
}

void PnlSetting::load()
{
	auto lay_root = GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/pnl_setting.json");
	this->addChild(lay_root);
	this->setName("pnl_setting");
}
