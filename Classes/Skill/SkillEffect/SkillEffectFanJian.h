#pragma once

#include "SkillEffect.h"

class SkillEffectFanJian : public SkillEffect{

public:
	SkillEffectFanJian(BaseSprite* obj, int skillEffectId, BaseSprite* caster);
	~SkillEffectFanJian();

protected:
	virtual bool trigger();

	virtual void end();

};