
#include "SkillEffectAnim.h"
#include "Model/BaseSprite.h"


SkillEffectAnim *SkillEffectAnim::create()
{
    SkillEffectAnim *armature = new (std::nothrow) SkillEffectAnim();
    if (armature && armature->init())
    {
        armature->autorelease();
        return armature;
    }
    CC_SAFE_DELETE(armature);
    return nullptr;
}


SkillEffectAnim *SkillEffectAnim::create(const std::string& name)
{
    SkillEffectAnim *armature = new (std::nothrow) SkillEffectAnim();
    if (armature && armature->init(name))
    {
        armature->autorelease();
        return armature;
    }
    CC_SAFE_DELETE(armature);
    return nullptr;
}

SkillEffectAnim *SkillEffectAnim::create(const std::string& name, Bone *parentBone)
{
    SkillEffectAnim *armature = new (std::nothrow) SkillEffectAnim();
    if (armature && armature->init(name, parentBone))
    {
        armature->autorelease();
        return armature;
    }
    CC_SAFE_DELETE(armature);
    return nullptr;
}

SkillEffectAnim::SkillEffectAnim()
	: Armature()
	, m_obj(nullptr)
{
}

SkillEffectAnim::~SkillEffectAnim()
{
}

BaseSprite* SkillEffectAnim::getObj(){
	return this->m_obj;
}
void SkillEffectAnim::setObj(BaseSprite* obj){
	this->m_obj = obj;
}


int SkillEffectAnim::getKey(){
	return this->m_auto_key;
}
void SkillEffectAnim::setKey(int key){
	this->m_auto_key = key;
}