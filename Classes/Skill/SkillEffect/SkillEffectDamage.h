#pragma once

#include "SkillEffect.h"

class SkillEffectDamage : public SkillEffect{

public:
	SkillEffectDamage(BaseSprite* obj, int skillEffectId, BaseSprite* caster);
	~SkillEffectDamage();

protected:
	virtual bool trigger();

	virtual void end();

};