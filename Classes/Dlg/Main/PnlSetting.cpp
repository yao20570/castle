
#include "Utils/PublicDefault.h"
#include "Utils/UIUtils.h"
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
	, waiting_objs()
	, drag_start_pos()
	, pnl_obj_select(nullptr)
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
	this->txt_num = (Text*)(Helper::seekWidgetByName(lay_root, "txt_num"));
	//设置面板
	this->pnl_setting = (Layout*)Helper::seekWidgetByName(lay_root, "pnl_setting");
	//主公
	this->pnl_player = (Layout*)Helper::seekWidgetByName(lay_root, "pnl_player");
	this->pnl_player->addTouchEventListener(CC_CALLBACK_2(PnlSetting::onOpenPlayerUI, this));

	char str[128] = { 0 };
	
	int _dir = GM()->getDir(Vec2(0, -1));
	Armature* _arm = Armature::create("Anim_Hero_Liubei");	
	_arm->getAnimation()->play("idle" + GM()->getIntToStr(_dir));

	_arm->setPositionX(this->pnl_player->getContentSize().width/2);
	_arm->setPositionY(0);

	this->pnl_player->addChild(_arm);


	//阵型标签
	for (int i = 0; i < 3; ++i) {
		char str[50] = "\0";
		sprintf(str, "pnl_btn%d", i + 1);
		this->pnls[i] = (Layout*)Helper::seekWidgetByName(lay_root, str);
		this->pnls[i]->addTouchEventListener(CC_CALLBACK_2(PnlSetting::onSelectSetting, this));
		this->pnls[i]->setTag(i);
	}


	this->pnl_save = (Layout*)(Helper::seekWidgetByName(lay_root, "pnl_save"));
	this->pnl_save->addTouchEventListener(CC_CALLBACK_2(PnlSetting::onSaveSetting, this));

	
	this->sv_setting = (ScrollView*)Helper::seekWidgetByName(lay_root, "sv_setting");
	this->sv_setting->setSwallowTouches(false);

	this->sv_obj = (ScrollView*)Helper::seekWidgetByName(lay_root, "sv_obj");
	this->sv_obj->setSwallowTouches(false);


	//拖动容器
	this->pnl_drag = (Layout*)GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/item_obj2.json");
	this->pnl_drag->setSwallowTouches(false);
	this->pnl_drag->setAnchorPoint(Vec2(0.5, 0.5));
	this->pnl_drag_container = (Layout*)Helper::seekWidgetByName(lay_root, "pnl_drag");
	this->pnl_drag_container->addChild(this->pnl_drag);
	this->pnl_drag->setVisible(false);

	this->addTouch();


	this->selectSetting(0);
}

void PnlSetting::updateUI()
{
	this->updateSettingPanel();
	this->updateSelectList();
}

