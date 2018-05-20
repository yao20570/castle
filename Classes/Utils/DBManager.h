#ifndef __DBManager_H__
#define __DBManager_H__

#include "Utils/Public.h"

#define DBM() DBManager::getInstance()

class DBManager : public cocos2d::Ref
{
public:
    DBManager();
    virtual ~DBManager();
    virtual bool init();
    static DBManager* getInstance();
    
    bool open();
    void close();
    void executeUpdate(std::string sql);
    ValueVector executeQuery(std::string sql);
    
    void createTable();
    void createPlayerInfo();
    void createTownStateInfo();
    void createMiwuStateInfo();
    void createBuildingListInfo();
    void createSoilderListInfo();
    void createHeroListInfo();

	void createMyObj();
	void createMySetting();
	void createMyEquip();
	void createMyMission();
	
    
    void loadCsvData(std::string file, ValueVector& data);
public :
	map<int, ValueMap >* getMyObj();
	ValueMap* getMyObjById(int id);
	void updatMyObj(int type, int lv);
	void insertMyObj(int type, int lv);

	void saveMySetting(int settingIndex, Vector<Node*> nodes);
	void cleanMySetting(int settingIndex);
	ValueVector getMySetting(int settingIndex);

	ValueVector getMyEquips();
	ValueVector getMyEquipsById(int myEquipId);
	void insertMyEquip(int myEquipId);
	void deleteMyEquip(int id);
	void updateMyEquip(int id, bool isWear);

	ValueVector getMyMission();
	void updateMyMission(int MissionMain, int MissionSub);
private:
    static DBManager* _g;
    sqlite3* _pdb;
};

#endif // __DBManager_H__
