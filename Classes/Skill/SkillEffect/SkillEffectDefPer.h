#pragma once

#include "SkillEffect.h"

class SkillEffectDefPer : public SkillEffect{

public:
	SkillEffectDefPer(BaseSprite* obj, int skillEffectId);
	~SkillEffectDefPer();

protected:
	virtual void trigger();

	virtual void end();

};