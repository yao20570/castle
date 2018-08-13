#pragma once

#include "SkillEffect.h"

class SkillEffectXiXue : public SkillEffect{

public:
	SkillEffectXiXue(BaseSprite* obj, int skillEffectId, BaseSprite* caster);
	~SkillEffectXiXue();

protected:
	virtual bool trigger();

	virtual void end();

};