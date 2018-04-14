#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocostudio;
using namespace cocos2d::ui;

#include "DlgMain.h"
#include "Core/BaseComponent.h"
#include "DLG/Main/PnlSetting.h"

USING_NS_CC;




DlgMain::DlgMain()
	: _lay_mid(nullptr)
	, _pnl_setting(nullptr)
	, _pnls()
	, _cur_pnl(nullptr)
{
	_dlg_type = ENUM_DLG_TYPE::Full;
	_dlg_name = "DlgFight";
}

DlgMain::~DlgMain()
{

}

bool DlgMain::init(StateBase* gameState)
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

void DlgMain::load()
{
	auto lay_root = GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/dlg_mian.json");
	CCLOG("=============>%p", lay_root);
	this->addChild(lay_root);
	this->setName("dlg_mian");


	//panel����
	this->_lay_mid = (Layout*)Helper::seekWidgetByName(lay_root, "lay_mid");

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

void DlgMain::showPanel(PanelType type) {
	
	BaseComponent* comp = this->getPanel(type);

	//ͬһ����壬�򷵻�
	if (comp == this->_cur_pnl) {
		return;
	}

	//������ʾ����壬������
	if (this->_cur_pnl != nullptr) {
		this->_cur_pnl->setVisible(false);
	}

	//���õ�ǰ��岢��ʾ����
	this->_cur_pnl = comp;
	this->_cur_pnl->setVisible(true);
	this->_cur_pnl->updateUI();
}

BaseComponent* DlgMain::getPanel(PanelType type) {
	BaseComponent* comp = nullptr;
	auto it = this->_pnls.find(type);
	if (it == this->_pnls.end()) {
		switch (type)
		{
		case PanelType::Setting:
			comp = PnlSetting::create();
			_pnls[type] = comp;
			this->_lay_mid->addChild(comp);
			break;
		}
	}
	else {
		comp = it->second;
	}

	return comp;
}

void DlgMain::showDlg(const string& dlgName)
{
	DlgBase::showDlg(dlgName);
}

void DlgMain::hideDlg(const string& dlgName)
{
	DlgBase::hideDlg(dlgName);
}

//����������
void DlgMain::onLord(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {
		CCLOG("%s", "����������");
	}
}
//����������
void DlgMain::onSearch(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {
		CCLOG("����������");
	}
}
//��ս������
void DlgMain::onFight(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {
		CCLOG("��ս������");
		showDlg("DlgFight");
	}
}
//�򿪲�����
void DlgMain::onSetting(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {
		this->showPanel(PanelType::Setting);
	}
}
//�򿪹ؿ�����
void DlgMain::onChapter(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {
		CCLOG("�򿪹ؿ�����");
	}
}