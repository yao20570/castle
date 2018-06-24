#ifndef __PlayerObj_H__
#define __PlayerObj_H__

#include "Utils/PublicDefault.h"
#include "BaseSprite.h"




class AIMgr;

class PlayerObj : public BaseSprite
{
public:
	PlayerObj();
	~PlayerObj();
public:
	virtual bool init(int ID, AIMgr* ai, int camp);
	static PlayerObj* create(int ID, AIMgr* ai, int camp);

public:
	void loadData();    // 加载数据
	void showUI();      // 显示UI
	void addHPBar();    // 加血条
	
	virtual void hurt(int x, BaseSprite* atk );           // 受伤
	virtual bool isDeath();
	void update(float dt);
private:

public:
	
	int _totalHP;
	int _camp;
	
	Armature* _arm;

	AIMgr* _ai;

	Text* _txt_hp;
	LoadingBar* _hpBar; // 血条
	int _heroAtkTime;
};

#endif // __PlayerObj_H__
