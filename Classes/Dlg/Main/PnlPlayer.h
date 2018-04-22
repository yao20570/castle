#pragma once

#include "Core/BaseComponent.h"
#include "Core/BaseComponentDef.h"

class PnlPlayer : public BaseComponent
{
public:
	PnlPlayer();
	virtual ~PnlPlayer();
	CREATE_COMPONENT_FUNC(PnlPlayer);
	virtual bool init(DlgBase* dlg);
	virtual void load();

public:
	virtual void updateUI();
	void updateSelectList();
	void updateSettingPanel();

	void onCloseUI(Ref* sender, Widget::TouchEventType type);
	void onSelectEquip(Ref* sender, Widget::TouchEventType type);
private:
	
private:	
	ScrollView* sv_equip;
	Layout*		pnl_setting;
	Layout*		pnl_part[6];
	
};