
#include "SkillEffectDamagePer.h"
#include "Model/BaseSprite.h"


SkillEffectDamagePer::SkillEffectDamagePer(BaseSprite* obj, int skillEffectId, BaseSprite* caster)
	:SkillEffect(obj, skillEffectId, caster)
{
	//³¢ÊÔ´¥·¢
	this->onTrigger();
}

SkillEffectDamagePer::~SkillEffectDamagePer(){
	end();
}

void SkillEffectDamagePer::trigger(){
	this->m_obj->_damage_per = this->m_obj->_damage_per + this->m_value1;
}

void SkillEffectDamagePer::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

	int valur = this->m_value1 * this->m_curTimes;
	this->m_obj->_damage_per = this->m_obj->_damage_per - valur;
}