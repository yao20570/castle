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
#include "Skill/Skill.h"
#include "Skill/SkillMgr.h"
#include "Core/Msg/MsgMgr.h"
#include "Cocos2dEx/SkillEffectAnim.h"

USING_NS_CC;


//static const char* PathChaterCfg = "Config/Chater.csv";
//static const char* PathFormatChaterObjCfg = "Config/Chater/%s.csv";
static const char* PathObjPosition = "Config/ObjPosition.csv";
static const char* PathHeroInfo = "Config/HeroInfo.csv";
static const char* PathSoilderInfo = "Config/SoilderInfo.csv";

#define MaxWaitCD 2
#define MaxReadyCD 30
#define MaxReadyCD2 3 
#define MaxTotalTime 3 
#define MaxResultCD 2

static int DlgFight_delay_close = 90;
static float TotalTime = MaxTotalTime;
static float WaitCD = MaxWaitCD;
static float ReadyCD = MaxReadyCD;
static float ReadyCD2 = MaxReadyCD2;
static float ResultCD = MaxResultCD;

static int MYWIN = 0;
static int MYLOSE = 0;




DlgFight::DlgFight()
	: _objPosCfg(nullptr)
	, _player_id(0)
	, _lay_floor(nullptr)
	, _lay_fight(nullptr)
	, _lay_sky(nullptr)
	, _skill_shoot_range(nullptr)
	, _skill_radius(nullptr)
	//, _select_obj(nullptr)
	, _round(0)
	, _floor_aminas()
{
	_dlg_type = ENUM_DLG_TYPE::Full;
	_dlg_name = "DlgFight";
	_state = MapState::Wait;
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

	MsgCenter()->delListener(Msg_AddSkillAnim, this);
	MsgCenter()->delListener(Msg_DelSkillAnim, this);
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
	this->load();

	MsgCenter()->addListener(Msg_AddSkillAnim, this, CC_CALLBACK_1(DlgFight::addSkillAnim, this));
	MsgCenter()->addListener(Msg_DelSkillAnim, this, CC_CALLBACK_1(DlgFight::delSkillAnim, this));


	schedule(schedule_selector(DlgFight::update));
	return true;
}

void DlgFight::load()
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->playBackgroundMusic(OGG_BATTLEGROUND, true);

	auto lay_root = GUIReader::getInstance()->widgetFromJsonFile("UI/DlgFight/dlg_fight.json");
	this->addChild(lay_root);


	_lay_floor = (Layout*)Helper::seekWidgetByName(lay_root, "lay_floor");
	_lay_sky = (Layout*)Helper::seekWidgetByName(lay_root, "lay_sky");
	_lay_fight = (Layout*)Helper::seekWidgetByName(lay_root, "lay_fight");
	_lay_fight->addTouchEventListener(CC_CALLBACK_2(DlgFight::onMapTouch, this));

	//this->_setting_data = DBM()->getMySetting(0);
	//setObjPosition();

	this->img_lose = (ImageView*)Helper::seekWidgetByName(lay_root, "img_lose");
	this->img_win = (ImageView*)Helper::seekWidgetByName(lay_root, "img_win");
	this->img_lose->setVisible(false);
	this->img_win->setVisible(false);

	//开始
	this->img_tip = (ImageView*)Helper::seekWidgetByName(lay_root, "img_tip");
	this->lay_line = (ImageView*)Helper::seekWidgetByName(lay_root, "lay_line");
	this->btn_start = (Button*)Helper::seekWidgetByName(lay_root, "btn_start");
	this->btn_start->addTouchEventListener(CC_CALLBACK_2(DlgFight::onStart, this));
	this->btn_start->setScale(1.5);

	this->lay_wait = (Layout*)Helper::seekWidgetByName(lay_root, "lay_wait");
	this->lay_wait->setVisible(false);
	this->lab_round = (Text*)Helper::seekWidgetByName(this->lay_wait, "lab_round");

	this->lay_result = (Layout*)Helper::seekWidgetByName(lay_root, "lay_result");
	this->lay_result->setVisible(false);
	this->lay_result->setSwallowTouches(false);
	this->lay_result->addTouchEventListener(CC_CALLBACK_2(DlgFight::onNextRound, this));

	this->txt_num = (Text*)Helper::seekWidgetByName(lay_root, "txt_num");
	this->txt_result = (Text*)Helper::seekWidgetByName(lay_root, "txt_result");

	//属性面板
	this->lay_attr = (Layout*)Helper::seekWidgetByName(lay_root, "lay_attr");
	this->txt_name = (Text*)Helper::seekWidgetByName(lay_root, "txt_name");
	this->txt_hp = (Text*)Helper::seekWidgetByName(lay_root, "txt_hp");
	this->txt_damage = (Text*)Helper::seekWidgetByName(lay_root, "txt_damage");
	this->txt_def = (Text*)Helper::seekWidgetByName(lay_root, "txt_def");
	this->txt_speed = (Text*)Helper::seekWidgetByName(lay_root, "txt_speed");
	this->txt_shoot_range = (Text*)Helper::seekWidgetByName(lay_root, "txt_shoot_range");
	this->txt_xixue = (Text*)Helper::seekWidgetByName(lay_root, "txt_xixue");
	this->txt_hurt_more = (Text*)Helper::seekWidgetByName(lay_root, "txt_hurt_more");

	this->txt_hp_diff = (Text*)Helper::seekWidgetByName(lay_root, "txt_hp_diff");
	this->txt_damage_diff = (Text*)Helper::seekWidgetByName(lay_root, "txt_damage_diff");
	this->txt_def_diff = (Text*)Helper::seekWidgetByName(lay_root, "txt_def_diff");
	this->txt_speed_diff = (Text*)Helper::seekWidgetByName(lay_root, "txt_speed_diff");
	this->txt_shoot_range_diff = (Text*)Helper::seekWidgetByName(lay_root, "txt_shoot_range_diff");

}

