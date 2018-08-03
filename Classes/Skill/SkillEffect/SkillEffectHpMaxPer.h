#pragma once

#include "SkillEffect.h"

class SkillEffectHpMaxPer : public SkillEffect{

public:
	SkillEffectHpMaxPer(BaseSprite* obj, int skillEffectId, BaseSprite* caster);
	~SkillEffectHpMaxPer();

protected:
	virtual void trigger();

	virtual void end();

};