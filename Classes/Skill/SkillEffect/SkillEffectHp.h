#pragma once

#include "SkillEffect.h"

class SkillEffectHp : public SkillEffect{

public:
	SkillEffectHp(BaseSprite* obj, int skillEffectId, BaseSprite* caster);
	~SkillEffectHp();

protected:
	virtual bool trigger();

	virtual void end();

};