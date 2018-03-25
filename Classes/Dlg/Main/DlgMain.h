#pragma once
#include "cocos2d.h"

class DlgMain : public cocos2d::Layer
{
public:
	DlgMain();
	virtual ~DlgMain();
	CREATE_FUNC(DlgMain);
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
	DlgMain* _main_layer;
};
