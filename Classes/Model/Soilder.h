#ifndef __Soilder_H__
#define __Soilder_H__

#include "Utils/Public.h"
#include "BaseSprite.h"

class Soilder : public BaseSprite
{
public:
    virtual bool init(int soilderID, Vec2 pos, AIManager* ai);
    static Soilder* create(int soilderID, Vec2 pos, AIManager* ai);

public:
    void loadData();    // ��������
    void showUI();      // ��ʾUI
    void addHPBar();    // ��Ѫ��

    virtual void run();         // ��·
    virtual void atk();         // ����
    virtual void hurt(int x);   // ����
    virtual bool isDeath();     // �Ƿ�����
    
    void update(float dt);
    
public:
    int _soilderID;
    int _type;
    int _level;
    int _name;
    
    int _healthPoint;
    int _totalHP;
    int _damage;
    int _attackSpeed;
    int _shootRange;
    
    bool _isbroken;
    int _dir;
    Armature* arm;
    LoadingBar* _hpBar;         // Ѫ��
    
    
    AIManager* _ai;
    BaseSprite* _target;
};

#endif // __Soilder_H__
