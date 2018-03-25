#ifndef __AIManager_H__
#define __AIManager_H__

#include "Utils/Public.h"

class AIManager : public Node
{
public:
    virtual bool init();
    CREATE_FUNC(AIManager);
    
    void setHudLayer(BattleHudLayer* hudLayer);
    void setMapLayer(BattleMapLayer* mapLayer);

    void setSelect(int select);
    
    // ��ȡĿ��
    BaseSprite* getTarget(Vec2 pos);    // pos���ǿյأ����ǽ���
    BaseSprite* getTargetEnemy(Vec2 pos);
    BaseSprite* getTargetFriend(Vec2 pos, int range);
    Vec2 getNextPos(Vec2 src, Vec2 des, bool isHero);
    
    bool isWithinShootRange(Vec2 src, Vec2 des, int range);
    inline bool isHeroEnter() { return _hero != nullptr; }
    
    void gameOver(int state); // ��Ϸ����
    
public:
    Hero* _hero;
    Vector<Soilder*> _soilders;
    Vector<Building*> _buildings;
    
    BattleHudLayer* _hudLayer;
    BattleMapLayer* _mapLayer;
};

#endif // __AIManager_H__
