#ifndef __BuildingUpgradeDialog_H__
#define __BuildingUpgradeDialog_H__

#include "Utils/Public.h"


class BuildingUpgradeDialog : public cocos2d::Layer
{
public:
    static BuildingUpgradeDialog* create(BuildingSprite* buildingSprite);
    virtual bool init(BuildingSprite* buildingSprite);
    
    void showDialog();
    void hideDialog();
    void removeDialog();
    void showNowInfo();     // ��ʾ��ǰ�ȼ���Ϣ
    void showNextInfo();    // ��ʾ��һ�ȼ�������Ϣ
    void showRequire();     // ��ʾ��Դ������Ϣ
    void btnCallback(Ref* sender, Widget::TouchEventType type);
    void upgradeCallback(Ref* sender, Widget::TouchEventType type);
private:
    int _type;
    int _goldRequire;
    int _woodRequire;
    int _timeRequire;
    
    BuildingSprite* _buildingSprite;
};

#endif // __BuildingUpgradeDialog_H__
