#pragma once

#include "SkillEffect.h"

class SkillEffectDef : public SkillEffect{

public:
	SkillEffectDef(BaseSprite* obj, int skillEffectId, BaseSprite* caster);
	~SkillEffectDef();

protected:
	virtual void trigger();

	virtual void end();

};