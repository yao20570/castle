#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocostudio;
using namespace cocos2d::ui;


#include "DlgFight.h"
#include "AIMgr.h"
#include "Utils/DataManager.h"
#include "Utils/DBManager.h"
#include "Utils/GlobalValue.h"
#include "Utils/ConfigMgr.h"
#include "Utils/UIUtils.h"
#include "Model/Hero.h"
#include "Model/Soilder.h"
#include "Model/PlayerObj.h"

USING_NS_CC;


//static const char* PathChaterCfg = "Config/Chater.csv";
//static const char* PathFormatChaterObjCfg = "Config/Chater/%s.csv";
static const char* PathObjPosition	= "Config/ObjPosition.csv";
static const char* PathHeroInfo		= "Config/HeroInfo.csv";
static const char* PathSoilderInfo	= "Config/SoilderInfo.csv";

#define MaxWaitCD 2
#define MaxReadyCD 30 
#define MaxReadyCD2 3 
#define MaxTotalTime 3 
#define MaxResultCD 2

static int state = 0;//0:等待，1:准备，2：战斗，3:等待结束，4，5，6
static int DlgFight_delay_close = 90;
static float TotalTime = MaxTotalTime;
static float WaitCD = MaxWaitCD;
static float ReadyCD = MaxReadyCD;
static float ReadyCD2 = MaxReadyCD2;
static float ResultCD = MaxResultCD;

static int MYWIN = 0;
static int MYLOSE = 0;

DlgFight::DlgFight()	
	:_objPosCfg(nullptr)
	, _map(nullptr)
	, _select_obj(nullptr)
	, _round(0)
{
	_dlg_type = ENUM_DLG_TYPE::Full;
	_dlg_name = "DlgFight";
	state = 0;
	WaitCD = MaxWaitCD;
	ReadyCD = MaxReadyCD;
	ReadyCD2 = MaxReadyCD2;
	TotalTime = MaxTotalTime;
	ResultCD = MaxResultCD;
	MYWIN = 0;
	MYLOSE = 0;
}

DlgFight::~DlgFight()
{
	CC_SAFE_DELETE(this->_ai);
}

