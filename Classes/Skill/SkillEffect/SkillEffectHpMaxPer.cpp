
#include "SkillEffectHpMaxPer.h"
#include "Model/BaseSprite.h"


SkillEffectHpMaxPer::SkillEffectHpMaxPer(BaseSprite* obj, int skillEffectId, BaseSprite* caster)
	:SkillEffect(obj, skillEffectId, caster)
{
	//³¢ÊÔ´¥·¢
	this->onTrigger();
}

SkillEffectHpMaxPer::~SkillEffectHpMaxPer(){
	end();
}

void SkillEffectHpMaxPer::trigger(){
	int hp = this->m_obj->_totalHP * m_value1 / 100;
	this->m_obj->hurt(2, -hp, nullptr);
}

void SkillEffectHpMaxPer::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

}