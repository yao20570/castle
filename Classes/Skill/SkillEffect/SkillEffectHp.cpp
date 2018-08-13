
#include "SkillEffectHp.h"
#include "Model/BaseSprite.h"


SkillEffectHp::SkillEffectHp(BaseSprite* obj, int skillEffectId, BaseSprite* caster)
	:SkillEffect(obj, skillEffectId, caster)
{
	//尝试触发
	this->onTrigger();
}

SkillEffectHp::~SkillEffectHp(){
	end();
}

bool SkillEffectHp::trigger(){
	switch (m_value1)
	{
		case 1:	//配置的伤害
			this->m_obj->hurt(2,  -m_value2, nullptr, m_value3 == 0);
			break;
		case 2: //施法者攻击力的伤害
			this->m_obj->hurt(2,  -((float)m_caster->getDamage() * m_value2 / 100), nullptr, m_value3 == 0);
			break;
		case 3: //生命百分比			
			this->m_obj->hurt(2,  -((float)this->m_obj->_totalHP * m_value2 / 100), nullptr, m_value3 == 0);
			break;
	}
	return true;
}

void SkillEffectHp::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

}