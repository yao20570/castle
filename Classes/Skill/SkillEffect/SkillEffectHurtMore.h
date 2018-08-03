#pragma once

#include "SkillEffect.h"

class SkillEffectHurtMore : public SkillEffect{

public:
	SkillEffectHurtMore(BaseSprite* obj, int skillEffectId, BaseSprite* caster);
	~SkillEffectHurtMore();

protected:
	virtual void trigger();

	virtual void end();

};