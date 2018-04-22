
#include "Utils/PublicDefault.h"
#include "PnlSetting.h"


#include "Utils/ConfigMgr.h"
#include "Dlg/Main/DlgMain.h"

USING_NS_CC;

struct ListData
{
	int type;
	int id;
};

static int InvalidSelectId = -99999999;

PnlSetting::PnlSetting()
	:select_obj_id(InvalidSelectId)
	, select_setting_id(-1)
	, setting_objs()
	, drag_start_pos()
{
	memset(this->pnls, 0, 3 * sizeof(BaseSprite*));
}

PnlSetting::~PnlSetting()
{

}

bool PnlSetting::init(DlgBase* dlg)
{
	//////////////////////////////
	// 1. super init first
	//if (!PanZoomLayer::init())
	if (!BaseComponent::init(dlg))
	{
		return false;
	}

	return true;
}

void PnlSetting::load()
{
	auto lay_root = GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/pnl_setting.json");
	this->addChild(lay_root);
	//this->setName("pnl_setting");

	//人口单位
	this->txt_num = (Text*)(lay_root->getChildByName("txt_num"));
	//设置面板
	this->pnl_setting = (Layout*)Helper::seekWidgetByName(lay_root, "pnl_setting");
	//主公
	this->pnl_player = (Layout*)Helper::seekWidgetByName(lay_root, "pnl_player");
	this->pnl_player->addTouchEventListener(CC_CALLBACK_2(PnlSetting::onOpenPlayerUI, this));

	//阵型标签
	for (int i = 0; i < 3; ++i) {
		char str[50] = "\0";
		sprintf(str, "pnl_btn%d", i + 1);
		this->pnls[i] = (Layout*)Helper::seekWidgetByName(lay_root, str);
		this->pnls[i]->addTouchEventListener(CC_CALLBACK_2(PnlSetting::onSelectSetting, this));
		this->pnls[i]->setTag(i);
	}
	this->selectSetting(0);


	this->pnl_save = (Layout*)(lay_root->getChildByName("pnl_save"));
	this->pnl_save->addTouchEventListener(CC_CALLBACK_2(PnlSetting::onSaveSetting, this));

	
	this->sv_setting = (ScrollView*)Helper::seekWidgetByName(lay_root, "sv_setting");
	this->sv_setting->setSwallowTouches(false);

	this->sv_obj = (ScrollView*)Helper::seekWidgetByName(lay_root, "sv_obj");
	this->sv_obj->setSwallowTouches(false);


	//拖动容器
	this->pnl_drag = (Layout*)GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/item_obj.json");
	this->pnl_drag->setSwallowTouches(false);
	this->pnl_drag->setAnchorPoint(Vec2(0.5, 0.5));
	this->pnl_drag->getChildByName("pnl_select")->setVisible(false);
	this->pnl_drag_container = (Layout*)Helper::seekWidgetByName(lay_root, "pnl_drag");
	this->pnl_drag_container->addChild(this->pnl_drag);
	this->pnl_drag->setVisible(false);

	this->addTouch();
}

void PnlSetting::updateUI()
{
	this->updateSelectList();
	this->updateSettingPanel();
}

void PnlSetting::updateSelectList() {

	int i = 0;
	const map<int, ValueMap>* myObjs = DBM()->getMyObj();
	for (auto& it : *myObjs) {
		//数据
		auto row = it.second;
		auto& cfg = *(CFG()->getObjInfoById(row["ID"].asInt()));

		int id = cfg["ID"].asInt();

		Layout* lay_root = (Layout*)this->sv_obj->getChildByTag(i);
		if (lay_root == nullptr) {
			lay_root = (Layout*)GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/item_obj.json");
			lay_root->setSwallowTouches(false);
			lay_root->setPositionX(i * 150);
			lay_root->setTag(i);
			lay_root->setUserData((void*)(cfg["ID"].asInt()));
			lay_root->addTouchEventListener(CC_CALLBACK_2(PnlSetting::onSelectObj, this));
			this->sv_obj->addChild(lay_root);
		}

		ImageView* img_icon = (ImageView*)Helper::seekWidgetByName(lay_root, "img_icon");
		Text* txt_name = (Text*)Helper::seekWidgetByName(lay_root, "txt_name");
		Layout* pnl_select = (Layout*)Helper::seekWidgetByName(lay_root, "pnl_select");

		pnl_select->setVisible(false);

		img_icon->loadTexture(cfg["Icon"].asString());
		txt_name->setString(cfg["Name"].asString());

		++i;
	}

	this->sv_obj->setInnerContainerSize(Size(i * 150, 150));
}


