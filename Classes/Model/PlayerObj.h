#ifndef __PlayerObj_H__
#define __PlayerObj_H__

#include "Utils/PublicDefault.h"
#include "BaseSprite.h"




class AIMgr;

class PlayerObj : public BaseSprite
{
public:
	PlayerObj();
	~PlayerObj();
public:
	virtual bool init(int ID, AIMgr* ai, int camp);
	static PlayerObj* create(int ID, AIMgr* ai, int camp);

public:
	void loadData();    // 加载数据
	void showUI();      // 显示UI
	void addHPBar();    // 加血条
	
	virtual void hurt(int hurtType, int x, BaseSprite* atk );           // 受伤
	virtual bool isDeath();
	void update(float dt);

	virtual void addSkillEffect(int skillEffectId);
	virtual void delSkillEffect(int skillEffectId);
	virtual void triggerSkill(SkillTriggerType tt, const Vec2& targetPos);
private:

public:
	
	int _camp;
	
	Armature* _arm;

	AIMgr* _ai;

	int _heroAtkTime;
};

#endif // __PlayerObj_H__
