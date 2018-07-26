#pragma once

#include "SkillEffect.h"

class SkillEffectDamage : public SkillEffect{

public:
	SkillEffectDamage(BaseSprite* obj, int skillEffectId);
	~SkillEffectDamage();

protected:
	virtual void trigger();

	virtual void end();

};