#pragma once

#include "SkillEffect.h"

class SkillEffectShootRange : public SkillEffect{

public:
	SkillEffectShootRange(BaseSprite* obj, int skillEffectId, BaseSprite* caster);
	~SkillEffectShootRange();

protected:
	virtual void trigger();

	virtual void end();

};