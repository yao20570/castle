
#include "PnlSetting.h"


#include "Utils/ConfigMgr.h"

USING_NS_CC;

struct ListData
{
	int type;
	int id;
};

PnlSetting::PnlSetting()
	:select_obj_id(-1)
	, select_setting_id(-1)
	, setting_objs()
	, drag_start_pos()
{
	memset(this->pnls, 0, 3 * sizeof(BaseSprite*));
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

	return true;
}

void PnlSetting::load()
{
	auto lay_root = GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/pnl_setting.json");
	this->addChild(lay_root);
	this->setName("pnl_setting");


	for (int i = 0; i < 3; ++i) {
		char str[50] = "\0";
		sprintf(str, "pnl_btn%d", i + 1);
		this->pnls[i] = (Layout*)Helper::seekWidgetByName(lay_root, str);
		this->pnls[i]->addTouchEventListener(CC_CALLBACK_2(PnlSetting::onSelectSetting, this));
		this->pnls[i]->setTag(i);
	}
	this->selectSetting(0);


	this->txt_num = (Label*)Helper::seekWidgetByName(lay_root, "txt_num");



	this->sv_setting = (ScrollView*)Helper::seekWidgetByName(lay_root, "sv_setting");
	this->sv_setting->setSwallowTouches(false);

	this->sv_obj = (ScrollView*)Helper::seekWidgetByName(lay_root, "sv_obj");
	this->sv_obj->setSwallowTouches(false);

	this->pnl_setting = (Layout*)Helper::seekWidgetByName(lay_root, "pnl_setting");


	this->pnl_drag = (Layout*)GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/item_obj.json");
	this->pnl_drag->setSwallowTouches(false);
	this->pnl_drag->setAnchorPoint(Vec2(0.5, 0.5));
	this->pnl_drag->getChildByName("pnl_select")->setVisible(false);
	this->pnl_drag_container = (Layout*)Helper::seekWidgetByName(lay_root, "pnl_drag");
	this->pnl_drag_container->addChild(this->pnl_drag);


	this->addTouch();
}

void PnlSetting::updateUI()
{
	this->updateSelectList();
	this->updateSettingPanel();
}

void PnlSetting::updateSelectList() {

	int i = 0;
	map<int, ValueMap>* myObjs = DBM()->getMyObj();
	for (auto& it : *myObjs) {
		//数据
		auto& row = it.second;
		auto& cfg = *(CFG()->getObjInfoById(row["ID"].asInt()));

		//添加item
		auto lay_root = GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/item_obj.json");
		lay_root->setSwallowTouches(false);
		lay_root->setPositionX(i * 150);
		lay_root->setTag(cfg["ID"].asInt());
		lay_root->addTouchEventListener(CC_CALLBACK_2(PnlSetting::onSelectObj, this));
		++i;
		this->sv_obj->addChild(lay_root);

		ImageView* img_icon = (ImageView*)Helper::seekWidgetByName(lay_root, "img_icon");
		Text* txt_name = (Text*)Helper::seekWidgetByName(lay_root, "txt_name");
		Layout* pnl_select = (Layout*)Helper::seekWidgetByName(lay_root, "pnl_select");

		pnl_select->setVisible(false);

		if (this->select_obj_id == -1) {
			this->selectObj(lay_root->getTag());
		}

		img_icon->loadTexture(cfg["Icon"].asString());
		txt_name->setString(cfg["Name"].asString());
	}

	this->sv_obj->setInnerContainerSize(Size(i * 150, 150));
}

void PnlSetting::updateSettingPanel() {

}

void PnlSetting::selectSetting(int selectId)
{
	//相同返回
	if (selectId == this->select_setting_id) {
		return;
	}

	//将旧的隐藏
	if (this->select_setting_id != -1) {
		this->pnls[this->select_setting_id]->setBackGroundColor(Color3B(150, 200, 255));
	}

	//显示新的
	this->select_setting_id = selectId;
	this->pnls[this->select_setting_id]->setBackGroundColor(Color3B(80, 190, 80));

	//更新设置面板
	this->updateSettingPanel();
}

void PnlSetting::selectObj(int selectId)
{
	//将旧的隐藏
	if (this->select_obj_id != -1) {
		Layout* selectItem = (Layout*)this->sv_obj->getChildByTag(this->select_obj_id);
		Layout* selectUI = (Layout*)selectItem->getChildByName("pnl_select");
		selectUI->setVisible(false);
	}

	//显示新的
	this->select_obj_id = selectId;
	Layout* selectItem = (Layout*)this->sv_obj->getChildByTag(this->select_obj_id);
	Layout* selectUI = (Layout*)selectItem->getChildByName("pnl_select");
	selectUI->setVisible(true);
}

void PnlSetting::addTouch() {
	auto dispatcher = this->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(PnlSetting::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(PnlSetting::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(PnlSetting::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this->pnl_setting);
}

bool PnlSetting::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	if (pEvent->getCurrentTarget() == this->pnl_setting) {
		this->drag_start_pos = this->pnl_drag_container->convertToNodeSpace(pTouch->getLocation());
		this->pnl_drag->setPosition(this->drag_start_pos);
		this->pnl_drag->setVisible(true);
	}

	return true;
}

void PnlSetting::onTouchMoved(Touch* pTouch, Event* pEvent) {
	if (pEvent->getCurrentTarget() == this->pnl_setting) {
		Vec2 pos = this->pnl_drag_container->convertToNodeSpace(pTouch->getLocation());
		this->pnl_drag->setPosition(pos);
	}
}

void PnlSetting::onTouchEnded(Touch* pTouch, Event* pEvent) {
	if (pEvent->getCurrentTarget() == this->pnl_setting) {


		Vec2 pos = this->pnl_setting->convertToNodeSpace(pTouch->getLocation());
		Rect rect(40, 40, 600, 440);
		if (rect.containsPoint(pos)) {
			//放置
			ValueMap& info = *CFG()->getObjInfoById(this->select_obj_id);
			int id = info["ID"].asInt();
			int objType = info["ObjType"].asInt();
			this->setting_objs.push_back(info["ID"].asInt());

			switch (objType)
			{
			case 1: {
				break;
			}
			case 2:
			{
				Hero* hero = Hero::create(heroId, _ai, 1);
				this->_map->addChild(hero);
				break;
			}
			}




		}
		else {
			//不在放置范围内
			this->pnl_drag->runAction(Sequence::create(
				MoveTo::create(0.1, this->drag_start_pos),
				CallFunc::create([this]() {
				this->pnl_drag->setVisible(false);
			}), NULL
			));
		}

	}
}

void PnlSetting::onSelectSetting(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {
		this->selectSetting(((Layout*)sender)->getTag());
	}
}

void PnlSetting::onSaveSetting(Ref* sender, Widget::TouchEventType type) {
	if (type == Widget::TouchEventType::ENDED) {
		this->selectSetting(((Layout*)sender)->getTag());
	}
}

void PnlSetting::onSelectObj(Ref* sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN: {
		this->selectObj(((Layout*)sender)->getTag());

		auto& cfg = *(CFG()->getObjInfoById(this->select_obj_id));
		ImageView* img_icon = (ImageView*)this->pnl_drag->getChildByName("img_icon");
		Text* txt_name = (Text*)this->pnl_drag->getChildByName("txt_name");
		img_icon->loadTexture(cfg["Icon"].asString());
		txt_name->setString(cfg["Name"].asString());
		break;
	}

	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}