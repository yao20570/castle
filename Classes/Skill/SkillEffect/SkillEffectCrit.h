#pragma once

#include "SkillEffect.h"

class SkillEffectCrit : public SkillEffect{

public:
	SkillEffectCrit(BaseSprite* obj, int skillEffectId, BaseSprite* caster);
	~SkillEffectCrit();

protected:
	virtual bool trigger();

	virtual void end();

};