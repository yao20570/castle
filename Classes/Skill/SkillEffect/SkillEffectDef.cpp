
#include "SkillEffectDef.h"
#include "Model/BaseSprite.h"


SkillEffectDef::SkillEffectDef(BaseSprite* obj, int skillEffectId)
	:SkillEffect(obj, skillEffectId)
{
	//³¢ÊÔ´¥·¢
	this->onTrigger();
}

SkillEffectDef::~SkillEffectDef(){
	end();
}

void SkillEffectDef::trigger(){
	this->m_obj->_def_diff = this->m_obj->_def_diff + this->m_value;
}

void SkillEffectDef::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

	int value = this->m_obj->_def_diff * this->m_curTimes; 

	this->m_obj->_def_diff = this->m_obj->_def_diff - value;
}