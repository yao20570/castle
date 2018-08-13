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

    virtual void atk(Armature* arm, MovementEventType eventType, const std::string& str);         // 攻击
	//virtual void hurt(int hurtType, int x, BaseSprite* atk);   // 受伤
	virtual void hurtEffect(int x, int bulletType);
    virtual bool isDeath();     // 是否死亡
    
    //void update(float dt);
	virtual void setSelect(bool b);
	virtual void setObjName(string name);
    
	void addTouch();
	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
	virtual void onTouchMoved(Touch* pTouch, Event* pEvent);
	virtual void onTouchEnded(Touch* pTouch, Event* pEvent);
	void setState(int state, int _dir);
public:
    int _type;
    int _level;    

    int _attackSpeed;
    
    //Armature* _arm;
    

	Armature* _skill1;
	Sprite* _circle;

};

#endif // __Soilder_H__
