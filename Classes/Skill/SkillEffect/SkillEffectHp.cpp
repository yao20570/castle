
#include "SkillEffectHp.h"
#include "Model/BaseSprite.h"


SkillEffectHp::SkillEffectHp(BaseSprite* obj, int skillEffectId, BaseSprite* caster)
	:SkillEffect(obj, skillEffectId, caster)
{
	//³¢ÊÔ´¥·¢
	this->onTrigger();
}

SkillEffectHp::~SkillEffectHp(){
	end();
}

void SkillEffectHp::trigger(){
	switch (m_value1)
	{
		case 1:	//ÅäÖÃµÄÉËº¦
			this->m_obj->hurt(2,  -m_value2, nullptr);
			break;
		case 2: //Ê©·¨Õß¹¥»÷Á¦µÄÉËº¦
			this->m_obj->hurt(2,  -(m_caster->getDamage() * m_value2 / 100), nullptr);
			break;
	}
}

void SkillEffectHp::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

}