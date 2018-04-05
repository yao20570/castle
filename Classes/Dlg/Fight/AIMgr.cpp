

#include "AIMgr.h"
#include "Model/BaseSprite.h"
#include "Model/Hero.h"
#include "Model/Soilder.h"
#include "Model/Building.h"

static Vec2 dir[8] = { Vec2(0, 1)
					 , Vec2(1,1)
					 , Vec2(1,0)
					 , Vec2(1,-1)
					 , Vec2(0,-1)
					 , Vec2(-1,-1)
					 , Vec2(-1,0)
					 , Vec2(-1,1) };


AIMgr::AIMgr()
	:_objSelf()
	, _objEnemy()
	, _select_obj(nullptr)
{
	init();
}
AIMgr::~AIMgr() {
}

bool AIMgr::init()
{
	memset(dotMap, 0, 32 * 48 * sizeof(BaseSprite*));
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
	int dis = src.getDistance(des);
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
	case 1: _objSelf.insert((BaseSprite*)hero);	 break;
	case 2: _objEnemy.insert((BaseSprite*)hero); break;
	}
}

void AIMgr::addSoilder(Soilder* soilder, int type)
{
	switch (type)
	{
	case 1: _objSelf.insert((BaseSprite*)soilder);	break;
	case 2: _objEnemy.insert((BaseSprite*)soilder);	break;
	}
}

void AIMgr::delHero(Hero* hero, int type)
{
	set<BaseSprite*>* heros = &_objSelf;
	switch (type)
	{
	case 1:heros = &_objSelf;	break;
	case 2:heros = &_objEnemy;	break;
	}
	auto it = heros->find(hero);
	if (it != heros->end()) {
		heros->erase(it);
	}
}

void AIMgr::delSoilder(Soilder* soilder, int type)
{
	set<BaseSprite*>* soilders = &_objSelf;
	switch (type)
	{
	case 1: soilders = &_objSelf;	break;
	case 2: soilders = &_objEnemy;	break;
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

void AIMgr::start() {
	for (auto it : _objSelf) {
		it->setState(STATE_RUN, it->_dir);
	}
	for (auto it : _objEnemy) {
		it->setState(STATE_RUN, it->_dir);
	}
}

void AIMgr::setObjPos(BaseSprite* obj, Vec2 pos) {
	if (((Hero*)obj)->_name == 13) {
		int i = 1;
	}
	Vec2 dot(-1, -1);
	if (this->isCanSet(obj, pos, dot)) {

		//设置位置，重置dotMap占位
		Vec2 p = obj->getPosition();
		obj->_prePosList.push_back(p);
		if (obj->_prePosList.size() > 3) {
			obj->_prePosList.pop_front();
		}
		obj->setPosition(pos);
		obj->setLocalZOrder(obj->getPositionX() - obj->getPositionY() * 10000);
		if (obj->_dotX > 0 && obj->_dotX < 32 && obj->_dotY > 0 && obj->_dotY < 48) {
			dotMap[obj->_dotX + obj->_dotY * 32] = nullptr;
		}		
		obj->_dotX = dot.x;
		obj->_dotY = dot.y;
		dotMap[obj->_dotX + obj->_dotY * 32] = obj;
	}
	else {
		float speed = sqrt((float)obj->_speed * 0.5);
		
		Vec2 minPos(1000000, 1000000);
		for (auto& it : dir) {
			Vec2 testPos(it.x * speed, it.y * speed);
			testPos.add(obj->getPosition());

			//不走回头路
			bool isGoBack = false;
			for (auto& p : obj->_prePosList) {
				if ((int)p.x == (int)testPos.x && (int)p.y == (int)testPos.y) {
					isGoBack = true;
				}
			}
			if (isGoBack) {
				continue;
			}

			//能否设置
			if (isCanSet(obj, testPos, dot)) {
				int testDis = testPos.getDistance(obj->_target->getPosition());
				int mixDis = minPos.getDistance(obj->_target->getPosition());
				if (testDis < mixDis) {
					minPos = testPos;
				}
			}
		}

		//不能移动了
		if (minPos.equals(Vec2(1000000, 1000000))) {
			return;
		}

		//设置位置，重置dotMap占位
		obj->_prePosList.push_back(obj->getPosition());
		if (obj->_prePosList.size() > 3) {
			obj->_prePosList.pop_front();
		}
		obj->setPosition(minPos);
		obj->setLocalZOrder(obj->getPositionX() - obj->getPositionY() * 10000);
		if (obj->_dotX > 0 && obj->_dotX < 32 && obj->_dotY > 0 && obj->_dotY < 48) {
			dotMap[obj->_dotX + obj->_dotY * 32] = nullptr;
		}
		obj->_dotX = dot.x;
		obj->_dotY = dot.y;
		dotMap[obj->_dotX + obj->_dotY * 32] = obj;
	}
}

bool AIMgr::isCanSet(BaseSprite* obj, Vec2& pos, Vec2& dot) {
	int dotX = ceil(pos.x / 20) - 1;
	int dotY = ceil(pos.y / 20) - 1;

	for (int i = -1; i < 2; ++i) {
		for (int j = -1; j < 2; ++j) {
			int testX = dotX + i;
			int testY = dotY + j;
			if (testX < 0 || testX>32) {
				continue;
			}
			if (testY < 0 || testY > 48) {
				continue;
			}			

			BaseSprite* ptr = dotMap[testY * 32 + testX];
			if (ptr != nullptr && ptr != obj) {
				if (ptr->getPosition().getDistance(pos) < max(ptr->_radius, obj->_radius)) {
					return false;
				}
			}
		}
	}
	dot.x = dotX;
	dot.y = dotY;
	return true;
}

void AIMgr::setObjDead(BaseSprite* obj) {
	dotMap[obj->_dotY * 32 + obj->_dotX] = nullptr;
	obj->_dotX = 1000000;
	obj->_dotY = 1000000;
}