bool DlgFight::init(StateBase* gameState)
{
	if (this->_is_init == true) {
		return true;
	}
	this->_is_init = true;
	this->_game_state = gameState;
	this->_ai = new AIMgr();
	this->addTouch();
	//////////////////////////////
	// 1. super init first
	//if (!PanZoomLayer::init())
	if (!Layer::init())
	{
		return false;
	}
	schedule(schedule_selector(DlgFight::update));
	this->load();
	return true;
}
void DlgFight::update(float dt) {
	_ai->update(dt);

	switch (state)
	{
	case 0:
	{
		//等待阶段
		this->lay_wait->setVisible(true);
		this->lay_result->setVisible(false);
		_ai->hideEnemy();
		
		if (WaitCD < 0){
			WaitCD = MaxWaitCD;
			state = 1;
		}
		else{
			WaitCD -= dt;

			char str[128] = {0};
			sprintf(str, CFG()->getWord(1).c_str(), _round + 1);
			this->lab_round->setString(str);
		}
	}
		break;
	case 1:
	{
		//准备阶段
		this->lay_wait->setVisible(false);
		this->lay_result->setVisible(true);
		this->btn_start->setVisible(true);
		this->img_tip->setVisible(true);
		this->lay_line->setVisible(true);

		this->img_win->setVisible(false);
		this->img_lose->setVisible(false);
		this->txt_result->setVisible(false);
		_ai->hideEnemy();
		if (_ai->isWin(1)) {
			this->txt_num->setFontSize(50);
			this->txt_num->setTextColor(Color4B(0, 255, 0, 255));
			this->txt_num->setString(Value((int)ReadyCD).asString());	
		}
		else {
			this->txt_num->setFontSize(50);
			this->txt_num->setTextColor(Color4B(0, 255, 0, 255));
			this->txt_num->setString(Value((int)ReadyCD).asString());
		}

		if (ReadyCD < 0) {
			ReadyCD = MaxReadyCD;
			ReadyCD2 = MaxReadyCD2;
			_ai->showEnemy();
			state = 2;
		}
		else {
			ReadyCD -= dt;
		}
	}
		break;
	case 2:
	{
		this->btn_start->setVisible(false);
		this->img_tip->setVisible(false);
		this->lay_line->setVisible(false);
		if (ReadyCD2 < 0) {
			_ai->showName(false);
			_ai->start();
			state = 3;
		}
		else {
			ReadyCD2 -= dt;
		}
		this->txt_num->setVisible(true);
		this->txt_num->setFontSize(100);
		this->txt_num->setTextColor(Color4B(255, 0, 0, 255));
		this->txt_num->setString(Value((int)(ReadyCD2 + 1)).asString());
	}
		break;
	case 3:
	{		
		this->lay_result->setVisible(false);
		if (_ai->isOver(1)) {
			DlgFight_delay_close = 90;
			TotalTime = MaxTotalTime;
			state = 4;
		}
	}
		break;
	case 4:
	{		
		//虽然武将都死了，战斗结束，但远程攻击还没到达，导致主公的血计算不正确
		//所以需要这个状态，延迟几秒再算主公的血
		if (TotalTime < 0){
			state = 5;
			if (_ai->isWin(1)) {
				++MYWIN;
			}
			else {
				++MYLOSE;
			}
		}
		else{
			TotalTime -= dt;
		}
	}
		break;
	case 5:
	{		
		this->lay_result->setVisible(true);
		this->txt_num->setVisible(false);
		char str[50] = {0};
		sprintf(str, "%d - %d", MYWIN, MYLOSE);
		this->txt_result->setString(str);
		this->txt_result->setVisible(true);

		if (MYWIN >= 2 || MYLOSE >= 2){

			if (ResultCD < 0){
				ResultCD = MaxResultCD;
				state = 7;
			}
			else{
				ResultCD -= dt;
			}			
		}
	}
		break;
	case 6:
	{	
		++_round;
		_ai->reset();
		setObjPosition();
		//_ai->start();
		state = 0;
	}
		break;
	case 7:
	{
		this->txt_result->setVisible(false);
		bool isWin = MYWIN > MYLOSE;
		this->img_win->setVisible(isWin);
		this->img_lose->setVisible(!isWin);
	}
		break;
	}

	
}

void DlgFight::load()
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->playBackgroundMusic(OGG_BATTLEGROUND, true);

	auto lay_root = GUIReader::getInstance()->widgetFromJsonFile("UI/DlgFight/dlg_fight.json");
	this->addChild(lay_root);


	_map = (Layout*)lay_root->getChildByName("lay_fight");
	
	////获取所有剧情
	//map<int, ValueMap>* chaterCfg = DM()->loadCsvData(PathChaterCfg, "ID");

	////当前剧情
	//auto it = chaterCfg->find(CUR_CHATER_ID);
	//if (it == chaterCfg->end()) {
	//	//没有剧情则关闭
	//	hideDlg(_dlg_name);
	//	assert(false, "找不到剧情配置");
	//	return;
	//}
	//ValueMap pCurChater = it->second;

	////剧情对应位置的配置文件
	//char strPathOjbCfg[100];
	//sprintf(strPathOjbCfg, PathFormatChaterObjCfg, pCurChater["DetailKey"].asString());
	//map<int, ValueMap>* objPosCfg = DM()->loadCsvData(PathObjPosition, "ID");

	this->_setting_data = DBM()->getMySetting(0);
	setObjPosition();

	this->img_lose= (ImageView*)Helper::seekWidgetByName(lay_root, "img_lose");
	this->img_win = (ImageView*)Helper::seekWidgetByName(lay_root, "img_win");
	this->img_lose->setVisible(false);
	this->img_win->setVisible(false);

	//开始
	this->img_tip = (ImageView*)Helper::seekWidgetByName(lay_root, "img_tip");
	this->lay_line = (ImageView*)Helper::seekWidgetByName(lay_root, "lay_line");
	this->btn_start = (Button*)Helper::seekWidgetByName(lay_root, "btn_start");
	this->btn_start->addTouchEventListener(CC_CALLBACK_2(DlgFight::onStart, this));
	this->btn_start->setScale(1.5);
	
	////开始
	//auto btnStart = (Layout*)Helper::seekWidgetByName(lay_root, "lay_btn_1");
	//btnStart->addTouchEventListener(CC_CALLBACK_2(DlgFight::onStart, this));
	////关闭
	//auto btnClose = (Layout*)Helper::seekWidgetByName(lay_root, "lay_btn_5");
	//btnClose->addTouchEventListener(CC_CALLBACK_2(DlgFight::onClose, this));
	//关闭
	this->lay_wait = (Layout*)Helper::seekWidgetByName(lay_root, "lay_wait");
	this->lay_wait->setVisible(false);
	this->lab_round = (Text*)Helper::seekWidgetByName(this->lay_wait, "lab_round");

	this->lay_result = (Layout*)Helper::seekWidgetByName(lay_root, "lay_result");
	this->lay_result->setVisible(false);
	this->lay_result->setSwallowTouches(false);
	this->lay_result->addTouchEventListener(CC_CALLBACK_2(DlgFight::onNextRound, this));

	this->txt_num = (Text*)Helper::seekWidgetByName(lay_root, "txt_num");
	this->txt_result = (Text*)Helper::seekWidgetByName(lay_root, "txt_result");


}

