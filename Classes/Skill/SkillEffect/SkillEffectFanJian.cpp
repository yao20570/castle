
#include "SkillEffectFanJian.h"
#include "Model/BaseSprite.h"
#include "Dlg/Fight/AIMgr.h"


SkillEffectFanJian::SkillEffectFanJian(BaseSprite* obj, int skillEffectId, BaseSprite* caster)
	:SkillEffect(obj, skillEffectId, caster)
{
	//���Դ���
	this->onTrigger();
}

SkillEffectFanJian::~SkillEffectFanJian(){
	end();
}

bool SkillEffectFanJian::trigger(){

	if (this->m_obj->_is_boos){
		this->m_isCanRemove = true;
		return false;
	}

	if (this->m_obj->getCamp() == 1){
		this->m_obj->_ai->delObj(this->m_obj, 1);
		this->m_obj->setCamp(2);
	}
	else if(this->m_obj->getCamp() == 2){
		this->m_obj->_ai->delObj(this->m_obj, 2);
		this->m_obj->setCamp(1);
	}

	
	this->m_obj->_ai->addObj(this->m_obj, this->m_obj->_camp);
	this->m_obj->_target = nullptr;
	this->m_obj->setState(STATE_RUN, this->m_obj->getDir());

	return true;
}

void SkillEffectFanJian::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

	this->trigger();
}