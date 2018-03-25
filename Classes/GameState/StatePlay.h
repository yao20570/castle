#pragma once


#include "cocos2d.h"

class  DlgMain;

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

private:
	bool _is_init;
	DlgMain* _main_layer;
};