void PnlSetting::updateSelectList() {

	int i = 0;
	const map<int, ValueMap>* myObjs = DBM()->getMyObj();

	Vector<Node*> nodes = this->pnl_setting->getChildren();
	

	map<int, ValueMap> filterObjs;
	for (auto& it : *myObjs) {
		auto row = it.second;

		bool isUsed = false;
		for (auto it : nodes) {
			ValueMap& values = *((ValueMap*)(it->getUserData()));
			if (values["ID"].asInt() == row["ID"].asInt()) {
				isUsed = true;
			}
		}
		if (isUsed == false) {
			filterObjs.insert(make_pair(row["ID"].asInt(), row));
		}
	}


	for (auto& it : filterObjs) {
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
			lay_root->addTouchEventListener(CC_CALLBACK_2(PnlSetting::onSelectObj, this));
			this->sv_obj->addChild(lay_root);
		}
		lay_root->setVisible(true);
		lay_root->setUserData((void*)(cfg["ID"].asInt()));

		ImageView* img_icon = (ImageView*)Helper::seekWidgetByName(lay_root, "img_icon");
		//img_icon->loadTexture(cfg["Head"].asString());
		img_icon->setVisible(false);

		//
		Layout* pnl_select = (Layout*)Helper::seekWidgetByName(lay_root, "pnl_select");
		pnl_select->setVisible(false);

		ImageView* spr_jobBg = (ImageView*)Helper::seekWidgetByName(lay_root, "spr_jobBg");
		spr_jobBg->setVisible(true);
		Text* lab_job = (Text*)Helper::seekWidgetByName(lay_root, "lab_job");
		lab_job->setString(CFG()->getWord( 1 + cfg["Kind"].asInt()));
		setTextColorByKind(lab_job, cfg["Kind"].asInt());



		//名称
		Text* txt_name = (Text*)Helper::seekWidgetByName(lay_root, "txt_name");
		txt_name->setString(cfg["Name"].asString());
		setTextColorByQuality(txt_name, cfg["Quality"].asInt());

		//动画
		string animaName = cfg["Anima"].asString();
		auto arm = lay_root->getChildByTag(999);
		if (!arm || arm->getName() != animaName) {
			if (arm) {
				arm->removeFromParent();
			}

			char str[128] = { 0 };
			sprintf(str, "animation/%s/%s.ExportJson", animaName.c_str(), animaName.c_str());
			ArmatureDataManager::getInstance()->addArmatureFileInfo(str);
			Armature* _arm = Armature::create(animaName);
			_arm->getAnimation()->play("idle0");
			_arm->setPosition(img_icon->getPosition());
			_arm->setTag(999);
			lay_root->addChild(_arm);
		}
		
		

		++i;
	}

	Vector<Node*> svNodes = this->sv_obj->getChildren();
	for (int i = filterObjs.size(); i < svNodes.size(); ++i) {
		svNodes.at(i)->setVisible(false);
	}

	this->sv_obj->setInnerContainerSize(Size(i * 150, 150));
}


void PnlSetting::updateSettingPanel() {
	this->pnl_setting->removeAllChildren();
	
	int unitNum = 0;
	ValueVector datas = DBM()->getMySetting(this->select_setting_id);
	for (cocos2d::Value& it : datas) {
		ValueMap& row = it.asValueMap();
		int x = row["x"].asInt();
		int y = row["y"].asInt();
		int objId = row["ObjId"].asInt();
		
		auto cfgPtr = CFG()->getObjInfoById(objId);
		auto& cfg = *cfgPtr;

		Layout* settingObj = (Layout*)GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/item_obj2.json");
		settingObj->setSwallowTouches(false);
		settingObj->setAnchorPoint(Vec2(0.5, 0.5));
		settingObj->setPositionX(x);
		settingObj->setPositionY(y);
		settingObj->setLocalZOrder(100000000 - y * 10000 + x);
		settingObj->setTag(objId);
		settingObj->setUserData(cfgPtr);
		settingObj->addTouchEventListener(CC_CALLBACK_2(PnlSetting::onSelectPnlSettingObj, this));
		((Layout*)settingObj->getChildByName("pnl_select"))->setVisible(select_fight_id == objId);
		//((ImageView*)settingObj->getChildByName("img_icon"))->loadTexture(cfg["Head"].asString());
		ImageView* img_icon = (ImageView*)settingObj->getChildByName("img_icon");
		img_icon->setVisible(false);

		Text* txt_name = (Text*)settingObj->getChildByName("txt_name");
		txt_name->setString(cfg["Name"].asString());
		setTextColorByQuality(txt_name, cfg["Quality"].asInt());

		Text* txt_kind = (Text*)settingObj->getChildByName("txt_kind");
		txt_kind->setString(CFG()->getWord( 1 + cfg["Kind"].asInt()));
		setTextColorByKind(txt_kind, cfg["Kind"].asInt());
		//动画
		string animaName = cfg["Anima"].asString();
		auto arm = settingObj->getChildByTag(999);
		if (!arm || arm->getName() != animaName) {
			if (arm) {
				arm->removeFromParent();
			}

			char str[128] = { 0 };
			Armature* _arm = Armature::create(animaName);
			_arm->getAnimation()->play("idle4");
			_arm->setPosition(img_icon->getPosition());
			_arm->setTag(999);
			settingObj->addChild(_arm);
		}

		this->pnl_setting->addChild(settingObj);

		unitNum += cfg["Unit"].asInt();
	}
	cocos2d::Value v(unitNum);
	this->txt_num->setString(v.asString() + "/30");
}

