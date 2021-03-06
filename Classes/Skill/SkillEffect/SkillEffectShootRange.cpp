
#include "SkillEffectShootRange.h"
#include "Model/BaseSprite.h"


SkillEffectShootRange::SkillEffectShootRange(BaseSprite* obj, int skillEffectId, BaseSprite* caster)
	:SkillEffect(obj, skillEffectId, caster)
{
	//���Դ���
	this->onTrigger();
}

SkillEffectShootRange::~SkillEffectShootRange(){
	end();
}

bool SkillEffectShootRange::trigger(){
	this->m_obj->_shoot_range_diff = this->m_obj->_shoot_range_diff + this->m_value1;
	return true;
}

void SkillEffectShootRange::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

	this->m_obj->_shoot_range_diff = this->m_obj->_shoot_range_diff - this->m_value1;
}