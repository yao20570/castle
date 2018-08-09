#include "PnlFloor.h"
#include "Skill/Skill.h"



PnlFloor * PnlFloor::create()
{
	PnlFloor * ret = new (std::nothrow) PnlFloor();
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

PnlFloor::PnlFloor()
{
}

PnlFloor::~PnlFloor()
{
}

bool PnlFloor::init(){
	_skill_shoot_range = nullptr;
	_skill_radius = nullptr;

	return true;
}

void PnlFloor::showSkillShootRange(bool b, Skill* skill){
	if (_skill_shoot_range){
		_skill_shoot_range->removeFromParent();
		_skill_shoot_range = nullptr;
	}

	if (b == true){
		_skill_shoot_range = DrawNode::create();
		_skill_shoot_range->drawCircle(this->getPosition(), skill->shootRange, 100, 1, true, Color4F(1, 1, 1, 1));
		this->addChild(_skill_shoot_range);
	}
}

void PnlFloor::showSkillRadius(bool b, Skill* skill){
	if (_skill_radius){
		_skill_radius->removeFromParent();
		_skill_radius = nullptr;
	}

	if (b == true){
		switch (skill->scopeType)
		{
			case SkillScopeType::ROUND:{
				_skill_radius = DrawNode::create();
				_skill_radius->drawCircle(this->getPosition(), skill->radius, 100, 1, true, Color4F(1, 1, 1, 1));
				this->addChild(_skill_radius);
				break;
			}
			case SkillScopeType::RECT_SCOPE:{
				_skill_radius = DrawNode::create();
				_skill_radius->drawRect(Vec2::ZERO, Vec2(skill->shootRange, skill->radius), Color4F(1, 1, 1, 1));
				this->addChild(_skill_radius);
				break;
			}
			case SkillScopeType::FAN:{

				break;
			}
		}
	}
}
