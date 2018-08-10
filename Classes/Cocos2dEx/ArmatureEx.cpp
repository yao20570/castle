
#include "ArmatureEx.h"


ArmatureEx *ArmatureEx::create()
{
    ArmatureEx *armature = new (std::nothrow) ArmatureEx();
    if (armature && armature->init())
    {
        armature->autorelease();
        return armature;
    }
    CC_SAFE_DELETE(armature);
    return nullptr;
}


ArmatureEx *ArmatureEx::create(const std::string& name)
{
    ArmatureEx *armature = new (std::nothrow) ArmatureEx();
    if (armature && armature->init(name))
    {
        armature->autorelease();
        return armature;
    }
    CC_SAFE_DELETE(armature);
    return nullptr;
}

ArmatureEx *ArmatureEx::create(const std::string& name, Bone *parentBone)
{
    ArmatureEx *armature = new (std::nothrow) ArmatureEx();
    if (armature && armature->init(name, parentBone))
    {
        armature->autorelease();
        return armature;
    }
    CC_SAFE_DELETE(armature);
    return nullptr;
}

ArmatureEx::ArmatureEx()
	:Armature()
{
}

ArmatureEx::~ArmatureEx()
{
}