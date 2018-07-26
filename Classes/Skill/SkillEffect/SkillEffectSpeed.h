#pragma once

#include "SkillEffect.h"

class SkillEffectSpeed : public SkillEffect{

public:
	SkillEffectSpeed(BaseSprite* obj, int skillEffectId);
	~SkillEffectSpeed();

protected:
	virtual void trigger();

	virtual void end();

};