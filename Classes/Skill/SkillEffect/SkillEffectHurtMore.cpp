
#include "SkillEffectHurtMore.h"
#include "Model/BaseSprite.h"
#include "Utils/GlobalManager.h"

SkillEffectHurtMore::SkillEffectHurtMore(BaseSprite* obj, int skillEffectId, BaseSprite* caster)
	:SkillEffect(obj, skillEffectId, caster)
{
	//³¢ÊÔ´¥·¢
	this->onTrigger();
}

SkillEffectHurtMore::~SkillEffectHurtMore(){
	end();
}

bool SkillEffectHurtMore::trigger(){
	this->m_obj->_hurt_more = this->m_obj->_hurt_more + this->m_value1;
	return true;
}

void SkillEffectHurtMore::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

	int value = this->m_value1 * this->m_curTimes; 

	this->m_obj->_hurt_more = this->m_obj->_hurt_more - value;
}