void DlgFight::addTouch()
{
	auto dispatcher = this->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(DlgFight::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(DlgFight::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(DlgFight::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool DlgFight::onTouchBegan(Touch* pTouch, Event* pEvent) 
{
	if (state != 1) {
		return true;
	}

	if (_ai->getSelectObj() == nullptr) {
		return true;
	}

	//第一次选中
	if (_select_obj != _ai->getSelectObj()) {
		_select_obj = _ai->getSelectObj();
		return true;
	}

	//是否点在可布置范围
	auto posInMap = _map->convertToNodeSpace(pTouch->getLocation());
	int radius = _select_obj->_radius;
	auto mapSize = _map->getContentSize();
	Rect rect(radius / 2, radius / 2, mapSize.width - radius, mapSize.height/2 - radius);
	if (rect.containsPoint(posInMap) == false) {
		return false;
	}
	
	//是否点在其它的对象上
	bool isCan = true;
	for (auto it : _ai->_objSelf) 
	{
		if (it == _select_obj) {
			continue;
		}		
		if (GM()->isPointInCircle(it->getPosition(), MAX(it->_radius , _select_obj->_radius), posInMap)) {
			isCan = false;
			break;
		}
	}

	_ai->setObjPos(_select_obj, posInMap);
	
	ValueVector& datas = this->_setting_data;
	for (Value& it : datas) {
		ValueMap& row = it.asValueMap();
		int objId = row["ObjId"].asInt();
		ValueMap* objInfo = CFG()->getObjInfoById(objId);
		if (_select_obj->_objType == (*objInfo)["ObjType"].asInt() 
			&& _select_obj->_id == (*objInfo)["SubType"].asInt()){
			row["x"] = Value(posInMap.x);
			row["y"] = Value(posInMap.y);
		}
	}

	return true;
}

void DlgFight::onTouchMoved(Touch* pTouch, Event* pEvent) {
}

void DlgFight::onTouchEnded(Touch* pTouch, Event* pEvent) {
}

void DlgFight::setObjPosition()
{
	ValueVector& datas = this->_setting_data;
	{
		int camp = 1;
		for (Value& it : datas) {
			ValueMap& row = it.asValueMap();
			int x = row["x"].asInt();
			int y = row["y"].asInt();

			Vec2 pos(x, y);
			int objId = row["ObjId"].asInt();

			ValueMap* objInfo = CFG()->getObjInfoById(objId);
			int objType = (*objInfo)["ObjType"].asInt();
			//int subType = (*objInfo)["SubType"].asInt();
			//string name = (*objInfo)["Name"].asString();
			switch (objType)
			{
			case 2:
				//武将			
				addHero(pos, camp, objInfo);
				break;
			case 1:
				//士兵
				addSoilder(pos, camp, objInfo);
				break;
			case 3:
				//建筑/障碍等
				
				break;
			}
		}

		addPlayer(Vec2(320, 40), camp);
	}

	{
		int camp = 2;
		for (Value& it : datas) {
			ValueMap& row = it.asValueMap();
			int x = row["x"].asInt();
			int y = 1080 - row["y"].asInt();

			Vec2 pos(x, y);
			int objId = row["ObjId"].asInt();

			ValueMap* objInfo = CFG()->getObjInfoById(objId);
			int objType = (*objInfo)["ObjType"].asInt();
			//int subType = (*objInfo)["SubType"].asInt();
			//string name = (*objInfo)["Name"].asString();

			string name = (*objInfo)["Name"].asString();
			switch (objType)
			{
			case 2:
				//武将			
				addHero(pos, camp, objInfo);
				break;
			case 1:
				//士兵
				addSoilder(pos, camp, objInfo);
				break;
			case 3:
				break;
			}
		}
		addPlayer(Vec2(320, 920), camp);
	}

}




static int i = 0;
void DlgFight::addHero(Vec2 pos, int camp, ValueMap* objInfo)
{
	int objType = (*objInfo)["ObjType"].asInt();
	int subType = (*objInfo)["SubType"].asInt();
	string name = (*objInfo)["Name"].asString();
	int quality = (*objInfo)["Quality"].asInt();

	Hero* hero = Hero::create(subType, _ai, camp);
	hero->setObjName(name);
	setTextColor(hero->txtName, quality);
	this->_map->addChild(hero); 
	_ai->setObjPos(hero, pos);
	//++i;
	//hero->_name = i;

	//auto txtName = Text::create("名称", FONT_ARIAL, 20);
	//txtName->setName("txtName");
	//txtName->setString(GM()->getIntToStr(i));
	//hero->addChild(txtName);
}

void DlgFight::addSoilder(Vec2 pos, int camp, ValueMap* objInfo)
{
	int objType = (*objInfo)["ObjType"].asInt();
	int subType = (*objInfo)["SubType"].asInt();
	string name = (*objInfo)["Name"].asString();
	int quality = (*objInfo)["Quality"].asInt();

	Soilder* soilder = Soilder::create(subType, _ai, camp);
	soilder->setObjName(name);
	setTextColor(soilder->txtName, quality);
	this->_map->addChild(soilder);
	_ai->setObjPos(soilder, pos);
	//++i;
	//soilder->_name = i;

	//auto txtName = Text::create("名称", FONT_ARIAL, 20);
	//txtName->setName("txtName");
	//txtName->setString(GM()->getIntToStr(i));
	//soilder->addChild(txtName);
}

void DlgFight::addPlayer(Vec2 pos, int camp)
{
	PlayerObj* player = PlayerObj::create(1, _ai, camp);
	this->_map->addChild(player);
	_ai->setObjPos(player, pos);
}

DlgBase* DlgFight::showDlg(const string& dlgName)
{
	return DlgBase::showDlg(dlgName);
}

void DlgFight::hideDlg(const string& dlgName)
{
	DlgBase::hideDlg(dlgName);
	_ai->close();
}

void DlgFight::onStart(Ref* sender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) {
		return;
	}

	ReadyCD = MaxReadyCD;
	ReadyCD2 = MaxReadyCD2;
	_ai->showEnemy();
	state = 2;
}

void DlgFight::onNextRound(Ref* sender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) {
		return;
	}

	switch (state)
	{
	case 5:
		if (MYWIN < 2 && MYLOSE < 2) {
			state = 6;
		}
		break;
	case 7:
		hideDlg(this->getDlgName());
		break;
	default:
		break;
	}
}

void DlgFight::onClose(Ref* sender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) {
		return;
	}

	hideDlg(this->getDlgName());
}

void DlgFight::onResetObjPos(Ref* sender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) {
		return;
	}

	//
	int i = 1;
}