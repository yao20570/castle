#pragma once

#include "Utils/Public.h"

class  DlgMain;
class  ClassFactory;

class StatePlay : public cocos2d::Layer
{
public:
	StatePlay();
	virtual ~StatePlay();
	CREATE_FUNC(StatePlay);
	virtual bool init();
	void load();

	

public:
	void show();
	void hide();

	void regDlg();
	Node* creataeDlg(const string& dlgName);
	void showDlg(const string& dlgName);
	void hideDlg();

private:
	bool _is_init;
	Node* _cur_dlg;
	std::vector<string> _open_dlgs;
	ClassFactory* _class_factory;
};
