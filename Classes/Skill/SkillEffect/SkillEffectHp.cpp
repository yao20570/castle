
#include "SkillEffectHp.h"
#include "Model/BaseSprite.h"


SkillEffectHp::SkillEffectHp(BaseSprite* obj, int skillEffectId)
	:SkillEffect(obj, skillEffectId)
{
	//���Դ���
	this->onTrigger();
}

SkillEffectHp::~SkillEffectHp(){
	end();
}

void SkillEffectHp::trigger(){
	this->m_obj->hurt(2,  -m_value, nullptr);
}

void SkillEffectHp::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

}