void PnlSetting::updateSettingPanel() {
	this->pnl_setting->removeAllChildren();
	
	int unitNum = 0;
	ValueVector datas = DBM()->getMySetting(this->select_setting_id);
	for (Value& it : datas) {
		ValueMap& row = it.asValueMap();
		int x = row["x"].asInt();
		int y = row["y"].asInt();
		int objId = row["ObjId"].asInt();
		
		auto cfgPtr = CFG()->getObjInfoById(objId);
		auto& cfg = *cfgPtr;

		Layout* settingObj = (Layout*)GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/item_obj.json");
		settingObj->setSwallowTouches(false);
		settingObj->setAnchorPoint(Vec2(0.5, 0.5));
		settingObj->setPositionX(x);
		settingObj->setPositionY(y);
		settingObj->setLocalZOrder(100000000 - y * 10000 + x);
		settingObj->setTag(objId);
		settingObj->setUserData(cfgPtr);
		settingObj->addTouchEventListener(CC_CALLBACK_2(PnlSetting::onSelectPnlSettingObj, this));
		((Layout*)settingObj->getChildByName("pnl_select"))->setVisible(false);
		((ImageView*)settingObj->getChildByName("img_icon"))->loadTexture(cfg["Icon"].asString());
		((Text*)settingObj->getChildByName("txt_name"))->setString(cfg["Name"].asString());
		this->pnl_setting->addChild(settingObj);

		unitNum += cfg["Unit"].asInt();
	}
	Value v(unitNum);
	this->txt_num->setString(v.asString() + "/20");
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
	////将旧的隐藏
	//if (this->select_obj_id != InvalidSelectId) {
	//	Layout* selectItem = (Layout*)this->sv_obj->getChildByTag(this->select_obj_id);
	//	Layout* selectUI = (Layout*)selectItem->getChildByName("pnl_select");
	//	selectUI->setVisible(false);
	//}

	////显示新的
	//this->select_obj_id = selectId;
	//if (selectId != InvalidSelectId) {
	//	Layout* selectItem = (Layout*)this->sv_obj->getChildByTag(this->select_obj_id);
	//	Layout* selectUI = (Layout*)selectItem->getChildByName("pnl_select");
	//	selectUI->setVisible(true);
	//}
	this->select_obj_id = selectId;
	Vector<Node*>& items = this->sv_obj->getChildren();
	for (auto it : items) {
		int cfgId = (int)it->getUserData();
		bool isSelect = cfgId == selectId;
		Layout* selectUI = (Layout*)it->getChildByName("pnl_select");
		selectUI->setVisible(isSelect);
	}

}

