#pragma once

#include "Utils/Public.h"

class ClassFactory;
class DlgBase;

class StateBase : public cocos2d::Layer
{
public:
	StateBase();
	virtual ~StateBase();
	CREATE_FUNC(StateBase);
	virtual bool init();
	virtual void load();

public:
	virtual void regDlg();
	virtual void showDlg(const string& dlgName);
	virtual void hideDlg(const string& dlgName);

protected:
	void addDlg(DlgBase* dlg);

private:
	DlgBase* creataeDlg(const string& dlgName);
	

protected:
	bool _is_init;
	DlgBase* _cur_dlg;
	vector<string> _open_dlgs;
	ClassFactory* _class_factory;
	vector<Layer*> _layers;
};