
#include "SkillEffectHurtMore.h"
#include "Model/BaseSprite.h"


SkillEffectHurtMore::SkillEffectHurtMore(BaseSprite* obj, int skillEffectId)
	:SkillEffect(obj, skillEffectId)
{
	//³¢ÊÔ´¥·¢
	this->onTrigger();
}

SkillEffectHurtMore::~SkillEffectHurtMore(){
	end();
}

void SkillEffectHurtMore::trigger(){
	this->m_obj->_hurt_more = this->m_obj->_hurt_more + this->m_value;
}

void SkillEffectHurtMore::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

	int value = this->m_obj->_hurt_more * this->m_curTimes; 

	this->m_obj->_hurt_more = this->m_obj->_hurt_more - value;
}