void PnlSetting::isObjInSettingPnl(int id) {

}

void PnlSetting::selectSetting(int selectId)
{
	//相同返回
	if (selectId == this->select_setting_id) {
		return;
	}

	//将旧的隐藏
	if (this->select_setting_id != -1) {

		Text* lab = (Text*)Helper::seekWidgetByName(this->pnls[this->select_setting_id], "Lab");

		lab->setTextColor(Color4B(255, 255, 255, 255));
	}

	//显示新的
	this->select_setting_id = selectId;
	Text* lab = (Text*)Helper::seekWidgetByName(this->pnls[this->select_setting_id], "Lab");
	lab->setTextColor(Color4B(255, 0, 0, 255));
	//更新UI
	this->updateUI();
}

void PnlSetting::selectObj(int selectId)
{
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
	Rect rect(40, 40, 560, 520);
	if (!rect.containsPoint(pos)) {
		//在可放置范围
		isCanSetting = false;
	}

	Vec2 playerPos = this->pnl_player->getPosition();
	Size playerSize = this->pnl_player->getSize();
	Rect plyerRect(playerPos.x - 50, playerPos.y - 50, playerSize.width, playerSize.height);
	if (plyerRect.containsPoint(pos)) {
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
			int temp = unitNum + cfg["Unit"].asInt();
			if (temp <= 30) {
				
				Layout* settingObj = (Layout*)GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/item_obj2.json");
				settingObj->setSwallowTouches(false);
				settingObj->setAnchorPoint(Vec2(0.5, 0.5));
				settingObj->setPosition(pos);
				settingObj->setLocalZOrder(100000000 - pos.y * 10000 + pos.x);
				settingObj->setTag(cfg["ID"].asInt());
				settingObj->setUserData(row);
				settingObj->addTouchEventListener(CC_CALLBACK_2(PnlSetting::onSelectPnlSettingObj, this));
				((Layout*)settingObj->getChildByName("pnl_select"))->setVisible(true);


				
				this->pnl_obj_select = settingObj;
				Layout* pnl_select = (Layout*)this->pnl_obj_select->getChildByName("pnl_select");
				pnl_select->setVisible(true);


				//((ImageView*)settingObj->getChildByName("img_icon"))->loadTexture(cfg["Head"].asString());
				ImageView* img_icon = (ImageView*)settingObj->getChildByName("img_icon");
				img_icon->setVisible(false);

				Text* txt_name = (Text*)settingObj->getChildByName("txt_name");
				txt_name->setString(cfg["Name"].asString());
				setTextColorByQuality(txt_name, cfg["Quality"].asInt());
				

				Text* txt_kind = (Text*)settingObj->getChildByName("txt_kind");
				txt_kind->setString(CFG()->getWord( 1 + cfg["Kind"].asInt()));
				setTextColorByKind(txt_kind, cfg["Kind"].asInt());

				//动画
				string animaName = cfg["Anima"].asString();
				auto arm = settingObj->getChildByTag(999);
				if (!arm || arm->getName() != animaName) {
					if (arm) {
						arm->removeFromParent();
					}

					char str[128] = { 0 };
					Armature* _arm = Armature::create(animaName);
					_arm->getAnimation()->play("idle4");
					_arm->setPosition(img_icon->getPosition());
					_arm->setTag(999);
					settingObj->addChild(_arm);
				}


				this->pnl_setting->addChild(settingObj);
				this->pnl_drag->setVisible(false);
				isCanSetting = true;

				this->updateSelectList();

				unitNum += cfg["Unit"].asInt();
			}
			else {
				isCanSetting = false;
			}
		}
	}
	

	if (isCanSetting == false)
	{
		//不在放置范围内
		this->pnl_drag->setVisible(false);
		//this->pnl_drag->runAction(Sequence::create(
		//	MoveTo::create(0.1, this->drag_start_pos),
		//	CallFunc::create([this]() {
		//	this->pnl_drag->setVisible(false);
		//}), NULL
		//));
		this->updateSelectList();
	}
	cocos2d::Value v(unitNum);
	this->txt_num->setString(v.asString() + "/30");
	this->selectObj(InvalidSelectId);
}

