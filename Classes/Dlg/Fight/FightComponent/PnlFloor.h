#pragma once

#include "Utils/PublicDefault.h"

class Skill;

class PnlFloor : public Node
{
public:
	PnlFloor();
	~PnlFloor();
	static PnlFloor * create();
	bool init();

	void showSkillShootRange(bool b, Skill* skill);
	void showSkillRadius(bool b, Skill* skill);
	
private:
	DrawNode* _skill_shoot_range;
	DrawNode* _skill_radius;
};
