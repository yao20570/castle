#pragma once

#include "cocos2d.h"

#include "Core/BaseComponent.h"
#include "Core/BaseComponentDef.h"

class PnlSetting : public BaseComponent
{
public:
	PnlSetting();
	virtual ~PnlSetting();
	CREATE_COMPONENT_FUNC(PnlSetting);
	virtual bool init(DlgBase* dlg);
	virtual void load();

public:
	virtual void updateUI();
	void updateSelectList();
	void updateSettingPanel();

private:

	void selectSetting(int selectId);
	void selectObj(int selectId);

	void isObjInSettingPnl(int id);
	
private:
	void addTouch();
	bool onTouchBegan(Touch* pTouch, Event* pEvent);
	void onTouchMoved(Touch* pTouch, Event* pEvent);
	void onTouchEnded(Touch* pTouch, Event* pEvent);
	void onTouchCancelled(Touch* pTouch, Event* pEvent);

	void onOpenPlayerUI(Ref* sender, Widget::TouchEventType type);
	void onSelectSetting(Ref* sender, Widget::TouchEventType type);
	void onSaveSetting(Ref* sender, Widget::TouchEventType type);
	void onSelectObj(Ref* sender, Widget::TouchEventType type);
	void onSelectPnlSettingObj(Ref* sender, Widget::TouchEventType type);
	
private:
	ScrollView*	sv_setting;
	ScrollView* sv_obj;
	Text*		txt_num;
	Layout*		pnl_setting;
	Layout*		pnl_player;
	Layout*		pnl_drag;
	Layout*		pnl_drag_container;
	Layout*		pnls[3];
	Layout*		pnl_save;
	Layout*		pnl_obj_select;

	int			select_fight_id;
	int			select_obj_id;
	int			select_setting_id;
	vector<int> setting_objs;
	vector<int> waiting_objs;
	Vec2		drag_start_pos;
};