void DlgFight::update(float dt) {
	_ai->update(dt);

	switch (_state)
	{
		case MapState::Wait:
		{
			//等待阶段
			this->lay_wait->setVisible(true);
			this->lay_result->setVisible(false);
			_ai->hideEnemy();

			if (WaitCD < 0){
				WaitCD = MaxWaitCD;
				_state = MapState::Setting;
			}
			else{
				WaitCD -= dt;

				char str[128] = { 0 };
				sprintf(str, CFG()->getWord(1).c_str(), _round + 1);
				this->lab_round->setString(str);
			}
		}
			break;
		case MapState::Setting:
		{
			//准备阶段
			this->lay_wait->setVisible(false);
			this->lay_result->setVisible(true);
			this->btn_start->setVisible(true);
			this->img_tip->setVisible(true);
			this->lay_line->setVisible(true);
			this->txt_num->setVisible(true);

			this->img_win->setVisible(false);
			this->img_lose->setVisible(false);
			this->txt_result->setVisible(false);
			_ai->hideEnemy();
			if (_ai->isWin(1)) {
				this->txt_num->setFontSize(50);
				this->txt_num->setTextColor(Color4B(0, 255, 0, 255));
				this->txt_num->setString(cocos2d::Value((int)ReadyCD).asString());
			}
			else {
				this->txt_num->setFontSize(50);
				this->txt_num->setTextColor(Color4B(0, 255, 0, 255));
				this->txt_num->setString(cocos2d::Value((int)ReadyCD).asString());
			}

			if (ReadyCD < 0) {
				ReadyCD = MaxReadyCD;
				ReadyCD2 = MaxReadyCD2;
				_ai->showEnemy();
				_state = MapState::Ready;
			}
			else {
				ReadyCD -= dt;
			}
		}
			break;
		case MapState::Ready:
		{
			this->btn_start->setVisible(false);
			this->img_tip->setVisible(false);
			this->lay_line->setVisible(false);
			if (ReadyCD2 < 0) {
				_ai->showName(false);
				_ai->start();
				_state = MapState::Fight;

				this->triggerSkill(SkillTriggerType::RoundStart);
			}
			else {
				ReadyCD2 -= dt;
			}
			this->txt_num->setVisible(true);
			this->txt_num->setFontSize(100);
			this->txt_num->setTextColor(Color4B(255, 0, 0, 255));
			this->txt_num->setString(cocos2d::Value((int)(ReadyCD2 + 1)).asString());
		}
			break;
		case MapState::Fight:
		{
			this->lay_result->setVisible(false);
			if (_ai->isOver(1)) {
				DlgFight_delay_close = 90;
				TotalTime = MaxTotalTime;
				_state = MapState::RoundEnd;
			}

			BaseSprite* obj = _ai->getSelectObj();
			if (obj && this->_skill_shoot_range){
				this->_skill_shoot_range->setPosition(obj->getPosition());
			}
		}
			break;
		case MapState::RoundEnd:
		{
			//虽然武将都死了，战斗结束，但远程攻击还没到达，导致主公的血计算不正确
			//所以需要这个状态，延迟几秒再算主公的血
			if (TotalTime < 0){
				_state = MapState::Settlement;
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
		case MapState::Settlement:
		{
			this->lay_result->setVisible(true);
			this->txt_num->setVisible(false);
			char str[50] = { 0 };
			sprintf(str, "%d - %d", MYWIN, MYLOSE);
			this->txt_result->setString(str);
			this->txt_result->setVisible(true);

			if (MYWIN >= 2 || MYLOSE >= 2){

				if (ResultCD < 0){
					ResultCD = MaxResultCD;
					_state = MapState::FightEnd;
				}
				else{
					ResultCD -= dt;
				}
			}
		}
			break;
		case MapState::Reset:
		{
			++_round;
			_ai->reset();
			setObjPosition();
			//_ai->start();
			_state = MapState::Wait;
		}
			break;
		case MapState::FightEnd:
		{
			this->txt_result->setVisible(false);
			bool isWin = MYWIN > MYLOSE;
			this->img_win->setVisible(isWin);
			this->img_lose->setVisible(!isWin);
		}
			break;
	}

	this->updateObjAttrLayer();

	//地板技能特效跟随角色移动
	for (auto& it : this->_floor_aminas){
		SkillEffectAnim* anim = it.second;
		BaseSprite* obj = anim->getObj();
		if (obj){
			anim->setPosition(obj->getPosition());
		}
	}
}

void DlgFight::addTouch()
{
	//auto dispatcher = this->getEventDispatcher();
	//auto listener = EventListenerTouchOneByOne::create();
	//listener->onTouchBegan = CC_CALLBACK_2(DlgFight::onTouchBegan, this);
	//listener->onTouchMoved = CC_CALLBACK_2(DlgFight::onTouchMoved, this);
	//listener->onTouchEnded = CC_CALLBACK_2(DlgFight::onTouchEnded, this);
	//dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool DlgFight::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	//if (_state != 1) {
	//	return true;
	//}

	//if (_ai->getSelectObj() == nullptr) {
	//	return true;
	//}

	////第一次选中
	//if (_select_obj != _ai->getSelectObj()) {
	//	_select_obj = _ai->getSelectObj();
	//	return true;
	//}

	////是否点在可布置范围
	//auto posInMap = _map->convertToNodeSpace(pTouch->getLocation());
	//int radius = _select_obj->_radius;
	//auto mapSize = _map->getContentSize();
	//Rect rect(radius / 2, radius / 2, mapSize.width - radius, mapSize.height/2 - radius);
	//if (rect.containsPoint(posInMap) == false) {
	//	return false;
	//}
	//
	////是否点在其它的对象上
	//bool isCan = true;
	//for (auto it : _ai->_objSelf) 
	//{
	//	if (it == _select_obj) {
	//		continue;
	//	}		
	//	if (GM()->isPointInCircle(it->getPosition(), MAX(it->_radius , _select_obj->_radius), posInMap)) {
	//		isCan = false;
	//		break;
	//	}
	//}

	//
	////保存设置数据，等下局使用
	//ValueVector& datas = this->_setting_data;
	//for (cocos2d::Value& it : datas) {
	//	ValueMap& row = it.asValueMap();
	//	int objId = row["ObjId"].asInt();
	//	ValueMap* objInfo = CFG()->getObjInfoById(objId);
	//	if (_select_obj->_objType == (*objInfo)["ObjType"].asInt() 
	//		&& _select_obj->_id == (*objInfo)["SubType"].asInt()){
	//		row["x"] = cocos2d::Value(posInMap.x);
	//		row["y"] = cocos2d::Value(posInMap.y);
	//	}
	//}

	return true;
}

void DlgFight::onTouchMoved(Touch* pTouch, Event* pEvent) {


}

void DlgFight::onTouchEnded(Touch* pTouch, Event* pEvent) {
}

BaseSprite* DlgFight::selectObj(const Vec2& touchPos){
	//取消已选中的对象
	BaseSprite* oldObj = _ai->getSelectObj();
	if (oldObj){
		oldObj->setSelect(false);
	}

	//最短距离的对象
	BaseSprite* minDistanceObj = nullptr;
	//最短距离
	int minDistance = 0;
	//获取选中的对象
	cocos2d::Vector<Node*> childs = this->_lay_fight->getChildren();
	for (auto node : childs){
		BaseSprite* obj = dynamic_cast<BaseSprite*>(node);
		if (obj){
			auto posInNode = obj->convertToNodeSpace(touchPos);
			//Vec2(0, 60)为图片的中心点(这个应该由配置表配置，因为对象可能有大有小)
			int dis = posInNode.getDistance(Vec2(0, 60));
			if (dis < obj->_radius) {
				if (minDistanceObj == nullptr || dis < minDistance){
					minDistanceObj = obj;
					minDistance = dis;
				}
			}
		}
	}

	//选中新的对象
	if (minDistanceObj){
		_ai->setSelectObj(minDistanceObj);
		minDistanceObj->setSelect(true);
	}

	return minDistanceObj;
}

void DlgFight::setObjPos(const Vec2& pos){
	BaseSprite* obj = _ai->getSelectObj();

	//是否点在可布置范围
	int radius = obj->_radius;
	auto mapSize = _lay_fight->getContentSize();
	Rect rect(radius / 2, radius / 2, mapSize.width - radius, mapSize.height / 2 - radius);
	if (rect.containsPoint(pos) == false) {
		return;
	}

	//是否和其它的对象上重叠
	for (auto it : _ai->_objSelf) {
		if (it == obj) {
			continue;
		}
		if (GM()->isPointInCircle(it->getPosition(), MAX(it->_radius, obj->_radius), pos)) {
			return;
		}
	}

	_ai->setObjPos(obj, pos);
}

void DlgFight::saveObjPos(){
	//当前选择的对象
	BaseSprite* obj = _ai->getSelectObj();
	if (obj == nullptr){
		return;
	}

	//保存设置数据，等下回合使用
	ValueVector& datas = this->_setting_data;
	for (cocos2d::Value& it : datas) {
		ValueMap& row = it.asValueMap();
		int objId = row["ObjId"].asInt();
		ValueMap& objInfo = *(CFG()->getObjInfoById(objId));
		if (obj->_objType == objInfo["ObjType"].asInt()
			&& obj->_id == objInfo["SubType"].asInt()){
			const Vec2& pos = obj->getPosition();
			row["x"] = cocos2d::Value(pos.x);
			row["y"] = cocos2d::Value(pos.y);
			break;
		}
	}
}


void DlgFight::setObjPosition()
{
	ValueVector& datas = this->_setting_data;
	{
		int camp = 1;
		for (cocos2d::Value& it : datas) {
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
		if (this->_player_id > 0){
			map<int, ValueMap>& enemyDatas = *CFG()->getPlayerById(this->_player_id);
			for (auto& it : enemyDatas) {
				ValueMap& row = it.second;
				int x = row["x"].asInt();
				int y = row["y"].asInt();

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
		}
		else{
			for (cocos2d::Value& it : datas) {
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

		}
		addPlayer(Vec2(320, 920), camp);
	}

}

void DlgFight::addHero(Vec2 pos, int camp, ValueMap* objInfo)
{
	int objType = (*objInfo)["ObjType"].asInt();
	int subType = (*objInfo)["SubType"].asInt();
	string name = (*objInfo)["Name"].asString();
	int quality = (*objInfo)["Quality"].asInt();

	Hero* hero = Hero::create(subType, _ai, camp);
	setTextColorByQuality(hero->_txtName, quality);
	this->_lay_fight->addChild(hero);
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
	setTextColorByQuality(soilder->_txtName, quality);
	this->_lay_fight->addChild(soilder);
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
	this->_lay_fight->addChild(player);
	_ai->setObjPos(player, pos);
}

void DlgFight::setAttrText(Text* txt, int diffNum){
	txt->setVisible(diffNum != 0);
	if (diffNum > 0){
		txt->setString("+" + cocos2d::Value(diffNum).asString());
		setTextColorByQuality(txt, 2);
	}
	else{
		txt->setString(cocos2d::Value(diffNum).asString());
		setTextColorByQuality(txt, 6);
	}
}

void DlgFight::updateObjAttrLayer(){
	if (_state != MapState::Fight){
		this->lay_attr->setVisible(false);
		return;
	}

	BaseSprite* obj = _ai->getSelectObj();
	if (obj != nullptr){
		this->lay_attr->setVisible(true);
		this->txt_name->setString(obj->getObjName());
		this->txt_hp->setString(cocos2d::Value(obj->getHp()).asString());
		this->txt_damage->setString(cocos2d::Value(obj->_damage).asString());
		this->txt_def->setString(cocos2d::Value(obj->_def).asString());
		this->txt_speed->setString(cocos2d::Value(obj->_speed).asString());
		this->txt_shoot_range->setString(cocos2d::Value(obj->_shoot_range).asString());
		this->txt_xixue->setString(cocos2d::Value(obj->getXiXue()).asString() + "%");
		this->txt_hurt_more->setString(cocos2d::Value(obj->getHurtMore()).asString() + "%");

		this->setAttrText(this->txt_damage_diff, obj->getDamage() - obj->_damage);
		this->setAttrText(this->txt_def_diff, obj->getDef() - obj->_def);
		this->setAttrText(this->txt_speed_diff, obj->getSpeed() - obj->_speed);
		this->setAttrText(this->txt_shoot_range_diff, obj->getShootRange() - obj->_shoot_range);

		this->txt_damage_diff->setPositionX(this->txt_damage->getPositionX() + this->txt_damage->getContentSize().width + 25);
		this->txt_def_diff->setPositionX(this->txt_def->getPositionX() + this->txt_def->getContentSize().width + 25);
		this->txt_speed_diff->setPositionX(this->txt_speed->getPositionX() + this->txt_speed->getContentSize().width + 25);
		this->txt_shoot_range_diff->setPositionX(this->txt_shoot_range->getPositionX() + this->txt_shoot_range->getContentSize().width + 25);
	}
	else{
		this->lay_attr->setVisible(false);
	}
}

void DlgFight::triggerSkill(SkillTriggerType tt){
	for (auto it : _ai->_objSelf){
		it->triggerSkill(tt, it->getPosition());
	}
	for (auto it : _ai->_objEnemy){
		it->triggerSkill(tt, it->getPosition());
	}
}

void DlgFight::showSkillRange(bool isShow, BaseSprite* obj){
	if (_skill_shoot_range){
		_skill_shoot_range->removeFromParent();
		_skill_shoot_range = nullptr;

		if (_skill_radius){
			_skill_radius = nullptr;
		}
	}

	if (isShow && obj && obj->_mgr_skill){

		vector<Skill*>* skills = obj->_mgr_skill->getSkills(SkillTriggerType::Hand);
		if (skills == nullptr || skills->empty()){
			return;
		}

		Skill* skill = (*skills)[0];
		if (skill){
			switch (skill->scopeType)
			{
				case SkillScopeType::ALL:
					break;
				case SkillScopeType::SINGLE:
				case SkillScopeType::ROUND:
				{
					_skill_shoot_range = DrawNode::create();
					_skill_shoot_range->drawCircle(this->getPosition(), skill->shootRange, 0, 100, false, Color4F(1, 1, 1, 1));
					_skill_shoot_range->setPosition(obj->getPosition());
					this->_lay_floor->addChild(_skill_shoot_range);

					_skill_radius = DrawNode::create();
					_skill_radius->drawCircle(this->getPosition(), skill->radius, 0, 100, false, Color4F(1, 1, 1, 1));
					_skill_shoot_range->addChild(_skill_radius);

					break;
				}
				case SkillScopeType::RECT_SCOPE:
					break;
				case SkillScopeType::FAN:
					break;
			}
		}
	}
}

void DlgFight::addSkillAnim(void* data){

	SkilAnimData* animData = (SkilAnimData*)data;

	SkillEffectAnim* arm = SkillEffectAnim::create(animData->fileName);
	arm->getAnimation()->play("Animation1");
	arm->setKey(animData->key);
	arm->setObj(animData->obj);
	arm->setRotation(animData->angle);
	if (animData->loop == 0){
		//不循环移除
		arm->getAnimation()->setMovementEventCallFunc([arm, animData](Armature* armature, MovementEventType movementType, const std::string& movementID){
			if (movementType == cocostudio::LOOP_COMPLETE){				
				switch (animData->layerType)
				{			
					case SkillAnimLayerType::Body:{
						auto cb = CallFunc::create([animData](){
							animData->obj->delSkillEffectAnim(animData->key);
						});
						arm->runAction(cb);
						break;			
					}	
					default:{
						RemoveSelf* rs = RemoveSelf::create();
						arm->runAction(rs);
						break;
					}
						
				}
			}
		});
	}
	else if (animData->loop > 0){
		//持续一段时间
		DelayTime* dt = DelayTime::create((float)animData->loop / 1000);
		switch (animData->layerType)
		{			
			case SkillAnimLayerType::Body:{
				auto cb = CallFunc::create([animData](){
					animData->obj->delSkillEffectAnim(animData->key);
				});
				Sequence* seq = Sequence::create(dt, cb, nullptr);
				arm->runAction(seq);
				break;			
			}	
			default:{
				RemoveSelf* rs = RemoveSelf::create();
				Sequence* seq = Sequence::create(dt, rs, nullptr);
				arm->runAction(seq);
				break;
			}
				
		}
		
	}
	else if (animData->loop < 0){
		//循环
	}

	switch (animData->layerType)
	{
		case SkillAnimLayerType::None:
			break;
		case SkillAnimLayerType::Floor:{
			this->_lay_floor->addChild(arm);
			this->_floor_aminas[animData->key] = arm;
			break;
		}
		case SkillAnimLayerType::Body:
			arm->setPosition(Vec2(0, 120));
			animData->obj->addSkillEffectAnim(arm);			
			break;
		case SkillAnimLayerType::Pos:{
			this->_lay_fight->addChild(arm);
			//根据位置类型播放动画
			switch (animData->posType)
			{
				case SkillAnimPosType::Src:
					arm->setPosition(animData->obj->getPosition());
					break;
				case SkillAnimPosType::Target:
					arm->setPosition(animData->targetPos);
					break;
				case SkillAnimPosType::Src2Target:
					arm->setPosition(animData->obj->getPosition());
					arm->runAction(MoveTo::create(0.5, animData->targetPos));
					break;
				case SkillAnimPosType::Target2Src:
					arm->setPosition(animData->targetPos);
					arm->runAction(MoveTo::create(0.5, animData->obj->getPosition()));
					break;
			}
			break;
		}

		case SkillAnimLayerType::Sky:{
			Size s = this->_lay_fight->getContentSize();
			Vec2 p(s.width / 2, s.height / 2 + 100);
			arm->setPosition(p);
			this->_lay_sky->addChild(arm);
			break;
		}
	}
}

void DlgFight::delSkillAnim(void* data){
	SkilAnimData* animData = (SkilAnimData*)data;

	switch (animData->layerType)
	{
		case SkillAnimLayerType::None:
			break;
		case SkillAnimLayerType::Floor:{
			auto it = this->_floor_aminas.find(animData->key);
			if (it != this->_floor_aminas.end()){
				it->second->removeFromParent();
				this->_floor_aminas.erase(it);
			}
			break;
		}
		case SkillAnimLayerType::Body:
			animData->obj->delSkillEffectAnim(animData->key);
			break;
		case SkillAnimLayerType::Pos:{

			break;
		}

		case SkillAnimLayerType::Sky:
			break;
	}


}

void DlgFight::setFightType(int i){
	this->_player_id = i;

	this->_setting_data = DBM()->getMySetting(0);
	setObjPosition();
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

void DlgFight::onMapTouch(Ref* sender, Widget::TouchEventType type){
	switch (type)
	{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:	this->onMapTouchBegan(sender, type);	break;
		case cocos2d::ui::Widget::TouchEventType::MOVED:	this->onMapTouchMove(sender, type);		break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:	this->onMapTouchEnd(sender, type);		break;
		case cocos2d::ui::Widget::TouchEventType::CANCELED:	this->onMapTouchEnd(sender, type);	break;
	}
}

void DlgFight::onMapTouchBegan(Ref* sender, Widget::TouchEventType type){
	switch (_state)
	{
		case MapState::Wait:
			break;
		case MapState::Setting:
		{
			//点击位置
			const Vec2& touchPos = this->_lay_fight->getTouchBeganPosition();
			BaseSprite* obj = this->selectObj(touchPos);
			if (obj){
				this->_obj_pos_before_move = obj->getPosition();
			}
			else{
				this->_obj_pos_before_move = Vec2::ZERO;
			}
		}
			break;
		case MapState::Ready:
			break;
		case MapState::Fight:
		{
			//点击位置
			const Vec2& touchPos = this->_lay_fight->getTouchBeganPosition();
			BaseSprite* obj = this->selectObj(touchPos);
			if (obj){
				this->showSkillRange(true, obj);
			}
		}
			break;
		case MapState::RoundEnd:
			break;
		case MapState::Settlement:
			break;
		case MapState::Reset:
			break;
		case MapState::FightEnd:
			break;
	}
}

void DlgFight::onMapTouchMove(Ref* sender, Widget::TouchEventType type){
	switch (_state)
	{
		case MapState::Wait:
			break;
		case MapState::Setting:{
			BaseSprite* obj = _ai->getSelectObj();
			if (obj){
				const Vec2& beganPos = this->_lay_fight->getTouchBeganPosition();
				const Vec2& movePos = this->_lay_fight->getTouchMovePosition();
				Vec2 x = movePos - beganPos;
				Vec2 pos = this->_obj_pos_before_move + x;
				this->setObjPos(pos);
			}
			break;
		}
		case MapState::Ready:
			break;
		case MapState::Fight:{
			BaseSprite* obj = _ai->getSelectObj();
			if (_skill_radius && obj){

				Skill* skill = obj->_mgr_skill->getHandSkill();
				if (skill){
					const Vec2& movePos = this->_lay_fight->getTouchMovePosition();
					Vec2 pos = this->_skill_radius->getParent()->convertToNodeSpace(movePos);
					int distance = pos.getDistance(Vec2::ZERO);
					if (distance < skill->shootRange){
						_skill_radius->setPosition(pos);
					}
				}
			}
			break;
		}
		case MapState::RoundEnd:
			break;
		case MapState::Settlement:
			break;
		case MapState::Reset:
			break;
		case MapState::FightEnd:
			break;
	}
}

void DlgFight::onMapTouchEnd(Ref* sender, Widget::TouchEventType type){
	switch (_state)
	{
		case MapState::Wait:
			break;
		case MapState::Setting:		{
			this->saveObjPos();
			break;
		}
		case MapState::Ready:
			break;
		case MapState::Fight:{
			BaseSprite* obj = _ai->getSelectObj();
			if (obj){

				vector<Skill*>* skills = obj->_mgr_skill->getSkills(SkillTriggerType::Hand);
				if (skills == nullptr || skills->empty()){
					return;
				}

				Skill* skill = (*skills)[0];
				if (skill){
					switch (skill->scopeType)
					{
						case SkillScopeType::ALL:{
							obj->triggerSkill(SkillTriggerType::Hand, obj->getPosition());
							break;
						}
						case SkillScopeType::SINGLE:
						case SkillScopeType::ROUND:{
							if (_skill_radius){
								Vec2 worldPos = _skill_radius->convertToWorldSpace(Vec2::ZERO);
								Vec2 pos = obj->getParent()->convertToNodeSpace(worldPos);
								obj->triggerSkill(SkillTriggerType::Hand, pos);
							}
							break;
						}
						case SkillScopeType::RECT_SCOPE:
							break;
						case SkillScopeType::FAN:
							break;
					}
				}

			}
			this->showSkillRange(false, nullptr);
			break;
		}
		case MapState::RoundEnd:
			break;
		case MapState::Settlement:
			break;
		case MapState::Reset:
			break;
		case MapState::FightEnd:
			break;
	}
}


void DlgFight::onStart(Ref* sender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) {
		return;
	}

	ReadyCD = MaxReadyCD;
	ReadyCD2 = MaxReadyCD2;
	_ai->showEnemy();
	_state = MapState::Ready;
}

void DlgFight::onNextRound(Ref* sender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) {
		return;
	}

	switch (_state)
	{
		case MapState::Settlement:
			if (MYWIN < 2 && MYLOSE < 2) {
				_state = MapState::Reset;
			}
			break;
		case MapState::FightEnd:
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