#pragma once

#include "Utils/PublicDefault.h"
#include "Skill/SkillDefine.h"

class BaseSprite;

class SkillEffect{

public:
	SkillEffect(BaseSprite* obj, int skillEffectId, BaseSprite* caster);
	~SkillEffect();

	virtual void update(float dt);

public:
	bool isCanRemove();

	BaseSprite* getObj();
	int getEffectId();
	SkillEffectType getType();


protected:
	virtual bool isCanTrigger();
	virtual void onTrigger();
	virtual void trigger();		//����һ��Ч����������ʵ��

	virtual bool isCanEnd();
	virtual void onEnd();
	virtual void end();			//Ч��������������ʵ��
protected:
	BaseSprite*		m_caster;
	BaseSprite*		m_obj;
	bool			m_isCanRemove;

	int				m_id;
	SkillEffectType m_type;
	string			m_name;
	int				m_value1;
	int				m_value2;
	int				m_value3;
	int				m_times;				//��������
	int				m_cd;					//Ч��������ȴ
	int				m_lastTime;				//����ʱ��

	int				m_curTimes;				//��ǰ����
	INT64			m_startMTimestamp;		//Ч����ʼʱ��
	INT64			m_endMTimestamp;		//Ч������ʱ��
};