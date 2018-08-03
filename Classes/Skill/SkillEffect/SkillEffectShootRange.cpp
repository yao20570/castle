
#include "SkillEffectShootRange.h"
#include "Model/BaseSprite.h"


SkillEffectShootRange::SkillEffectShootRange(BaseSprite* obj, int skillEffectId, BaseSprite* caster)
	:SkillEffect(obj, skillEffectId, caster)
{
	//³¢ÊÔ´¥·¢
	this->onTrigger();
}

SkillEffectShootRange::~SkillEffectShootRange(){
	end();
}

void SkillEffectShootRange::trigger(){
	this->m_obj->_shootRange = this->m_obj->_shootRange + this->m_value1;
}

void SkillEffectShootRange::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

	this->m_obj->_shootRange = this->m_obj->_shootRange - this->m_value1;
}