#ifndef __GlobalManager_H__
#define __GlobalManager_H__

#include "Utils/Public.h"

#define GM() GlobalManager::getInstance()

class NewBuilding;

class GlobalManager : public cocos2d::Ref
{
public:
    GlobalManager();
    virtual ~GlobalManager();
    virtual bool init();
    static GlobalManager* getInstance();

//--------------------------------------------------------------------------------
// get
//--------------------------------------------------------------------------------
    inline int getMin(int x, int y) { return (x < y) ? x : y; }
    inline int getMax(int x, int y) { return (x > y) ? x : y; }
    
    // ������
    float xmult(Vec2 p1, Vec2 p2, Vec2 p0);  // ���
    bool isPointInCircle(Vec2 centerPoint, float radius, Vec2 p);   // ���Ƿ���Բ����
    bool isPointInDiamond(Vec2 centerPoint, Size size, Vec2 p);   // ���Ƿ���������
    bool isPointInRect(Vec2 centerPoint, Size size, Vec2 p);      // ���Ƿ��ھ�����
    
    string getIntToStr(int value);          // intתstring
    int getStrToInt(string value);          // stringתint
    float getDistance(Vec2 p1, Vec2 p2);    // �������
    int getManhadun(Vec2 p1, Vec2 p2);      // �����پ���
    int getTimeStamp();                     // ��ȡʱ���
    
    // ��Ƭ����
    Vec2 getMapPos(Vec2 pos);               // ��ͼ����
    Vec2 getTiledPos(Vec2 pos);             // ��Ƭ����
    bool isOutMap(Vec2 pos);                // ��ƬԽ��
    void setCoverd(Vec2 pos, int delta);    // ռ��/�ͷŵ���
    bool isCovered(Vec2 pos);               // �����Ƿ�ռ��
    void clearCovered();                    // ���ռ�����
    Vec2 getSpaceTiled();                   // ��ȡ�յ�
    ValueVector getNextSpace(Vec2 pos);     // �����ȡ��ǰλ����Χ�Ŀյ�
    
    int getDir(Vec2 src, Vec2 des);         // ͨ�����㣬��ȡ����
    int getDir(Vec2 delta);                 // ͨ���������򣬻�ȡ����
    Vec2 getMapDelta(int idr);              // ������Ƭ�����ƫ�Ʒ��򣬻�ȡ��ͼ�����ƫ����
    
    // ս�����
    int getBattleTownID();  // ��ȡҪ�����ĳǳ�ID
    
    // ����ͼƬ
    string getBuildingIMG(int type);        // ��ȡ������ͼƬ��
    string getBuildingBrokenIMG(int type);  // ��ȡ�������ݻٺ��ͼƬ��
    
    // �����л�
    void enterWorldScene();
    void enterHomeScene();
    void enterChapterScene(int townID, int type);

public:
    int _cover[40][40]; // ռ�����

    bool _newBuild;
    NewBuilding* _build;
    
    int _townID;
    
private:
    static GlobalManager* _g;
};

#endif // __GlobalManager_H__
