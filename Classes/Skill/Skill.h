#pragma once

#include "Utils/PublicDefault.h"
#include "Skill/SkillDefine.h"

class  AIMgr;
class BaseSprite;

struct SkillEffectInfo{
	
	SkillEffectType m_type;
	int value;
	int times;
	float cd;
	float lastTime;

	SkillEffectInfo(SkillEffectType type)
		: m_type(type)
		, value(0)
		, times(1)
		, cd(0)
		, lastTime(0)
	{}

	~SkillEffectInfo(){}
};


class Skill
{

public:
	Skill(BaseSprite* obj, AIMgr* ai, const rapidjson::Value& skillCfg);
	~Skill();
	void update(float dt);

	void reset();
	void useSkill(const Vec2& targetPos);
private:
	const set<BaseSprite*>& getObjsByTarget();
	void getObjsByScope(const Vec2& targetPos, const set<BaseSprite*>& inObjs, set<BaseSprite*>& outObjs);

	void getObjBySingle(const Vec2& targetPos, const set<BaseSprite*>& inObjs, set<BaseSprite*>& outObjs);
	void getObjByALL(const Vec2& targetPos, const set<BaseSprite*>& inObjs, set<BaseSprite*>& outObjs);
	void getObjByRound(const Vec2& targetPos, const set<BaseSprite*>& inObjs, set<BaseSprite*>& outObjs);
	void getObjByRect(const Vec2& targetPos, const set<BaseSprite*>& inObjs, set<BaseSprite*>& outObjs);
	void getObjByFan(const Vec2& targetPos, const set<BaseSprite*>& inObjs, set<BaseSprite*>& outObjs);

public:
	//基数信息
	int Id;
	string Name;
	SkillTriggerType triggerType;
	int TriggerValue;
	SkillTargetType targetType;
	SkillScopeType scopeType;
	int shootRange;					//施法距离
	int radius;						//效果范围半径		
	int cd;
public:
	//运行信息
	long m_useMSec;//当前使用技能时间(毫秒)，cd使用
	
private:
	BaseSprite* m_obj;
	AIMgr* m_ai;
	//vector<SkillEffectInfo> effects;
	vector<int> m_effectIds;
	INT64 m_CDMTimestamp;
};
