#pragma once

#include "SkillEffect.h"

class SkillEffectDamagePer : public SkillEffect{

public:
	SkillEffectDamagePer(BaseSprite* obj, int skillEffectId, BaseSprite* caster);
	~SkillEffectDamagePer();

protected:
	virtual bool trigger();

	virtual void end();

};