#pragma once
#include "cocos2d.h"

#include "../DlgBase.h"
#include "../DlgDefine.h"

class DlgFight : public DlgBase
{
public:
	DlgFight();
	virtual ~DlgFight();
	CREATE_DLG_FUNC(DlgFight);
	virtual bool init(StateBase* gameState);
	virtual void load();

public:
	virtual void showDlg(const string& dlgName);
	virtual void hideDlg(const string& dlgName);

private:
	//void onLord(Ref* sender, Widget::TouchEventType type);
	//void onSearch(Ref* sender, Widget::TouchEventType type);
	//void onFight(Ref* sender, Widget::TouchEventType type);
	//void onSetting(Ref* sender, Widget::TouchEventType type);
	//void onChapter(Ref* sender, Widget::TouchEventType type);
};

