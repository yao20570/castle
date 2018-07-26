#pragma once


#include "Utils/PublicDefault.h"

#include "Skill/Skill.h"


class BaseSprite;
class AIMgr;



class SkillMgr
{
public:
	

	SkillMgr(BaseSprite* obj, AIMgr* ai);
	~SkillMgr();

	void addSkill(int skillId);
	void delSkill(int skillId);

	void reset();

	bool setSkillUseTime(Skill* skill);
	void triggerSkill(SkillTriggerType tt);

	void update(float dt);

public:
	BaseSprite* m_obj;
	AIMgr* m_ai;
	map<SkillTriggerType, vector<Skill*>> m_skills;

	map<int, int> m_cds;
};


