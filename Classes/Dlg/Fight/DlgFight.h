#pragma once
#include "cocos2d.h"

class DlgFight : public cocos2d::Layer
{
public:
	DlgFight();
	virtual ~DlgFight();
	CREATE_FUNC(DlgFight);
	virtual bool init();
	void load();

public:
	void show();
	void hide();

	void onLord(Ref* sender, Widget::TouchEventType type);
	void onSearch(Ref* sender, Widget::TouchEventType type);
	void onFight(Ref* sender, Widget::TouchEventType type);
	void onSetting(Ref* sender, Widget::TouchEventType type);
	void onChapter(Ref* sender, Widget::TouchEventType type);

private:
	bool _is_init;
	DlgFight* _main_layer;
};
