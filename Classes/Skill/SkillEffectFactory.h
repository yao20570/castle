#pragma once

#include "Utils/Public.h"
#include "Skill/SkillDefine.h"
#include "Skill/SkillEffect/SkillEffect.h"

class SkillEffectFactory : public cocos2d::Ref
{	    
private:
    static SkillEffectFactory* _g;
public:
    SkillEffectFactory();
    virtual ~SkillEffectFactory();
    virtual bool init();
    static SkillEffectFactory* getInstance();

	SkillEffect* newSkillEffect(BaseSprite* obj, int effectId);
	SkillEffectType getEffectTypeById(int effectId);
};


#define SEF() SkillEffectFactory::getInstance()