void PnlSetting::addTouch() {
	auto dispatcher = this->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(PnlSetting::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(PnlSetting::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(PnlSetting::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(PnlSetting::onTouchCancelled, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this->pnl_setting);
}

bool PnlSetting::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	if (this->select_obj_id != InvalidSelectId) {
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
	bool isCanSetting = true;



	Vec2 pos = this->pnl_setting->convertToNodeSpace(pTouch->getLocation());
	Rect rect(40, 40, 560, 400);
	if (!rect.containsPoint(pos)) {
		//在可放置范围
		isCanSetting = false;
	}

	int unitNum = 0;

	
		//是否点在其他对象身上
	Vector<Node*> nodes = this->pnl_setting->getChildren();
	for (Node* it : nodes) {
		ValueMap* cfg = (ValueMap*)it->getUserData();
		int unit = (*cfg)["Unit"].asInt();
		unitNum += unit;
		if (it->getPosition().getDistance(pos) < 40) {
			isCanSetting = false;
		}
	}
	

	if (isCanSetting) {
		//放置
		ValueMap* row = CFG()->getObjInfoById(this->select_obj_id);
		if (row) {
			ValueMap& cfg = *row;
			Layout* settingObj = (Layout*)GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/item_obj.json");
			settingObj->setSwallowTouches(false);
			settingObj->setAnchorPoint(Vec2(0.5, 0.5));
			settingObj->setPosition(pos);
			settingObj->setLocalZOrder(100000000 - pos.y * 10000 + pos.x);
			settingObj->setTag(cfg["ID"].asInt());
			settingObj->setUserData(row);
			settingObj->addTouchEventListener(CC_CALLBACK_2(PnlSetting::onSelectPnlSettingObj, this));
			((Layout*)settingObj->getChildByName("pnl_select"))->setVisible(false);
			((ImageView*)settingObj->getChildByName("img_icon"))->loadTexture(cfg["Icon"].asString());
			((Text*)settingObj->getChildByName("txt_name"))->setString(cfg["Name"].asString());


			this->pnl_setting->addChild(settingObj);
			this->pnl_drag->setVisible(false);
			isCanSetting = true;

			unitNum += cfg["Unit"].asInt();
		}
	}
	else
	{
		//不在放置范围内
		this->pnl_drag->setVisible(false);
		//this->pnl_drag->runAction(Sequence::create(
		//	MoveTo::create(0.1, this->drag_start_pos),
		//	CallFunc::create([this]() {
		//	this->pnl_drag->setVisible(false);
		//}), NULL
		//));
	}
	Value v(unitNum);
	this->txt_num->setString(v.asString() + "/20");
	this->selectObj(InvalidSelectId);
}

void PnlSetting::onTouchCancelled(Touch* pTouch, Event* pEvent) {

}

void PnlSetting::onOpenPlayerUI(Ref* sender, Widget::TouchEventType type) {
	((DlgMain*)this->hostDlg)->showPanel(PanelType::Player);
}

void PnlSetting::onSelectSetting(Ref* sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED) {
		this->selectSetting(((Layout*)sender)->getTag());
	}
}

void PnlSetting::onSaveSetting(Ref* sender, Widget::TouchEventType type) {
	if (type == Widget::TouchEventType::ENDED) {
		Vector<Node*> nodes = this->pnl_setting->getChildren();
		DBM()->saveMySetting(this->select_setting_id, nodes);

		char str[256] = "\0";
		sprintf(str, "阵型%d保存成功", this->select_setting_id + 1);
		this->hostDlg->showTip(str);
	}
}

void PnlSetting::onSelectObj(Ref* sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN: {


		int id = (int)((Layout*)sender)->getUserData();
		ValueMap* selectCfg = CFG()->getObjInfoById(id);


		Vector<Node*> nodes = this->pnl_setting->getChildren();
		for (auto it : nodes) {
			ValueMap* nodeCfg = (ValueMap*)it->getUserData();
			if (nodeCfg == selectCfg && (*selectCfg)["ObjType"].asInt() == 2) {

				//武将类型并且已放置
				return;
			}
		}

		//选中列表里的武将
		this->selectObj(id);
		ImageView* img_icon = (ImageView*)this->pnl_drag->getChildByName("img_icon");
		Text* txt_name = (Text*)this->pnl_drag->getChildByName("txt_name");
		img_icon->loadTexture((*selectCfg)["Icon"].asString());
		txt_name->setString((*selectCfg)["Name"].asString());
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

//选择放置面板里的武将
void PnlSetting::onSelectPnlSettingObj(Ref* sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN: {

		if (this->select_obj_id == InvalidSelectId) {
			this->selectObj(((Layout*)sender)->getTag());

			auto& cfg = *(CFG()->getObjInfoById(this->select_obj_id));
			ImageView* img_icon = (ImageView*)this->pnl_drag->getChildByName("img_icon");
			Text* txt_name = (Text*)this->pnl_drag->getChildByName("txt_name");
			img_icon->loadTexture(cfg["Icon"].asString());
			txt_name->setString(cfg["Name"].asString());
			this->pnl_drag->setVisible(true);

			((Layout*)sender)->retain();
			((Layout*)sender)->autorelease();
			((Layout*)sender)->removeFromParent();

			break;
		}

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