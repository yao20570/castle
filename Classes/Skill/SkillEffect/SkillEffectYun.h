#pragma once

#include "SkillEffect.h"

class SkillEffectYun : public SkillEffect{

public:
	SkillEffectYun(BaseSprite* obj, int skillEffectId, BaseSprite* caster);
	~SkillEffectYun();

protected:
	virtual void trigger();

	virtual void end();

};