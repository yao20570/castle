#ifndef __BuildingSprite_H__
#define __BuildingSprite_H__

#include "Utils/Public.h"


class BuildingSprite : public cocos2d::Sprite
{
public:
    virtual bool init(int index);
    static BuildingSprite* create(int index);
    
    void addTouch();
    virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
    virtual void onTouchMoved(Touch* pTouch, Event* pEvent);
    virtual void onTouchEnded(Touch* pTouch, Event* pEvent);
    void detach();

public:
    void loadData(int index);       // ��������
    void showUI();                  // ��ʾUI
    void moveBuilding(Vec2 delta);  // �ƶ�����
    void selectedAction();          // ѡ�ж���
    void unselectedAction();        // ȥ��ѡ�ж���
    void setTiledPos();             // ������Ƭ����
    
    void showUpgradePro();          // ��ʾ������
    void upgrade();                 // ��������
    void upgradeFinished();         // �������
    
    // ľ�ĳ������ ���ջ���Դ
    void showBubble();
    void harvest();
    void update(float dt);

public:
    // ��������
    int _index;                 // �ڴ��е������±�
    int _id;                    // ���
    int _BuildingID;            // �������ID
    Vec2 _pos;                  // ����
    int _buildState;            // ����״̬��1�ѽ��ɣ�2���ڽ��졣
    int _lastBuildTime;         // �����ʱ�䡣
    int _lastGoldHarvest;       // ����ջ�ʱ�䡣��󳡽�Ҳ���
    int _lastWoodHarvest;       // ľ���ջ�ʱ�䡣ľ�ĳ�ľ�Ĳ�����
    
    int _type;                  // ����
    int _level;                 // �ȼ�
    int _healthPoint;           // ����ֵ
    string _name;               // ��������
    string _description;        // ����
    
    int _goldRequire;           // ����������
    int _woodRequire;           // ��������ľ��
    int _timeRequire;           // ��������ʱ��
    int _playerLevelRequire;    // ������ҵȼ�
    int _baseLevelRequire;      // ��������˾��ȼ�
    int _expReward;             // ���������Ҿ���
    
    // ��������
    int _goldProduct;           // ÿСʱ��Ҳ���
    int _woodProduct;           // ÿСʱľ�Ĳ���
    
    // ��Դ����
    int _goldCapacity;          // �������
    int _woodCapacity;          // ľ������
    
    // ��������
    bool _isBroken;             // �Ƿ񱻴ݻ�
    bool _canAttack;            // �Ƿ�Ϊ������ʩ
    int _damage;                // ������
    int _attackSpeed;           // �����ٶ�
    int _shootRange;            // ��̷�Χ
    int _damageRange;           // �������˷�Χ
    Node* _attackTarget;        // ����Ŀ��
    
    // ��������
    bool _canTouched;           // �Ƿ�����ƶ�
    bool _isTouched;            // �Ƿ񱻴���
    bool _isSelected;           // �Ƿ�ѡ��
    bool _isShowOpt;            // �Ƿ���ʾ��opt����
    float _deltaPos;            // �ƶ�ƫ��
    EventListenerTouchOneByOne* listener;
    
    // ͼƬ
    Size _size;       // �ߴ�
    Sprite* _normal;    // ����ͼƬ
    Sprite* _broken;    // �ٻ�ͼƬ
    Sprite* _tip;       // ��ͷ
    
    HarvestBubble* bubble;
};

#endif // __BuildingSprite_H__
