
#include "SkillEffectCritRate.h"
#include "Model/BaseSprite.h"


SkillEffectCritRate::SkillEffectCritRate(BaseSprite* obj, int skillEffectId, BaseSprite* caster)
	:SkillEffect(obj, skillEffectId, caster)
{
	//³¢ÊÔ´¥·¢
	this->onTrigger();
}

SkillEffectCritRate::~SkillEffectCritRate(){
	end();
}

void SkillEffectCritRate::trigger(){
	this->m_obj->_crit_rate = this->m_obj->_crit_rate + this->m_value1;
}

void SkillEffectCritRate::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

	int value = this->m_obj->_crit_rate * this->m_curTimes; 

	this->m_obj->_crit_rate = this->m_obj->_crit_rate - value;
}