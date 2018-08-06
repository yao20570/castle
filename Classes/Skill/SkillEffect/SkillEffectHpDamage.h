#pragma once

#include "SkillEffect.h"

class SkillEffectHpDamage : public SkillEffect{

public:
	SkillEffectHpDamage(BaseSprite* obj, int skillEffectId, BaseSprite* caster);
	~SkillEffectHpDamage();

protected:
	virtual void trigger();

	virtual void end();

};