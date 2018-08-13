#pragma once

#include "SkillEffect.h"

class SkillEffectYun : public SkillEffect{

public:
	SkillEffectYun(BaseSprite* obj, int skillEffectId, BaseSprite* caster);
	~SkillEffectYun();

protected:
	virtual bool trigger();

	virtual void end();

};