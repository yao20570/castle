#pragma once

#include "SkillEffect.h"

class SkillEffectShootRange : public SkillEffect{

public:
	SkillEffectShootRange(BaseSprite* obj, int skillEffectId, BaseSprite* caster);
	~SkillEffectShootRange();

protected:
	virtual bool trigger();

	virtual void end();

};