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

//����������
void DlgFight::onLord(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {

	}
}
//����������
void DlgFight::onSearch(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {

	}
}
//��ս������
void DlgFight::onFight(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {

	}
}
//�򿪲�����
void DlgFight::onSetting(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {

	}
}
//�򿪹ؿ�����
void DlgFight::onChapter(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {

	}
}