#include "SkillEffectFactory.h"

#include "Model/BaseSprite.h"
#include "Skill/SkillEffect/SkillEffectHp.h"
#include "Skill/SkillEffect/SkillEffectDamage.h"
#include "Skill/SkillEffect/SkillEffectDamagePer.h"
#include "Skill/SkillEffect/SkillEffectSpeed.h"
#include "Skill/SkillEffect/SkillEffectSpeedPer.h"
#include "Skill/SkillEffect/SkillEffectDef.h"
#include "Skill/SkillEffect/SkillEffectDefPer.h"
#include "Skill/SkillEffect/SkillEffectXiXue.h"
#include "Skill/SkillEffect/SkillEffectCrit.h"
#include "Skill/SkillEffect/SkillEffectCritRate.h"
#include "Skill/SkillEffect/SkillEffectShootRange.h"
#include "Skill/SkillEffect/SkillEffectYun.h"
#include "Skill/SkillEffect/SkillEffectHurtMore.h"
#include "Skill/SkillEffect/SkillEffectFanJian.h"
#include "Skill/SkillEffect/SkillEffectDisperse.h"


SkillEffectFactory* SkillEffectFactory::_g = nullptr;
SkillEffectFactory* SkillEffectFactory::getInstance()
{
    if (nullptr == _g) {
        _g = new SkillEffectFactory();
        _g->init();
    }
    return _g;
}

SkillEffectFactory::SkillEffectFactory()
{
    
}

SkillEffectFactory::~SkillEffectFactory()
{
    _g = nullptr;
}

bool SkillEffectFactory::init()
{
    
    return true;
}

SkillEffect* SkillEffectFactory::newSkillEffect(BaseSprite* obj, int effectId, BaseSprite* caster)
{
	SkillEffectType tt = this->getEffectTypeById(effectId);
	switch (tt)
	{
		case SkillEffectType::HP:
			return new SkillEffectHp(obj, effectId, caster);
		case SkillEffectType::Speed:
			return new SkillEffectSpeed(obj, effectId, caster);
		case SkillEffectType::SpeedPer:
			return new SkillEffectSpeedPer(obj, effectId, caster);
		case SkillEffectType::Damage:
			return new SkillEffectDamage(obj, effectId, caster);
		case SkillEffectType::DamagePer:
			return new SkillEffectDamagePer(obj, effectId, caster);
		case SkillEffectType::Def:
			return new SkillEffectDef(obj, effectId, caster);
		case SkillEffectType::DefPer:
			return new SkillEffectDefPer(obj, effectId, caster);
		case SkillEffectType::XiXue:
			return new SkillEffectXiXue(obj, effectId, caster);
		case SkillEffectType::Crit:
			return new SkillEffectCrit(obj, effectId, caster);
		case SkillEffectType::CritRate:
			return new SkillEffectCritRate(obj, effectId, caster);
		case SkillEffectType::ShootRange:
			return new SkillEffectShootRange(obj, effectId, caster);
		case SkillEffectType::Yun:
			return new SkillEffectYun(obj, effectId, caster);
		case SkillEffectType::FanJian:
			return new SkillEffectFanJian(obj, effectId, caster);
		case SkillEffectType::HurtMore:
			return new SkillEffectHurtMore(obj, effectId, caster);
		case SkillEffectType::Disperse:
			return new SkillEffectDisperse(obj, effectId, caster);
		default:
			break;
	}

	return nullptr;
}

SkillEffectType SkillEffectFactory::getEffectTypeById(int effectId)
{
	SkillEffectType tt = (SkillEffectType)(effectId / 10000);
	return tt;
}