#pragma once

#include "Core/BaseComponent.h"
#include "Core/BaseComponentDef.h"

class PnlHero : public BaseComponent
{
public:
	PnlHero();
	virtual ~PnlHero();
	CREATE_COMPONENT_FUNC(PnlHero);
	virtual bool init(DlgBase* dlg);
	virtual void load();

public:
	virtual void updateUI();
	void updateSvHero();
	void updateUpgradePanel(int id, bool isNext);


	void onOpenUpgradePanel(Ref* sender, Widget::TouchEventType type);
	void onCloseUpgradePanel(Ref* sender, Widget::TouchEventType type);
	void onUpgrade(Ref* sender, Widget::TouchEventType type);
	void onOk(Ref* sender, Widget::TouchEventType type);
	void onCancel(Ref* sender, Widget::TouchEventType type);
private:

private:
	ScrollView*		sv_hero;
	Layout*			pnl_btn_close;
	Layout*			pnl_upgrade;
	Layout*			pnl_btn_upgrade;
	Layout*			pnl_btn_ok;
	Layout*			pnl_btn_cancel;

	int		obj_id;

	ImageView* img_half;
	Text*	txt_level;
	Text*	txt_name;
	Text*	lab_attr_1;
	Text*	lab_attr_2;
	Text*	lab_attr_3;
	Text*	lab_attr_4;
	Text*	lab_attr_5;
	Text*	lab_attr_6;
	Text*	lab_attr_7;
	Text*	lab_attr_8;
	Text*	lab_attr_9;
	Text*	lab_skill_1;
	Text*	lab_skill_2;
	Text*	lab_skill_3;


	Text*	lab_attr_4_0;
	Text*	lab_attr_5_0;
	Text*	lab_attr_7_0;
};