#ifndef __Hero_H__
#define __Hero_H__

#include "Utils/PublicDefault.h"
#include "BaseSprite.h"




class AIMgr;

class Hero : public BaseSprite
{
public:
	Hero();
	~Hero();
public:
    virtual bool init(int ID, AIMgr* ai, int camp);
    static Hero* create(int ID, AIMgr* ai, int camp);

public:
    void loadData();    // 加载数据
    void showUI();      // 显示UI
    void addHPBar();    // 加血条
    void animaCallback(Armature *armature, MovementEventType movementType, const std::string& movementID);
    //void setSelect(bool select);

    virtual void idle();
    virtual void atk(Armature* arm, MovementEventType eventType, const std::string& str);   // 攻击
	//virtual void hurt(int hurtType, int x, BaseSprite* atk);           // 受伤
	virtual void hurtEffect(int x);
	virtual void death();           // 死亡
    virtual bool isDeath();             // 是否死亡
    virtual void putSkill(int type);    // 释放技能
    virtual void finishSkill(Armature* arm, int state); // 技能结束
    
    //void update(float dt);
	virtual void setSelect(bool b);
	virtual void setObjName(string name);

    void addTouch();
    virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
    virtual void onTouchMoved(Touch* pTouch, Event* pEvent);
    virtual void onTouchEnded(Touch* pTouch, Event* pEvent);
    
private:
	void setState(int state, int dir);

public:
    int _heroID;
    int _type;
    int _level;
    
    int _attackSpeed;
    
    float _delta;
    
    bool _isSelect;
    //Armature* _arm;
    Armature* _skill1;
    Armature* _skill2;
    
	
    Vec2 _targetPos;
    float _expReward;

	Sprite* _circle;
};

#endif // __Hero_H__
