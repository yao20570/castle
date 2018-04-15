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
    
    void loadCsvData(std::string file, ValueVector& data);
public :
	map<int, ValueMap >* getMyObj();
	void updatMyObj(int type, int lv);
	void insertMyObj(int type, int lv);

private:
    static DBManager* _g;
    sqlite3* _pdb;
};

#endif // __DBManager_H__
