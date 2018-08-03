
#include "SkillEffectFanJian.h"
#include "Model/BaseSprite.h"
#include "Dlg/Fight/AIMgr.h"


SkillEffectFanJian::SkillEffectFanJian(BaseSprite* obj, int skillEffectId, BaseSprite* caster)
	:SkillEffect(obj, skillEffectId, caster)
{
	//³¢ÊÔ´¥·¢
	this->onTrigger();
}

SkillEffectFanJian::~SkillEffectFanJian(){
	end();
}

void SkillEffectFanJian::trigger(){
	if (this->m_obj->_camp == 1){
		this->m_obj->_ai->delObj(this->m_obj, 1);
		this->m_obj->_camp = 2;
	}
	else if(this->m_obj->_camp == 2){
		this->m_obj->_ai->delObj(this->m_obj, 2);
		this->m_obj->_camp = 1;
	}

	
	this->m_obj->_ai->addObj(this->m_obj, this->m_obj->_camp);
	this->m_obj->_target == nullptr;
	this->m_obj->setState(STATE_RUN, this->m_obj->getDir());
}

void SkillEffectFanJian::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

	this->trigger();
}