#ifndef __DataManager_H__
#define __DataManager_H__

#include "Utils/Public.h"

#define DM() DataManager::getInstance()


class DataManager : public cocos2d::Ref
{
public:
    DataManager();
    virtual ~DataManager();
    virtual bool init();
    static DataManager* getInstance();
    
//--------------------------------------------------------------------------------
// load
//--------------------------------------------------------------------------------
    void loadPlayerInfo();          // ���������Ϣ
    
    void loadPlayerLevelInfo();     // ���صȼ���Ϣ
    void loadTownInfo();            // ���سǳ���Ϣ
    void loadMiwuInfo();            // ����������Ϣ
    
    void loadBuildingInfo();        // ���ؽ�����Ϣ
    void loadBuildingLimitInfo();   // ���ؽ������Ʊ���Ϣ��˾��ȼ���Ӧ������������
    
    void loadSoilderInfo();         // ����ʿ����Ϣ
    void loadSoilderLimitInfo();    // ���ر�Ӫʿ���������Ʊ�
    
    void loadHeroInfo();            // ����Ӣ����Ϣ
    void loadSkillInfo();           // ���ؼ�����Ϣ
    
    void loadBattleInfo();          // ���عؿ���Ϣ
    
    void loadCsvData(std::string file, ValueVector& data);
	//map<int, ValueMap>* loadCsvData(std::string file, const string& key);
    void bindingIndex(ValueVector& data, map<string, int>& index, string ID);
    void printData(ValueVector& data);
    
//--------------------------------------------------------------------------------
// update
//--------------------------------------------------------------------------------
    void updatePlayerExp(int exp);
    void updateGold(int gold);
    void updateWood(int wood);
    void updateGoldCapacity(int capacity);
    void updateWoodCapacity(int capacity);
    void updateRing(int addRing);
    
    
    void updateTownType(int townID, int type);
    void updateTownHarvest(int townID, int lastHarvest);
    
    void updateMiwuType(int level);
    
    void updateBuildingPos(int ID, Vec2 pos);           // �ı�����
    void updateBuildingBuildState(int ID, int state);   // �ı佨��״̬
    void updateBuildingLevel(int ID);                   // �ı佨���ȼ�
    void updateLastGoldHarvest(int ID, int lastHarvest);// ���
    void updateLastWoodHarvest(int ID, int lastHarvest);// ľ��
    int addNewBuilding(int type, Vec2 pos);             // �½�����
    
    // ʿ��
    void updateSoilderCount(int ID, int delta);         // ����ʿ��������+delta
    void updateSoilderLevel(int ID);                    // ����ʿ���ȼ�
    
    // Ӣ��
    void updateHeroExp(int ID, int addExp);             // ����Ӣ�۾���ֵ������
    void updateHeroLevel(int ID);                       // ����Ӣ�۵ȼ�
    

//--------------------------------------------------------------------------------
// get
//--------------------------------------------------------------------------------
    int getPlayerExpRequire(int level);
    int getPlayerLevel();   // ��ȡ��ҵȼ�
    int getGoldCount();     // ��ȡ�����Դ����
    int getWoodCount();     // ��ȡľ����Դ����
    int getGoldCapacity();  // ��ȡ�����Դ����
    int getWoodCapacity();  // ��ȡľ����Դ����
    
    // �ǳ�
    ValueMap& getTownInfo(int townID);  // ��ȡ�ǳ���Ϣ

    // ��ʩ
    ValueMap& getBuilding(int ID);                  // �����б�ID��ȡ������Ϣ
    ValueMap& getBuildingInfo(int buildingID);      // ����BuildingID����ȡ������Ϣ
    ValueMap& getBuildingLimit(int level);          // ����˾��ȼ�����ȡ��������
    ValueMap& getBuildingNextInfo(int buildingID);  // ��ȡ��ʩ��һ�ȼ���Ϣ
    int getBaseTowerLevel();                        // ��ȡ˾��ȼ�
    int getBuildingCountForType(int type);          // ��ȡĳ��type�Ľ�������
    
    // ʿ��
    ValueMap& getSoilder(int ID);               // �����б�ID����ȡʿ��
    ValueMap& getSoilderInfo(int soilderID);    // ����SoilderID����ȡʿ����Ϣ
    ValueMap& getSoilderNextInfo(int soilderID);// ��ȡʿ����һ�ȼ���Ϣ
    int getSoilderLimit(int level);             // ��ȡ��Ӫ��������
    int getCampLevel();                         // ��ȡ��Ӫ�ȼ�
    int getLabLevel();                          // ��ȡ�о����ȼ�
    
    // Ӣ��
    ValueMap& getHero(int ID);                  // ����Ӣ��ID����ȡ���Ӣ����Ϣ
    ValueMap& getHeroInfo(int heroID);          // ����HeroID����ȡӢ����Ϣ
    ValueMap& getHeroNextInfo(int heroID);      // ��ȡӢ����һ�ȼ���Ϣ
    ValueMap& getSkillInfo(int skillID);        // ��ȡ������Ϣ
    
    // �ؿ���Ϣ
    ValueVector getBattleBuilding(int townID); // ��ȡ�ؿ�townID����ʩ��Ϣ
    
public:

    // ValueMap <string, Value<stirng, string> >
    ValueVector _player;        // �����Ϣ
    ValueVector _playerLevel;   // ��ҵȼ�����
    
    ValueVector _town;          // �ǳ���Ϣ
    ValueVector _miwu;          // ������Ϣ

    int _baseID;                // ˾���ID�������±�
    int _campID;                // ��Ӫ��ID�������±�
    int _labID;                 // �о���ID�������±�
    ValueVector _buildingInfo;  // ���н�����Ϣ
    ValueVector _building;      // ��ҽ�����Ϣ
    ValueVector _buildingLimit; // �������Ʊ�
    
    ValueVector _soilderInfo;   // ����ʿ����Ϣ
    ValueVector _soilder;       // ���ʿ����Ϣ
    ValueVector _soilderLimit;  // ʿ���������Ʊ�
    
    ValueVector _heroInfo;      // ����Ӣ����Ϣ
    ValueVector _hero;          // Ӣ����Ϣ
    ValueVector _skillInfo;     // ���м�����Ϣ

    
    ValueVector _battleBuilding; // �ؿ���ʩ

    
    // ����Vector�±������ID
    map<string, int> _indexPlayerLevel;     // Level���±�
    map<string, int> _indexTown;            // TownID���±�
    map<string, int> _indexMiwu;            // MiwuID���±�
    
    map<string, int> _indexBuilding;        // ID���±�
    map<string, int> _indexBuildingInfo;    // BuildingID���±�
    map<string, int> _indexBuildingLimit;   // ˾�Level���±�
    
    map<string, int> _indexSoilder;         // ID���±�
    map<string, int> _indexSoilderInfo;     // SoilderID���±�
    map<string, int> _indexSoilderLimit;    // ��ӪLevel���±�
    
    map<string, int> _indexHero;            // ID���±�
    map<string, int> _indexHeroInfo;        // HeroID���±�
    map<string, int> _indexSkillInfo;       // SkillID���±�

private:
    static DataManager* _g;
};

#endif // __DataManager_H__

