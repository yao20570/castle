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
    void loadData();    // ��������
    void showUI();      // ��ʾUI
    void addHPBar();    // ��Ѫ��
    void animaCallback(Armature *armature, MovementEventType movementType, const std::string& movementID);
    //void setSelect(bool select);

    virtual void idle();
    virtual void atk(Armature* arm, MovementEventType eventType, const std::string& str);   // ����
	//virtual void hurt(int hurtType, int x, BaseSprite* atk);           // ����
	virtual void hurtEffect(int x);
	virtual void death();           // ����
    virtual bool isDeath();             // �Ƿ�����
    virtual void putSkill(int type);    // �ͷż���
    virtual void finishSkill(Armature* arm, int state); // ���ܽ���
    
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
