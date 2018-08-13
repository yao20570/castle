
#include "SkillEffectYun.h"
#include "Model/BaseSprite.h"


SkillEffectYun::SkillEffectYun(BaseSprite* obj, int skillEffectId, BaseSprite* caster)
	:SkillEffect(obj, skillEffectId, caster)
{
	//³¢ÊÔ´¥·¢
	this->onTrigger();
}

SkillEffectYun::~SkillEffectYun(){
	end();
}

bool SkillEffectYun::trigger(){
	this->m_obj->setState(STATE_YUN, this->m_obj->getDir());
	return true;
}

void SkillEffectYun::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

	this->m_obj->setState(STATE_RUN, this->m_obj->getDir());
}