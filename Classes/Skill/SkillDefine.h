#pragma once


const enum class SkillEffectType:int
{
	HP = 1011,
	HPPer = 1012,
	Speed = 1021,
	SpeedPer = 1022,
	Damage = 1031,
	DamagePer = 1032,
	Def = 1041,
	DefPer = 1042,
	XiXue = 1051,
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
	Life = 5,		//生命剩余时触发
	Count = 6,		//触发类型数量
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
	RECT_SCOPE = 4,		//矩形
	FAN = 5,		//扇形
};
