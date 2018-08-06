#pragma once


const enum class SkillEffectType:int
{
	HP = 1011,
	HpMaxPer = 1012,
	Speed = 1021,
	SpeedPer = 1022,
	Damage = 1031,
	DamagePer = 1032,
	Def = 1041,
	DefPer = 1042,
	XiXue = 1051,
	Crit = 1061,
	CritRate = 1062,
	ShootRange = 1071,
	Yun = 2001,
	HurtMore = 2002,
	FanJian = 2003,
};

const enum class EffectState:int
{
	Yun = 0,
	FanJian = 1,
};

//��������
const enum class SkillTriggerType:int
{
	Hand = 0,		//�ֶ��ͷ�
	RoundStart = 1, //�غϿ�ʼ
	Atk = 2,	//����ʱ
	Hurt = 3,		//����ʱ
	Dead = 4,		//����ʱ
	Count = 5,		//������������
};

//��������
const enum class SkillTriggerCondition:int
{
	HpLeft = 0,		//����������ĳ���ٷֱȴ���
	Random = 1,		//���ʴ���
};


//Ŀ������
const enum class SkillTargetType:int
{
	Enemy = 1,		//�з�
	Friend = 2,		//�ѷ�
};

//��Χ
const enum class SkillScopeType:int
{
	SINGLE = 1,		//����
	ALL = 2,		//ȫ��
	ROUND = 3,		//Բ��
	RECT_SCOPE = 4,		//����
	FAN = 5,		//����
};
