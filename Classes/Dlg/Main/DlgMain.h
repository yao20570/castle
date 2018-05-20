#pragma once
#include "cocos2d.h"

#include "../../Core/DlgBase.h"
#include "../../Core/DlgDefine.h"

class BaseComponent;

class DlgMain : public DlgBase
{
public:
	DlgMain();
	virtual ~DlgMain();
	CREATE_DLG_FUNC(DlgMain);
	virtual bool init(StateBase* gameState);
	virtual void load();

	BaseComponent* getPanel(PanelType type);
	void showPanel(PanelType type);
public:
	virtual DlgBase* showDlg(const string& dlgName);
	virtual void hideDlg(const string& dlgName);
	
private:
	void onLord(Ref* sender, Widget::TouchEventType type);
	void onSearch(Ref* sender, Widget::TouchEventType type);
	void onFight(Ref* sender, Widget::TouchEventType type);
	void onSetting(Ref* sender, Widget::TouchEventType type);
	void onChapter(Ref* sender, Widget::TouchEventType type);
public:
	Layout* _lay_mid;
	Layout* _pnl_setting;
	map<int, BaseComponent*> _pnls;
	BaseComponent*	_cur_pnl;
};
