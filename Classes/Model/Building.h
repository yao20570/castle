#ifndef __Building_H__
#define __Building_H__

#include "Utils/Public.h"
#include "BaseSprite.h"

class Building : public BaseSprite
{
public:
    virtual bool init(int index, AIManager* ai);
    static Building* create(int index, AIManager* ai);
	virtual ~Building() { }
    
public:
    void loadData(int index);       // ��������
    void showUI();                  // ��ʾUI
    void addHPBar();                // ���Ѫ��
    
    virtual void atk();
    virtual void hurt(int x);
    virtual bool isDeath();
    
    void update(float dt);
    
public:
    // ��������
    int _index;                 // �ڴ��е������±�
    int _id;                    // ���
    int _BuildingID;            // �������ID
    
    int _type;                  // ����
    int _healthPoint;           // ����ֵ
    int _totalHP;               // ������ֵ
    
    // ��������
    bool _isBroken;             // �Ƿ񱻴ݻ�
    int _canAttack;             // �Ƿ�Ϊ������ʩ
    int _damage;                // ������
    int _shootRange;            // ��̷�Χ
    int _damageRange;           // �������˷�Χ
    
    // ͼƬ
    Size _size;               // �ߴ�
    Sprite* _normal;            // ����ͼƬ
    Sprite* _broken;            // �ٻ�ͼƬ
    
    LoadingBar* _hpBar;         // Ѫ��
    
    BaseSprite* _target;
    AIManager* _ai;
};

#endif // __Building_H__
