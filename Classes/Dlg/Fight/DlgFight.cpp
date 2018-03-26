#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocostudio;
using namespace cocos2d::ui;

#include "DlgFight.h"


USING_NS_CC;

DlgFight::DlgFight()
	:_is_init(false)
{
}

DlgFight::~DlgFight()
{

}

bool DlgFight::init()
{
	if (this->_is_init == true) {
		return true;
	}
	this->_is_init = true;

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

void DlgFight::show()
{
	this->setVisible(true);
}

void DlgFight::hide()
{
	this->setVisible(false);
}

void foreach(Node* node)
{
	CCLOG(node->getName().c_str());
	for (const auto& child : node->getChildren())
	{
		foreach(child);
	}
}

void DlgFight::load()
{
	auto lay_root = GUIReader::getInstance()->widgetFromJsonFile("UI/Main/dlg_mian.json");
	this->addChild(lay_root);
	this->setName("dlg_mian");

	

}

//打开主公功能
void DlgFight::onLord(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {

	}
}
//打开搜索功能
void DlgFight::onSearch(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {

	}
}
//打开战斗功能
void DlgFight::onFight(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {

	}
}
//打开布阵功能
void DlgFight::onSetting(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {

	}
}
//打开关卡功能
void DlgFight::onChapter(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {

	}
}