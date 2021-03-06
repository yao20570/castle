#pragma once

#include "Utils/PublicDefault.h"
#include "Skill/SkillDefine.h"

class BaseSprite;

class SkillEffect{

public:
	SkillEffect(BaseSprite* obj, int skillEffectId, BaseSprite* caster);
	virtual ~SkillEffect();

	virtual void update(float dt);

public:
	bool isCanRemove();

	BaseSprite* getObj();
	int getEffectId();
	SkillEffectType getType();
	SkillEffectKind getKind();
	SkillAnimLayerType getAnimType();

protected:
	virtual bool isCanTrigger();
	virtual void onTrigger();
	virtual bool trigger();		//触发一次效果，由子类实现


	virtual bool isCanEnd();
	virtual void onEnd();
	virtual void end();			//效果结束，由子类实现
	
	virtual void addAnim();
	virtual void delAnim();

protected:
	int						m_auto_key;
	BaseSprite*				m_caster;
	BaseSprite*				m_obj;
	bool					m_isCanRemove;

	int						m_id;
	SkillEffectType			m_type;
	SkillEffectKind			m_kind;
	string					m_name;
	int						m_value1;
	int						m_value2;
	int						m_value3;
	int						m_times;				//触发次数
	int						m_cd;					//效果触发冷却
	int						m_lastTime;				//持续时间

	int						m_curTimes;				//当前次数
	INT64					m_startMTimestamp;		//效果开始时间
	INT64					m_endMTimestamp;		//效果结束时间

	string					m_anim_name;
	SkillAnimLayerType		m_anim_type;

	SkilAnimData			m_anim_data;
};