#pragma once

#include "Utils/PublicDefault.h"
#include "BaseComponentDef.h"

class DlgBase;

class BaseComponent : public Layout
{
public:
	BaseComponent();
	CREATE_COMPONENT_FUNC(BaseComponent);
	virtual ~BaseComponent();
	virtual bool init(DlgBase* dlg);
	virtual void load();

public:	
	virtual void updateUI();

protected:
	DlgBase* hostDlg;
};
