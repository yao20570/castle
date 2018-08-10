#pragma once

#include "Utils/PublicDefault.h"

class BaseSprite;

class SkillEffectAnim : public Armature
{
public:
	SkillEffectAnim();
	~SkillEffectAnim();

    static SkillEffectAnim *create();

    static SkillEffectAnim *create(const std::string& name);

    static SkillEffectAnim *create(const std::string& name, Bone *parentBone);

	BaseSprite* getObj();
	void setObj(BaseSprite* obj);

	int getKey();
	void setKey(int key);
private:
	int			m_auto_key;
	BaseSprite* m_obj;
};