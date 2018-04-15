#pragma once

#include "Utils/PublicDefault.h"

#include "Core/BaseComponent.h"
#include "Core/BaseComponentDef.h"

class PnlSetting : public BaseComponent
{
public:
	PnlSetting();
	virtual ~PnlSetting();
	CREATE_COMPONENT_FUNC(PnlSetting);
	virtual bool init();
	virtual void load();

public:
	virtual void updateUI();
	void updateSelectList();
	void updateSettingPanel();

private:
	void selectSetting(int selectId);
	void selectObj(int selectId);
	
private:
	void addTouch();
	bool onTouchBegan(Touch* pTouch, Event* pEvent);
	void onTouchMoved(Touch* pTouch, Event* pEvent);
	void onTouchEnded(Touch* pTouch, Event* pEvent);

	void onSelectSetting(Ref* sender, Widget::TouchEventType type);
	void onSaveSetting(Ref* sender, Widget::TouchEventType type);
	void onSelectObj(Ref* sender, Widget::TouchEventType type);
private:
	ScrollView*	sv_setting;
	ScrollView* sv_obj;
	Label*		txt_num;
	Layout*		pnl_setting;
	Layout*		pnl_drag;
	Layout*		pnl_drag_container;
	Layout*		pnls[3];
	int			select_obj_id;
	int			select_setting_id;
	vector<int> setting_objs;
	Vec2		drag_start_pos;
};