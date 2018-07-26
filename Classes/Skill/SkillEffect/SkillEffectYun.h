#pragma once

#include "SkillEffect.h"

class SkillEffectYun : public SkillEffect{

public:
	SkillEffectYun(BaseSprite* obj, int skillEffectId);
	~SkillEffectYun();

protected:
	virtual void trigger();

	virtual void end();

};