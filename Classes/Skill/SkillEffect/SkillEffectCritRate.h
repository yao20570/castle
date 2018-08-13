#pragma once

#include "SkillEffect.h"

class SkillEffectCritRate : public SkillEffect{

public:
	SkillEffectCritRate(BaseSprite* obj, int skillEffectId, BaseSprite* caster);
	~SkillEffectCritRate();

protected:
	virtual bool trigger();

	virtual void end();

};