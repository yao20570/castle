#pragma once

#include "SkillEffect.h"

class SkillEffectDisperse : public SkillEffect{

public:
	SkillEffectDisperse(BaseSprite* obj, int skillEffectId, BaseSprite* caster);
	~SkillEffectDisperse();

protected:
	virtual void trigger();

	virtual void end();

};