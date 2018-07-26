#include "SkillEffectFactory.h"

#include "Model/BaseSprite.h"
#include "Skill/SkillEffect/SkillEffectDamage.h"
#include "Skill/SkillEffect/SkillEffectDamagePer.h"
#include "Skill/SkillEffect/SkillEffectDef.h"
#include "Skill/SkillEffect/SkillEffectDefPer.h"
#include "Skill/SkillEffect/SkillEffectFanJian.h"
#include "Skill/SkillEffect/SkillEffectHp.h"
#include "Skill/SkillEffect/SkillEffectHpPer.h"
#include "Skill/SkillEffect/SkillEffectHurtMore.h"
#include "Skill/SkillEffect/SkillEffectSpeed.h"
#include "Skill/SkillEffect/SkillEffectSpeedPer.h"
#include "Skill/SkillEffect/SkillEffectXiXue.h"
#include "Skill/SkillEffect/SkillEffectYun.h"


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

SkillEffect* SkillEffectFactory::newSkillEffect(BaseSprite* obj, int effectId)
{
	SkillEffectType tt = this->getEffectTypeById(effectId);
	switch (tt)
	{
		case SkillEffectType::HP:
			return new SkillEffectHp(obj, effectId);
		case SkillEffectType::HPPer:
			return new SkillEffectHpPer(obj, effectId);
		case SkillEffectType::Damage:
			return new SkillEffectDamage(obj, effectId);
		case SkillEffectType::DamagePer:
			return new SkillEffectDamagePer(obj, effectId);
		case SkillEffectType::Speed:
			return new SkillEffectSpeed(obj, effectId);
		case SkillEffectType::SpeedPer:
			return new SkillEffectSpeedPer(obj, effectId);
		case SkillEffectType::Def:
			return new SkillEffectDef(obj, effectId);
		case SkillEffectType::DefPer:
			return new SkillEffectDefPer(obj, effectId);
		case SkillEffectType::XiXue:
			return new SkillEffectXiXue(obj, effectId);
		case SkillEffectType::Yun:
			return new SkillEffectYun(obj, effectId);
		case SkillEffectType::FanJian:
			return new SkillEffectFanJian(obj, effectId);
		case SkillEffectType::HurtMore:
			return new SkillEffectHurtMore(obj, effectId);
		default:
			break;
	}
}

SkillEffectType SkillEffectFactory::getEffectTypeById(int effectId)
{
	SkillEffectType tt = (SkillEffectType)(effectId / 10000);
	return tt;
}