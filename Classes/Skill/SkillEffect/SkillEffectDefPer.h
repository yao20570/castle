#pragma once

#include "SkillEffect.h"

class SkillEffectDefPer : public SkillEffect{

public:
	SkillEffectDefPer(BaseSprite* obj, int skillEffectId, BaseSprite* caster);
	~SkillEffectDefPer();

protected:
	virtual bool trigger();

	virtual void end();

};