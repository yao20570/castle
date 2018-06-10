#ifndef __BulletSprite_H__
#define __BulletSprite_H__

#include "Utils/Public.h"
#include "BaseSprite.h"

class BulletSprite : public cocos2d::Sprite
{
public:
    virtual bool init(Vec2 src, Vec2 des, int damage, BaseSprite* target, string img, int srcType);
    static BulletSprite* create(Vec2 src, Vec2 des, int damage, BaseSprite* target, string img, int srcType);

public:
    void showUI();
    void atk();
public:
    Vec2 _src;
    Vec2 _des;
    int _damage;
    BaseSprite* _target;
    string _img;
	int _srcType;
};

#endif // __BulletSprite_H__
