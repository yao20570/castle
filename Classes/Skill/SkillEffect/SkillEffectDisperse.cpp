
#include "SkillEffectDisperse.h"
#include "Model/BaseSprite.h"
#include "Dlg/Fight/AIMgr.h"


SkillEffectDisperse::SkillEffectDisperse(BaseSprite* obj, int skillEffectId, BaseSprite* caster)
	:SkillEffect(obj, skillEffectId, caster)
{
	//³¢ÊÔ´¥·¢
	this->onTrigger();
}

SkillEffectDisperse::~SkillEffectDisperse(){
	end();
}

bool SkillEffectDisperse::trigger(){
	this->m_obj->clearBadSkillEffect();
	return true;
}

void SkillEffectDisperse::end(){
	if (this->m_isCanRemove){
		return;
	}
	this->m_isCanRemove = true;

}