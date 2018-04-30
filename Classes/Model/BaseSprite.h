#ifndef __BaseSprite_H__
#define __BaseSprite_H__

#include "Utils/PublicDefault.h"


static const int STATE_IDLE = 0;
static const int STATE_RUN = 1;
static const int STATE_ATK = 2;
static const int STATE_SKILL = 3;
static const int STATE_DEATH = 4;
static const int STATE_WIN = 5;

class BaseSprite : public cocos2d::Sprite
{
public:
    virtual bool init();
    CREATE_FUNC(BaseSprite);
    BaseSprite() 
		:_prePosList()
	{}
    virtual ~BaseSprite() {}

public:
    virtual void atk();
    virtual void run();
    virtual void hurt(int x);
    virtual void idle();
    virtual bool isDeath();
	virtual void setSelect(bool b);
	virtual void setState(int state, int dir);
public:
	int _objType;
	list<Vec2> _prePosList;
    Vec2 _pos;
	int _radius;
	int _state;
	int _dir;
	string _cur_anima_key;
	bool _isbroken;
	int _healthPoint;
	int _dotX; 
	int _dotY;
	int _speed;
	BaseSprite* _target;

};


#endif // __BaseSprite_H__
