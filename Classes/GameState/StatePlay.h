#pragma once

#include "Utils/PublicDefault.h"
#include "Core/StateBase.h"

class  DlgMain;
class  ClassFactory;

class StatePlay : public StateBase
{
public:
	StatePlay();
	virtual ~StatePlay();
	CREATE_FUNC(StatePlay);
	virtual bool init();
	virtual void load();

public:
	virtual void regDlg();
	virtual void showDlg(const string& dlgName);
	virtual void hideDlg(const string& dlgName);
};
