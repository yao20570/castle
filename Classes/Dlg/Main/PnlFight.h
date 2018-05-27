#pragma once

#include "Core/BaseComponent.h"
#include "Core/BaseComponentDef.h"



class PnlFight : public BaseComponent
{
public:
	PnlFight();
	virtual ~PnlFight();
	CREATE_COMPONENT_FUNC(PnlFight);
	virtual bool init(DlgBase* dlg);
	virtual void load();
		
	virtual void setVisible(bool b);
public:
	virtual void updateUI();
	void updateSettingPanel();

	void onStart(Ref* sender, Widget::TouchEventType type);
private:

private:

	Layout*			pnl_setting;
	Layout*			pnl_start;

};