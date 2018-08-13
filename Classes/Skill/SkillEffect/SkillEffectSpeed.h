#pragma once

#include "SkillEffect.h"

class SkillEffectSpeed : public SkillEffect{

public:
	SkillEffectSpeed(BaseSprite* obj, int skillEffectId, BaseSprite* caster);
	~SkillEffectSpeed();

protected:
	virtual bool trigger();

	virtual void end();

};