#pragma once

#include "SkillEffect.h"

class SkillEffectXiXue : public SkillEffect{

public:
	SkillEffectXiXue(BaseSprite* obj, int skillEffectId);
	~SkillEffectXiXue();

protected:
	virtual void trigger();

	virtual void end();

};