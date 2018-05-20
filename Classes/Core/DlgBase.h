#pragma once

#include "Utils/PublicDefault.h"
#include "StateBase.h"
#include "DlgEnum.h"
#include "DlgDefine.h"

class StateBase;

class DlgBase : public cocos2d::Layer
{
public:
	DlgBase();
	//static DlgBase* create(StateBase* gameState, ENUM_DLG_TYPE dlgType);
	CREATE_DLG_FUNC(DlgBase);
	virtual ~DlgBase();
	virtual bool init(StateBase* gameState);
	virtual void load();
	ENUM_DLG_TYPE getDlgType();
	string getDlgName();

public:
	virtual DlgBase* showDlg(const string& dlgName);
	virtual void hideDlg(const string& dlgName);

	virtual void showTip(char* tip);
	virtual void showMessage(char* message, std::function<void()>& sureCallback, std::function<void()>& cancelCallback);
protected:
	bool _is_init;
	StateBase* _game_state;
	ENUM_DLG_TYPE _dlg_type;
	string _dlg_name;
};
