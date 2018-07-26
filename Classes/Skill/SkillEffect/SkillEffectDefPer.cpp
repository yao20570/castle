
#include "SkillEffectDefPer.h"
#include "Model/BaseSprite.h"


SkillEffectDefPer::SkillEffectDefPer(BaseSprite* obj, int skillEffectId)
	:SkillEffect(obj, skillEffectId)
{
	//���Դ���
	this->onTrigger();
}

SkillEffectDefPer::~SkillEffectDefPer(){
	end();
}

void SkillEffectDefPer::trigger(){
	this->m_obj->_def_per = this->m_obj->_def_per + this->m_value;
}

void SkillEffectDefPer::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

	int valur = this->m_value * this->m_curTimes;
	this->m_obj->_def_per = this->m_obj->_def_per - valur;
}