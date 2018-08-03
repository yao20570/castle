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
	//������Ϣ
	int Id;
	string Name;
	SkillTriggerType triggerType;
	int TriggerValue;
	SkillTargetType targetType;
	SkillScopeType scopeType;
	int shootRange;					//ʩ������
	int radius;						//Ч����Χ�뾶		
	int cd;
public:
	//������Ϣ
	long m_useMSec;//��ǰʹ�ü���ʱ��(����)��cdʹ��
	
private:
	BaseSprite* m_obj;
	AIMgr* m_ai;
	//vector<SkillEffectInfo> effects;
	vector<int> m_effectIds;
	INT64 m_CDMTimestamp;
};
