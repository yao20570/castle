#pragma once

#include "Utils/PublicDefault.h"

class BaseSprite;

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
	HpLeft = 1,		//����������ĳ���ٷֱȴ���
	Random = 2,		//���ʴ���
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
	RECT_SCOPE = 4,	//����
	FAN = 5,		//����
};

//��Ч������ĸ��ط�
const enum class SkillAnimLayerType:int
{
	None = 0,		//��
	Floor = 1,		//�ذ�
	Body = 2,		//��ɫ����
	Pos = 3,		//����У��ͽ�ɫͬһ���ڵ�
	Sky = 4,		//���
};

//��Чλ������
const enum class SkillAnimPosType :int{
	Src = 1,				//ʩ����λ���ϲ���
	Target = 2,				//Ŀ��㲥��
	Src2Target = 3,			//ʩ����λ�÷��е�Ŀ�ĵ�
	Target2Src = 4,			//Ŀ�ĵطɵ�ʩ����
};


struct SkilAnimData{

	SkilAnimData()
		: angle(0)
	{
	}

	SkillAnimLayerType layerType;
	SkillAnimPosType posType;
	int loop; //-1:ѭ��,0:һ��,>0����ʱ��
	string fileName;
	BaseSprite* obj;
	Vec2 targetPos;
	float angle;
	int key;

	string getFilePath(){
		char str[256] = {0};
		sprintf(str, "animation/skill/%s.ExportJson", fileName.c_str());
		return str;
	}
};
