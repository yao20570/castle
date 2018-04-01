#ifndef __Soilder_H__
#define __Soilder_H__

#include "Utils/PublicDefault.h"
#include "BaseSprite.h"
class AIMgr;

class Soilder : public BaseSprite
{
public:
	Soilder();
	~Soilder();
public:
    virtual bool init(int soilderID, AIMgr* ai, int camp);
    static Soilder* create(int soilderID, AIMgr* ai, int camp);

public:
    void loadData();    // 加载数据
    void showUI();      // 显示UI
    void addHPBar();    // 加血条

    virtual void run();         // 走路
    virtual void atk();         // 攻击
    virtual void hurt(int x);   // 受伤
    virtual bool isDeath();     // 是否死亡
    
    void update(float dt);
	virtual void setSelect(bool b);
    
	void addTouch();
	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
	virtual void onTouchMoved(Touch* pTouch, Event* pEvent);
	virtual void onTouchEnded(Touch* pTouch, Event* pEvent);
public:
    int _soilderID;
    int _type;
    int _level;
    int _name;
	int _camp;
    
    int _healthPoint;
    int _totalHP;
    int _damage;
    int _attackSpeed;
    int _shootRange;
    
    bool _isbroken;
    int _dir;
    Armature* _arm;
    LoadingBar* _hpBar;         // 血条
    
	AIMgr* _ai;
    BaseSprite* _target;

	Sprite* _circle;
};

#endif // __Soilder_H__
