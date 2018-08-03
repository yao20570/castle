
#include "SkillEffectDefPer.h"
#include "Model/BaseSprite.h"


SkillEffectDefPer::SkillEffectDefPer(BaseSprite* obj, int skillEffectId, BaseSprite* caster)
	:SkillEffect(obj, skillEffectId, caster)
{
	//���Դ���
	this->onTrigger();
}

SkillEffectDefPer::~SkillEffectDefPer(){
	end();
}

void SkillEffectDefPer::trigger(){
	this->m_obj->_def_per = this->m_obj->_def_per + this->m_value1;
}

void SkillEffectDefPer::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

	int value = this->m_value1 * this->m_curTimes;
	this->m_obj->_def_per = this->m_obj->_def_per - value;
}