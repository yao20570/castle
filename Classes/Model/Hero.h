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
    
    //void setSelect(bool select);

    virtual void idle();
    virtual void atk(Armature* arm, MovementEventType eventType, const std::string& str);   // ����
    virtual void hurt(int x);           // ����
	virtual void death();           // ����
    virtual bool isDeath();             // �Ƿ�����
    virtual void putSkill(int type);    // �ͷż���
    virtual void finishSkill(Armature* arm, int state); // ���ܽ���
    
    void update(float dt);
	virtual void setSelect(bool b);

    void addTouch();
    virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
    virtual void onTouchMoved(Touch* pTouch, Event* pEvent);
    virtual void onTouchEnded(Touch* pTouch, Event* pEvent);
    
private:
	void setState(int state, int dir);

public:
    int _id;
    int _heroID;
    int _type;
    int _level;
    int _name;
	int _camp;
    
    int _totalHP;
    int _damage;
    int _attackSpeed;
    int _shootRange;
    
    float _delta;
    LoadingBar* _hpBar; // Ѫ��
    
    bool _isSelect;
    Armature* _arm;
    Armature* _skill1;
    Armature* _skill2;
    
	
    Vec2 _targetPos;
	AIMgr* _ai;
    float _expReward;

	Sprite* _circle;
};

#endif // __Hero_H__
