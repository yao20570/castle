
#include "SkillEffectHpPer.h"
#include "Model/BaseSprite.h"


SkillEffectHpPer::SkillEffectHpPer(BaseSprite* obj, int skillEffectId)
	:SkillEffect(obj, skillEffectId)
{
	//³¢ÊÔ´¥·¢
	this->onTrigger();
}

SkillEffectHpPer::~SkillEffectHpPer(){
	end();
}

void SkillEffectHpPer::trigger(){
	int hp = this->m_obj->_totalHP * m_value / 100;
	this->m_obj->hurt(2, -hp, nullptr);
}

void SkillEffectHpPer::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

}