void PnlSetting::onTouchCancelled(Touch* pTouch, Event* pEvent) {

}

void PnlSetting::onOpenPlayerUI(Ref* sender, Widget::TouchEventType type) {
	//((DlgMain*)this->hostDlg)->showPanel(PanelType::Player);
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

		//选中列表里的武将
		this->selectObj(id);
		ImageView* img_icon = (ImageView*)this->pnl_drag->getChildByName("img_icon");
		//img_icon->loadTexture((*selectCfg)["Head"].asString());
		img_icon->setVisible(false);

		Text* txt_name = (Text*)this->pnl_drag->getChildByName("txt_name");
		txt_name->setString((*selectCfg)["Name"].asString());
		setTextColorByQuality(txt_name, (*selectCfg)["Quality"].asInt());
		
		Text* txt_kind = (Text*)this->pnl_drag->getChildByName("txt_kind");
		txt_kind->setString(CFG()->getWord( 1 + (*selectCfg)["Kind"].asInt()));
		setTextColorByKind(txt_kind, (*selectCfg)["Kind"].asInt());

		//动画
		string animaName = (*selectCfg)["Anima"].asString();
		auto arm = this->pnl_drag->getChildByTag(999);
		if (!arm || arm->getName() != animaName) {
			if (arm) {
				arm->removeFromParent();
			}

			char str[128] = { 0 };
			Armature* _arm = Armature::create(animaName);
			_arm->getAnimation()->play("idle4");
			_arm->setPosition(img_icon->getPosition());
			_arm->setTag(999);
			this->pnl_drag->addChild(_arm);
		}


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
			//img_icon->loadTexture(cfg["Head"].asString());
			img_icon->setVisible(false);

			Text* txt_name = (Text*)this->pnl_drag->getChildByName("txt_name");
			txt_name->setString(cfg["Name"].asString());
			setTextColorByQuality(txt_name, cfg["Quality"].asInt());
			this->pnl_drag->setVisible(true);
			
			Text* txt_kind = (Text*)this->pnl_drag->getChildByName("txt_kind");
			txt_kind->setString(CFG()->getWord( 1 + cfg["Kind"].asInt()));
			setTextColorByKind(txt_kind, cfg["Kind"].asInt());

			//动画
			string animaName = cfg["Anima"].asString();
			auto arm = this->pnl_drag->getChildByTag(999);
			if (!arm || arm->getName() != animaName) {
				if (arm) {
					arm->removeFromParent();
				}

				char str[128] = { 0 };
				Armature* _arm = Armature::create(animaName);
				_arm->getAnimation()->play("idle4");
				_arm->setPosition(img_icon->getPosition());
				_arm->setTag(999);
				this->pnl_drag->addChild(_arm);
			}

			if (this->pnl_obj_select != nullptr) {
				Layout* pnl_select = (Layout*)this->pnl_obj_select->getChildByName("pnl_select");
				pnl_select->setVisible(false);
			}
			this->pnl_obj_select = nullptr;
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