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
    virtual void atk(Armature* arm, MovementEventType eventType, const std::string& str);         // ����
    virtual void hurt(int x);   // ����
	virtual void hurtEffect(int x);
    virtual bool isDeath();     // �Ƿ�����
    
    void update(float dt);
	virtual void setSelect(bool b);
	virtual void setObjName(string name);
    
	void addTouch();
	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
	virtual void onTouchMoved(Touch* pTouch, Event* pEvent);
	virtual void onTouchEnded(Touch* pTouch, Event* pEvent);
	void setState(int state, int _dir);
public:
    int _soilderID;
    int _type;
    int _level;
    int _name;
	int _camp;

    int _totalHP;
    int _damage;
    int _attackSpeed;
    int _shootRange;
    
    Armature* _arm;
    LoadingBar* _hpBar;         // Ѫ��
    
	AIMgr* _ai;

	Armature* _skill1;
	Sprite* _circle;

};

#endif // __Soilder_H__
