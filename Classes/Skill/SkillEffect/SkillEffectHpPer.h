#pragma once

#include "SkillEffect.h"

class SkillEffectHpPer : public SkillEffect{

public:
	SkillEffectHpPer(BaseSprite* obj, int skillEffectId);
	~SkillEffectHpPer();

protected:
	virtual void trigger();

	virtual void end();

};