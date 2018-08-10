
#include "SkillEffectDamage.h"
#include "Model/BaseSprite.h"


SkillEffectDamage::SkillEffectDamage(BaseSprite* obj, int skillEffectId, BaseSprite* caster)
	:SkillEffect(obj, skillEffectId, caster)
{
	//³¢ÊÔ´¥·¢
	this->onTrigger();
}

SkillEffectDamage::~SkillEffectDamage(){
	end();
}

void SkillEffectDamage::trigger(){
	this->m_obj->_damage_diff = this->m_obj->_damage_diff + this->m_value1;
}

void SkillEffectDamage::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

	int value = this->m_value1 * this->m_curTimes; 

	this->m_obj->_damage_diff = this->m_obj->_damage_diff - value;

}