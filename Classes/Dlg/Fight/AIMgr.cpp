

#include "AIMgr.h"
#include "Model/BaseSprite.h"
#include "Model/Hero.h"
#include "Model/Soilder.h"
#include "Model/Building.h"

AIMgr::AIMgr() 
:_heroSelf()
,_heroEnemy()
,_soildersSelf()
,_soildersEnemy()
, _select_obj(nullptr)
{
	init();
}
AIMgr::~AIMgr() {

}

bool AIMgr::init()
{
	return true;
}


void AIMgr::setSelect(int select)
{
	//if (_hero == nullptr) return;

	//if (select == 0) {
	//	_hero->setSelect(true);
	//}
	//else {
	//	_hero->setSelect(false);
	//}
}

// pos点是空地，还是建筑
BaseSprite* AIMgr::getTarget(Vec2 pos)
{
	//Vec2 p = GM()->getMapPos(pos);
	//for (int i = 0; i < _buildings.size(); i++) {
	//	Building* b = _buildings.at(i);
	//	// 是否触摸到建筑
	//	Vec2 centerPoint = GM()->getMapPos(b->_pos);
	//	Size size = Size(b->_normal->getContentSize());
	//	if (GM()->isPointInRect(centerPoint, size, p)) {
	//		return b;
	//	}
	//}
	return nullptr;
}


BaseSprite* AIMgr::getTargetEnemy(Vec2 pos)
{
	//Building* target = nullptr;
	//int mindis = INF;
	//for (int i = 0; i < _buildings.size(); i++) {
	//	Building* b = _buildings.at(i);
	//	if (b->isDeath() == true) continue;

	//	int dis = GM()->getManhadun(pos, b->_pos);
	//	if (dis < mindis) {
	//		mindis = dis;
	//		target = b;
	//	}
	//}
	//return target;
	return nullptr;
}

BaseSprite* AIMgr::getTargetFriend(Vec2 pos, int range)
{
	//for (int i = 0; i < _soilders.size(); i++) {
	//	Soilder* target = _soilders.at(i);
	//	if (target->isDeath() == true) continue;
	//	if (isWithinShootRange(target->_pos, pos, range)) return target;
	//}
	//if (_hero != nullptr) {
	//	if (isWithinShootRange(_hero->_pos, pos, range)) {
	//		return _hero;
	//	}
	//}
	return nullptr;
}


bool AIMgr::isWithinShootRange(cocos2d::Vec2 src, cocos2d::Vec2 des, int range)
{
	Vec2 p1 = GM()->getMapPos(src);
	Vec2 p2 = GM()->getMapPos(des);
	int dis = (int)GM()->getDistance(p1, p2);
	return dis <= range;
}


Vec2 AIMgr::getNextPos(Vec2 src, Vec2 des, bool isHero)
{
	Vec2 pos = src;
	int mindis = INF;
	for (int i = 0; i < 9; i++) {
		int x = src.x + DX[i] + EPS;
		int y = src.y + DY[i] + EPS;
		if (x < 0 || x > TILED_TOTAL_X || y < 0 || y > TILED_TOTAL_Y) continue;
		if (isHero && GM()->isCovered(Vec2(x, y))) continue;
		int dis = GM()->getManhadun(Vec2(x, y), des);
		if (dis < mindis) {
			mindis = dis;
			pos = Vec2(DX[i], DY[i]);
		}
	}
	return pos;
}

void AIMgr::addHero(Hero* hero, int type)
{
	switch (type)
	{
	case 1: _heroSelf.insert(hero);	 break;
	case 2: _heroEnemy.insert(hero); break;
	}
}

void AIMgr::addSoilder(Soilder* soilder, int type)
{
	switch (type)
	{
	case 1: _soildersSelf.insert(soilder);	break;
	case 2: _soildersEnemy.insert(soilder);	break;
	}
}

void AIMgr::delHero(Hero* hero, int type)
{
	set<Hero*>* heros = &_heroSelf;
	switch (type)
	{
	case 1:heros = &_heroSelf;	break;
	case 2:heros = &_heroEnemy;	break;
	}
	auto it = heros->find(hero);
	if (it != heros->end()) {
		heros->erase(it);
	}
}

void AIMgr::delSoilder(Soilder* soilder, int type)
{
	set<Soilder*>* soilders = &_soildersSelf;
	switch (type)
	{
	case 1: soilders = &_soildersSelf;	break;
	case 2: soilders = &_soildersEnemy;	break;
	}
	auto it = soilders->find(soilder);
	if (it != soilders->end()) {
		soilders->erase(it);
	}
}

void AIMgr::update(float dt) 
{

}

void AIMgr::setSelectObj(BaseSprite* obj) {
	if (_select_obj == obj) {
		return;
	}

	if (_select_obj != nullptr) {
		_select_obj->setSelect(false);
	}

	_select_obj = obj;
	_select_obj->setSelect(true);
}