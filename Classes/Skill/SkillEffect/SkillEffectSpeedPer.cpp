
#include "SkillEffectSpeedPer.h"
#include "Model/BaseSprite.h"


SkillEffectSpeedPer::SkillEffectSpeedPer(BaseSprite* obj, int skillEffectId)
	:SkillEffect(obj, skillEffectId)
{
	//³¢ÊÔ´¥·¢
	this->onTrigger();
}

SkillEffectSpeedPer::~SkillEffectSpeedPer(){
	end();
}

void SkillEffectSpeedPer::trigger(){
	this->m_obj->_speed_per = this->m_obj->_speed_per + this->m_value;
}

void SkillEffectSpeedPer::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

	int value = this->m_value * this->m_curTimes;
	this->m_obj->_speed_per = this->m_obj->_speed_per - value;
}