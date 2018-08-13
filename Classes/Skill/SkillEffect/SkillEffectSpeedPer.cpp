
#include "SkillEffectSpeedPer.h"
#include "Model/BaseSprite.h"


SkillEffectSpeedPer::SkillEffectSpeedPer(BaseSprite* obj, int skillEffectId, BaseSprite* caster)
	:SkillEffect(obj, skillEffectId, caster)
{
	//³¢ÊÔ´¥·¢
	this->onTrigger();
}

SkillEffectSpeedPer::~SkillEffectSpeedPer(){
	end();
}

bool SkillEffectSpeedPer::trigger(){
	this->m_obj->_speed_per = this->m_obj->_speed_per + this->m_value1;
	return true;
}

void SkillEffectSpeedPer::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

	int value = this->m_value1 * this->m_curTimes;
	this->m_obj->_speed_per = this->m_obj->_speed_per - value;
}