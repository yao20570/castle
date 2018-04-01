#ifndef __Hero_H__
#define __Hero_H__

#include "Utils/PublicDefault.h"
#include "BaseSprite.h"


static const int STATE_IDLE     = 0;
static const int STATE_RUN      = 1;
static const int STATE_ATK      = 2;
static const int STATE_SKILL    = 3;
static const int STATE_DEATH    = 4;

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
    virtual void atk(BaseSprite* target);   // ����
    virtual void hurt(int x);           // ����
    virtual bool isDeath();             // �Ƿ�����
    virtual void putSkill(int type);    // �ͷż���
    virtual void finishSkill(Armature* arm, int state); // ���ܽ���
    
    void update(float dt);
	virtual void setSelect(bool b);

    void addTouch();
    virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
    virtual void onTouchMoved(Touch* pTouch, Event* pEvent);
    virtual void onTouchEnded(Touch* pTouch, Event* pEvent);
    
public:
    int _id;
    int _heroID;
    int _type;
    int _level;
    int _name;
	int _camp;
    
    int _healthPoint;
    int _totalHP;
    int _damage;
    int _attackSpeed;
    int _shootRange;
    
    float _delta;
    LoadingBar* _hpBar; // Ѫ��
    
    bool _isSelect;
    bool _isbroken;
    int _dir;
    Armature* _arm;
    Armature* _skill1;
    Armature* _skill2;
    
	
    int _state;         // ״̬
    Vec2 _targetPos;
    BaseSprite* _target;
	AIMgr* _ai;
    float _expReward;

	Sprite* _circle;
};

#endif // __Hero_H__
