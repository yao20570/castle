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
    void loadData();    // ��������
    void showUI();      // ��ʾUI
    void addHPBar();    // ��Ѫ��

    virtual void run();         // ��·
    virtual void atk();         // ����
    virtual void hurt(int x);   // ����
    virtual bool isDeath();     // �Ƿ�����
    
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
    LoadingBar* _hpBar;         // Ѫ��
    
	AIMgr* _ai;
    BaseSprite* _target;

	Sprite* _circle;
};

#endif // __Soilder_H__
