
#include "SkillEffectCrit.h"
#include "Model/BaseSprite.h"


SkillEffectCrit::SkillEffectCrit(BaseSprite* obj, int skillEffectId, BaseSprite* caster)
	:SkillEffect(obj, skillEffectId, caster)
{
	//³¢ÊÔ´¥·¢
	this->onTrigger();
}

SkillEffectCrit::~SkillEffectCrit(){
	end();
}

bool SkillEffectCrit::trigger(){
	this->m_obj->_crit = this->m_obj->_crit + this->m_value1;

	return true;
}

void SkillEffectCrit::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

	int value = this->m_obj->_crit * this->m_curTimes; 

	this->m_obj->_crit = this->m_obj->_crit - value;
}