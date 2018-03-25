#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocostudio;
using namespace cocos2d::ui;

#include "DlgMain.h"


USING_NS_CC;

DlgMain::DlgMain()
	:_is_init(false)
{
}

DlgMain::~DlgMain()
{

}

bool DlgMain::init()
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

void DlgMain::show()
{
	this->setVisible(true);
}

void DlgMain::hide()
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

void DlgMain::load()
{
	auto lay_root = GUIReader::getInstance()->widgetFromJsonFile("UI/dlg_mian.json");
	this->addChild(lay_root);
	this->setName("dlg_mian");

	//����
	auto btnLord = (Button*)Helper::seekWidgetByName(lay_root, "lay_btn_1");
	btnLord->addTouchEventListener(CC_CALLBACK_2(DlgMain::onLord, this));

	//����
	auto btnSearch = (Button*)Helper::seekWidgetByName(lay_root, "lay_btn_2");
	btnSearch->addTouchEventListener(CC_CALLBACK_2(DlgMain::onSearch, this));

	//ս��
	auto btnFight = (Button*)Helper::seekWidgetByName(lay_root, "lay_btn_3");
	btnFight->addTouchEventListener(CC_CALLBACK_2(DlgMain::onFight, this));

	//����
	auto btnSetting = (Button*)Helper::seekWidgetByName(lay_root, "lay_btn_4");
	btnSetting->addTouchEventListener(CC_CALLBACK_2(DlgMain::onSetting, this));

	//�ؿ�
	auto btnChapter = (Button*)Helper::seekWidgetByName(lay_root, "lay_btn_5");
	btnChapter->addTouchEventListener(CC_CALLBACK_2(DlgMain::onChapter, this));
	
}

//����������
void DlgMain::onLord(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {

	}
}
//����������
void DlgMain::onSearch(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {
		
	}
}
//��ս������
void DlgMain::onFight(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {
		
	}
}
//�򿪲�����
void DlgMain::onSetting(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {
		
	}
}
//�򿪹ؿ�����
void DlgMain::onChapter(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {
		
	}
}