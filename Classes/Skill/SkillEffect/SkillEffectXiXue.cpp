
#include "SkillEffectXiXue.h"
#include "Model/BaseSprite.h"


SkillEffectXiXue::SkillEffectXiXue(BaseSprite* obj, int skillEffectId, BaseSprite* caster)
	:SkillEffect(obj, skillEffectId, caster)
{
	//����һ��Ч��
	this->onTrigger();
}

SkillEffectXiXue::~SkillEffectXiXue(){
	end();
}

void SkillEffectXiXue::trigger(){
	this->m_obj->_xixue = this->m_obj->_xixue + this->m_value1;
}

void SkillEffectXiXue::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

	int value = this->m_obj->_damage_diff * this->m_curTimes; 

	this->m_obj->_damage_diff = this->m_obj->_damage_diff - value;
}