#pragma once

#include "SkillEffect.h"

class SkillEffectSpeedPer : public SkillEffect{

public:
	SkillEffectSpeedPer(BaseSprite* obj, int skillEffectId, BaseSprite* caster);
	~SkillEffectSpeedPer();

protected:
	virtual void trigger();

	virtual void end();

};