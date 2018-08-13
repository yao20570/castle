#pragma once
#include "cocos2d.h"

#include "../../Core/DlgBase.h"
#include "../../Core/DlgDefine.h"


const enum class MapState :int{
	Wait = 0,		//等待阶段
	Setting = 1,	//部署阶段
	Ready = 2,		//准备
	Fight = 3,		//战斗
	RoundEnd = 4,	//回合结束（但远程攻击还没到达，导致主公的血计算不正确）
	Settlement = 5,	//统计
	Reset = 6,		//重置阶段
	FightEnd = 7,	//战斗结束
};

class AIMgr;
class Skill;
class SkillEffectAnim;

class DlgFight : public DlgBase
{
public:
	DlgFight();
	virtual ~DlgFight();
	CREATE_DLG_FUNC(DlgFight);
	virtual bool init(StateBase* gameState);
	virtual void load();
	void update(float dt);

	void addTouch();
	bool onTouchBegan(Touch* pTouch, Event* pEvent);
	void onTouchMoved(Touch* pTouch, Event* pEvent);
	void onTouchEnded(Touch* pTouch, Event* pEvent);
public:
	virtual DlgBase* showDlg(const string& dlgName);
	virtual void hideDlg(const string& dlgName);

public:
	void setObjPosition();
	void addHero(Vec2 pos, int camp, ValueMap* objInfo);
	void addSoilder(Vec2 pos, int camp, ValueMap*objInfo);
	void addPlayer(Vec2 pos, int camp);

	BaseSprite* selectObj(const Vec2& pos);
	void setObjPos(const Vec2& pos);
	void saveObjPos();

	void setAttrText(Text* txt, int diffNum);
	void updateObjAttrLayer();
	
	void triggerSkill(SkillTriggerType tt);

	void showSkillRange(bool isShow, BaseSprite* obj);

	void addSkillAnim(void* data);
	void delSkillAnim(void* data);

	void setFightType(int type);
protected:
	virtual void onMapTouch(Ref* sender, Widget::TouchEventType type);
	virtual void onMapTouchBegan(Ref* sender, Widget::TouchEventType type);
	virtual void onMapTouchMove(Ref* sender, Widget::TouchEventType type);
	virtual void onMapTouchEnd(Ref* sender, Widget::TouchEventType type);

	
	void onNextRound(Ref* sender, Widget::TouchEventType type);
	void onStart(Ref* sender, Widget::TouchEventType type);
	void onClose(Ref* sender, Widget::TouchEventType type);
	void onResetObjPos(Ref* sender, Widget::TouchEventType type);
public:
	int _player_id;
	map<int, ValueMap>* _objPosCfg;
	AIMgr* _ai;
	int _round;

	MapState _state;//0:等待，1:准备，2：战斗，3:等待结束，4，5，6
	
	Layout* _lay_floor;
	Layout* _lay_fight;
	Layout* _lay_sky;
	//BaseSprite* _select_obj;
	Vec2	_obj_pos_before_move;

	
	Layout*		lay_wait;
	Text*		lab_round;

	Layout*		lay_result;
	Text*		txt_result;
	Text*		txt_num;
	Button*		btn_start;
	ImageView*	img_lose;
	ImageView*	img_win;
	ImageView*	img_tip;
	ImageView*	lay_line;

	Layout*		lay_attr;		
	Text*		txt_name;		
	Text*		txt_hp;
	Text*		txt_damage;	
	Text*		txt_def;		
	Text*		txt_speed;		
	Text*		txt_shoot_range;		
	Text*		txt_xixue;		
	Text*		txt_hurt_more; 

	Text*		txt_hp_diff;		
	Text*		txt_damage_diff;
	Text*		txt_def_diff;		
	Text*		txt_speed_diff;
	Text*		txt_shoot_range_diff;	

	DrawNode*	_skill_shoot_range;
	DrawNode*	_skill_radius;

	ValueVector _setting_data;

	map<int, SkillEffectAnim*> _floor_aminas;
};

