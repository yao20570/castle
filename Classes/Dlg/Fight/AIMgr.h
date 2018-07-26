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
	
	// 获取目标
	BaseSprite* getTarget(Vec2 pos);    // pos点是空地，还是建筑
	BaseSprite* getTargetEnemy(Vec2 pos);
	BaseSprite* getTargetFriend(Vec2 pos, int range);
	Vec2 getNextPos(Vec2 src, Vec2 des, bool isHero);

	bool isWithinShootRange(Vec2 src, Vec2 des, int range);

	void addObj(BaseSprite* obj, int type);
	//void addHero(Hero* hero, int type);
	//void addSoilder(Soilder* soilder, int type);
	void delObj(BaseSprite* obj, int type);
	//void delHero(Hero* hero, int type);
	//void delSoilder(Soilder* soilder, int type);

	
	BaseSprite* getObj(BaseSprite* obj);

	void update(float dt);
	void setSelectObj(BaseSprite* obj);
	BaseSprite* getSelectObj() { return _select_obj; }

	void start();
	void close();
	void hideEnemy();
	void showEnemy();

	void showName(bool b);

	void setObjPos(BaseSprite* obj, Vec2 pos);
	bool isCanSet(BaseSprite* obj, Vec2& pos, Vec2& dot);
	void setObjDead(BaseSprite* obj);

	bool isOver(int type);
	bool isWin(int type);
	void reset();
public:
	set<BaseSprite*> _objSelf;
	set<BaseSprite*> _objEnemy;

	BaseSprite* _select_obj;
	BaseSprite* dotMap[56 * 32];

};