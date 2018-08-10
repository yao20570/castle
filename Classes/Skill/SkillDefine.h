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

//触发类型
const enum class SkillTriggerType:int
{
	Hand = 0,		//手动释放
	RoundStart = 1, //回合开始
	Atk = 2,	//攻击时
	Hurt = 3,		//受伤时
	Dead = 4,		//死亡时
	Count = 5,		//触发类型数量
};

//触发条件
const enum class SkillTriggerCondition:int
{
	HpLeft = 1,		//当生命减到某个百分比触发
	Random = 2,		//几率触发
};


//目标类型
const enum class SkillTargetType:int
{
	Enemy = 1,		//敌方
	Friend = 2,		//友方
};

//范围
const enum class SkillScopeType:int
{
	SINGLE = 1,		//单个
	ALL = 2,		//全部
	ROUND = 3,		//圆形
	RECT_SCOPE = 4,	//矩形
	FAN = 5,		//扇形
};

//特效添加在哪个地方
const enum class SkillAnimLayerType:int
{
	None = 0,		//无
	Floor = 1,		//地板
	Body = 2,		//角色身上
	Pos = 3,		//半空中，和角色同一父节点
	Sky = 4,		//天空
};

//特效位置类型
const enum class SkillAnimPosType :int{
	Src = 1,				//施法者位置上播放
	Target = 2,				//目标点播放
	Src2Target = 3,			//施法者位置飞行到目的地
	Target2Src = 4,			//目的地飞到施法者
};


struct SkilAnimData{

	SkilAnimData()
		: angle(0)
	{
	}

	SkillAnimLayerType layerType;
	SkillAnimPosType posType;
	int loop; //-1:循环,0:一次,>0持续时间
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
