
#include "SkillEffectDef.h"
#include "Model/BaseSprite.h"


SkillEffectDef::SkillEffectDef(BaseSprite* obj, int skillEffectId, BaseSprite* caster)
	:SkillEffect(obj, skillEffectId, caster)
{
	//³¢ÊÔ´¥·¢
	this->onTrigger();
}

SkillEffectDef::~SkillEffectDef(){
	end();
}

void SkillEffectDef::trigger(){
	this->m_obj->_def_diff = this->m_obj->_def_diff + this->m_value1;
}

void SkillEffectDef::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

	int value = this->m_value1 * this->m_curTimes; 

	this->m_obj->_def_diff = this->m_obj->_def_diff - value;
}