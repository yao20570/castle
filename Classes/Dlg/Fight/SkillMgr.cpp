

#include "SkillMgr.h"
#include "Model/BaseSprite.h"
#include "Utils/JsonManager.h"
#include "Dlg/Fight/AIMgr.h"

SkillMgr::SkillMgr(BaseSprite* obj, AIMgr* ai)
	:m_obj(obj)
	,m_ai(ai)
{
	for (int i=0; i<(int)SkillTriggerType::Count; ++i){
		this->m_skills[SkillTriggerType(i)] = vector<Skill*>();
	}
	
}

SkillMgr::~SkillMgr(){

}

void SkillMgr::addSkill(int skillId){
	Skill* skill = new Skill(m_obj, m_ai, skillId);
		
	vector<Skill*>& skills = this->m_skills[skill->triggerType];
	skills.push_back(skill);
}

void SkillMgr::delSkill(int skillId){

}

void SkillMgr::update(float dt){
}

void SkillMgr::reset(){

	for (auto& it : this->m_skills){
		for (auto& skill : it.second){
			skill->reset();
		}
	}	
}

void SkillMgr::triggerSkill(SkillTriggerType tt){
	
	for (auto skill : this->m_skills[tt]){
		//skill->useSkill();
	}
}

bool SkillMgr::setSkillUseTime(Skill* skill){
	struct timeval now;
	gettimeofday(&now, NULL);
	long time = now.tv_usec / 1000;
	if (time > skill->m_useMSec + skill->cd){
		skill->m_useMSec = time;
		return true;
	}
	return false;
}