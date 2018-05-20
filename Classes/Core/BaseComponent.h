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

	virtual DlgBase* showDlg(const string& dlgName);
	virtual void hideDlg(const string& dlgName);
public:	
	virtual void updateUI();
	virtual void showTip(char* tip);
	virtual void showMessage(char* message, std::function<void()>& sureCallback, std::function<void()>& cancelCallback);
protected:
	DlgBase* hostDlg;
};
