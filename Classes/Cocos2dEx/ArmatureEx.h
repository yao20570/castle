#pragma once

#include "Utils/PublicDefault.h"

class ArmatureEx : public Armature
{
public:
	ArmatureEx();
	~ArmatureEx();

    static ArmatureEx *create();

    static ArmatureEx *create(const std::string& name);

    static ArmatureEx *create(const std::string& name, Bone *parentBone);
private:


};