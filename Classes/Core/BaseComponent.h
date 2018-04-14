#pragma once

#include "Utils/PublicDefault.h"
#include "BaseComponentDef.h"

class BaseComponent : public Layout
{
public:
	BaseComponent();
	CREATE_COMPONENT_FUNC(BaseComponent);
	virtual ~BaseComponent();
	virtual bool init();
	virtual void load();

public:	
	virtual void updateUI();

protected:

};
