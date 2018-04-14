

#include "BaseComponent.h"


USING_NS_CC;


BaseComponent::BaseComponent()	
{
	Layer::Layer();
}

BaseComponent::~BaseComponent()
{

}

bool BaseComponent::init()
{
	//////////////////////////////
	// 1. super init first
	//if (!PanZoomLayer::init())
	if (!Layout::init())
	{
		return false;
	}

	this->load();
	return true;
}

void BaseComponent::load()
{
}

void BaseComponent::updateUI()
{
	
}
