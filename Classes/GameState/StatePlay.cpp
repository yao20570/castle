
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocostudio;
using namespace cocos2d::ui;

#include "StatePlay.h"
#include "Dlg/Main/DlgMain.h"

USING_NS_CC;

StatePlay::StatePlay()
	:_is_init(false)
{
}

StatePlay::~StatePlay()
{

}

bool StatePlay::init()
{
	if (this->_is_init == true) {
		return true;
	}
	this->_is_init = true;

	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	this->load();


	

	return true;
}

void StatePlay::load() 
{
	this->_main_layer = DlgMain::create();
	this->addChild(this->_main_layer, 1);
}

void StatePlay::show()
{
	this->setVisible(true);
}

void StatePlay::hide()
{
	this->setVisible(false);
}