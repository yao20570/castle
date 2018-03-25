#ifndef __NewBuilding_H__
#define __NewBuilding_H__

#include "Utils/Public.h"


class NewBuilding : public cocos2d::Sprite
{
public:
    virtual bool init(int type, int gold);
    static NewBuilding* create(int type, int gold);
    
    void addTouch();
    virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
    virtual void onTouchMoved(Touch* pTouch, Event* pEvent);
    virtual void onTouchEnded(Touch* pTouch, Event* pEvent);

    
    void okCallback(Ref* sender, Widget::TouchEventType type);
    void cancleCallback(Ref* sender, Widget::TouchEventType type);
    
public:
    void showUI();                  // ��ʾUI
    void moveBuilding(Vec2 delta);  // �ƶ�����

public:
    // ��������
    int _type;
    int _gold;
    Vec2 _pos;
    
    // ��������
    bool _isTouched;            // �Ƿ񱻴���
    float _deltaPos;            // �ƶ�ƫ��
    EventListenerTouchOneByOne* listener;
    
    // ͼƬ
    Size _size;       // �ߴ�
    Sprite* _normal;    // ����ͼƬ
    Sprite* _tip;       // ��ͷ
    Button* _cancle;
    Button* _ok;
};

#endif // __NewBuilding_H__
