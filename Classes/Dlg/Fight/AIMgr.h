#pragma once


#include "Utils/PublicDefault.h"

class BaseSprite;
class Soilder;
class Building;
class Hero;

class AIMgr
{
public:
	AIMgr();
	~AIMgr();
	virtual bool init();
	
	void setSelect(int select);

	// 获取目标
	BaseSprite* getTarget(Vec2 pos);    // pos点是空地，还是建筑
	BaseSprite* getTargetEnemy(Vec2 pos);
	BaseSprite* getTargetFriend(Vec2 pos, int range);
	Vec2 getNextPos(Vec2 src, Vec2 des, bool isHero);

	bool isWithinShootRange(Vec2 src, Vec2 des, int range);

	void addHero(Hero* hero, int type);
	void addSoilder(Soilder* hero, int type);
	void delHero(Hero* hero, int type);
	void delSoilder(Soilder* hero, int type);

	void update(float dt);
	void setSelectObj(BaseSprite* obj);
	BaseSprite* getSelectObj() { return _select_obj; }
public:
	set<Hero*> _heroSelf;
	set<Hero*> _heroEnemy;

	set<Soilder*> _soildersSelf;
	set<Soilder*> _soildersEnemy;

	BaseSprite* _select_obj;

};