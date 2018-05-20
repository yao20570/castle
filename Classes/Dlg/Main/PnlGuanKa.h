#pragma once

#include "Core/BaseComponent.h"
#include "Core/BaseComponentDef.h"

class PnlGuanKa : public BaseComponent
{
public:
	PnlGuanKa();
	virtual ~PnlGuanKa();
	CREATE_COMPONENT_FUNC(PnlGuanKa);
	virtual bool init(DlgBase* dlg);
	virtual void load();

public:
	virtual void updateUI();
	
	void onSelectMissionSub(Ref* sender, Widget::TouchEventType type);
	void onStart(Ref* sender, Widget::TouchEventType type);
private:

private:
	ScrollView * sv_guanka;
	Layout * pnl_start;
	Layout* lay_select;
	vector<Layout*> layouts;
};