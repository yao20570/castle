#pragma once
#include "cocos2d.h"

#include "../DlgBase.h"
#include "../DlgDefine.h"

class DlgMain : public DlgBase
{
public:
	DlgMain();
	virtual ~DlgMain();
	CREATE_DLG_FUNC(DlgMain);
	virtual bool init(StateBase* gameState);
	virtual void load();

public:
	virtual void showDlg(const string& dlgName);
	virtual void hideDlg(const string& dlgName);
	
private:
	void onLord(Ref* sender, Widget::TouchEventType type);
	void onSearch(Ref* sender, Widget::TouchEventType type);
	void onFight(Ref* sender, Widget::TouchEventType type);
	void onSetting(Ref* sender, Widget::TouchEventType type);
	void onChapter(Ref* sender, Widget::TouchEventType type);
};
