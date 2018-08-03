
#include "SkillEffectSpeed.h"
#include "Model/BaseSprite.h"


SkillEffectSpeed::SkillEffectSpeed(BaseSprite* obj, int skillEffectId, BaseSprite* caster)
	:SkillEffect(obj, skillEffectId, caster)
{
	//³¢ÊÔ´¥·¢
	this->onTrigger();
}

SkillEffectSpeed::~SkillEffectSpeed(){
	end();
}

void SkillEffectSpeed::trigger(){
	this->m_obj->_speed_diff = this->m_obj->_speed_diff + this->m_value1;
}

void SkillEffectSpeed::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

	int value = this->m_obj->_speed_diff * this->m_curTimes; 

	this->m_obj->_speed_diff = this->m_obj->_speed_diff - value;
}