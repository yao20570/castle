#pragma once

#include "Core/BaseComponent.h"
#include "Core/BaseComponentDef.h"

class PnlSetting : public BaseComponent
{
public:
	PnlSetting();
	virtual ~PnlSetting();
	CREATE_COMPONENT_FUNC(PnlSetting);
	virtual bool init();
	virtual void load();
	

};