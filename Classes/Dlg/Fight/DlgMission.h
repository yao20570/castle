#pragma once
#pragma once
#include "cocos2d.h"

#include "../../Core/DlgBase.h"
#include "../../Core/DlgDefine.h"
#include "../../Dlg/Fight/DlgFight.h"
class AIMgr;

class DlgMission : public DlgFight
{
public:
	DlgMission();
	virtual ~DlgMission();
	CREATE_DLG_FUNC(DlgMission);
	virtual bool init(StateBase* gameState);
	virtual void load();
	void update(float dt);

public:
	virtual DlgBase* showDlg(const string& dlgName);
	virtual void hideDlg(const string& dlgName);

public:
	void setObjPosition(int missionSubId, int tempKey);
	
private:

	void onStart(Ref* sender, Widget::TouchEventType type);
	void onClose(Ref* sender, Widget::TouchEventType type);
	void onResetObjPos(Ref* sender, Widget::TouchEventType type);
public:
	map<int, ValueMap>* _objPosCfg;

	BaseSprite* _select_obj;

	Layout* lay_result;
	Text*	txt_result;

	int _missionSubId;
	int _tempKey;
	int _round;
};

