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
	virtual bool trigger();		//����һ��Ч����������ʵ��


	virtual bool isCanEnd();
	virtual void onEnd();
	virtual void end();			//Ч��������������ʵ��
	
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
	int						m_times;				//��������
	int						m_cd;					//Ч��������ȴ
	int						m_lastTime;				//����ʱ��

	int						m_curTimes;				//��ǰ����
	INT64					m_startMTimestamp;		//Ч����ʼʱ��
	INT64					m_endMTimestamp;		//Ч������ʱ��

	string					m_anim_name;
	SkillAnimLayerType		m_anim_type;

	SkilAnimData			m_anim_data;
};