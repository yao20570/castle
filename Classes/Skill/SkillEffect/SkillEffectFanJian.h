#pragma once

#include "SkillEffect.h"

class SkillEffectFanJian : public SkillEffect{

public:
	SkillEffectFanJian(BaseSprite* obj, int skillEffectId);
	~SkillEffectFanJian();

protected:
	virtual void trigger();

	virtual void end();

};