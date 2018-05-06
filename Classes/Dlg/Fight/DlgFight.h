#pragma once
#include "cocos2d.h"

#include "../../Core/DlgBase.h"
#include "../../Core/DlgDefine.h"

class AIMgr;

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
	virtual void showDlg(const string& dlgName);
	virtual void hideDlg(const string& dlgName);

public:
	void setObjPosition();
	void addHero(int heroId, Vec2 pos, int camp);
	void addSoilder(int soilderId, Vec2 pos, int camp);
	void addPlayer(Vec2 pos, int camp);

private:
	//void onLord(Ref* sender, Widget::TouchEventType type);
	//void onSearch(Ref* sender, Widget::TouchEventType type);
	//void onFight(Ref* sender, Widget::TouchEventType type);
	//void onSetting(Ref* sender, Widget::TouchEventType type);
	void onStart(Ref* sender, Widget::TouchEventType type);
	void onClose(Ref* sender, Widget::TouchEventType type);
	void onResetObjPos(Ref* sender, Widget::TouchEventType type);
public:
	map<int, ValueMap>* _objPosCfg;
	AIMgr* _ai;

	Layout* _map;
	BaseSprite* _select_obj;

	Layout* lay_result;
	Text*	txt_result;

	int _round;
};

