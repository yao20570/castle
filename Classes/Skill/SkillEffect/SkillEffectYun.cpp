
#include "SkillEffectYun.h"
#include "Model/BaseSprite.h"


SkillEffectYun::SkillEffectYun(BaseSprite* obj, int skillEffectId)
	:SkillEffect(obj, skillEffectId)
{
	//���Դ���
	this->onTrigger();
}

SkillEffectYun::~SkillEffectYun(){
	end();
}

void SkillEffectYun::trigger(){
	this->m_obj->setState(STATE_YUN, this->m_obj->getDir());
}

void SkillEffectYun::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

	this->m_obj->setState(STATE_RUN, this->m_obj->getDir());
}