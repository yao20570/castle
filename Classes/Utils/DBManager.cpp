#include "DBManager.h"


DBManager* DBManager::_g = nullptr;
DBManager* DBManager::getInstance()
{
    if (nullptr == _g) {
        _g = new DBManager();
        _g->init();
    }
    return _g;
}

DBManager::DBManager()
{
    
}

DBManager::~DBManager()
{
    _g = nullptr;
}

bool DBManager::init()
{
    return true;
}


bool DBManager::open()
{
    string pathx = FileUtils::getInstance()->getWritablePath() + "/" + DB_NAME;
	string path = "db/";
	path = path + DB_NAME;
    int ret = sqlite3_open(path.c_str(), &_pdb);
    if (ret != SQLITE_OK) {
        const char* errmsg = sqlite3_errmsg(_pdb);
        CCLOG("sqlite open error: %s", errmsg);
        sqlite3_close(_pdb);
        return false;
    }
    return true;
}

void DBManager::close()
{
    sqlite3_close(_pdb);
    _pdb = nullptr;
}

// 更新数据库
void DBManager::executeUpdate(std::string sql)
{
    if(open() == true) {
        int ret = sqlite3_exec(_pdb, sql.c_str(), nullptr, nullptr, nullptr);
        if(ret != SQLITE_OK) {
            CCLOG("update data failed!");
        }
        close();
    }
}

// 查询数据库
ValueVector DBManager::executeQuery(std::string sql)
{
    ValueVector v;

    if (open()) {
        char** table;   // 查询结果
        int r, c;       // 行数、列数
        
        sqlite3_get_table(_pdb, sql.c_str(), &table, &r, &c, nullptr);
        
        // 第0行（0 ~ c-1），为字段名
        // 第1~r行（c ~ 2*c-1），第一条记录
        for(int i = 1; i <= r; i++) {
            ValueMap map;
            for(int j = 0; j < c; j++) {
//                CCLOG("%s", table[i * c + j]);
                map[table[j]] = table[i * c + j];
            }
            v.push_back((Value)map);
        }
        
        // 记得释放查询表
        sqlite3_free_table(table);
    }
    
    return v;
}


void DBManager::createTable()
{
    //string path = FileUtils::getInstance()->getWritablePath() + "/" + DB_NAME;
	string path = "db/";
	path = path + DB_NAME;
    if (!FileUtils::getInstance()->isFileExist(path)) {
        CCLOG("create RingDB.db");
        //createPlayerInfo();
        //createTownStateInfo();
        //createMiwuStateInfo();
        //createBuildingListInfo();
        //createHeroListInfo();
        //createSoilderListInfo();

		createMyObj();
		createMySetting();
		createMyEquip();
    }
}


void DBManager::createPlayerInfo()
{
    ValueVector data;
    loadCsvData(CSV_PLAYERINFO, data);
    
    string sql = "create table PlayerInfo(ID integer primary key autoincrement, Name, Level, Exp, RingCount, GoldCount, WoodCount, GoldCapacity, WoodCapacity)";
    executeUpdate(sql);
    
    for (int i = 1; i < data.size(); i++) {
        ValueMap& map = data.at(i).asValueMap();
        sql = "insert into PlayerInfo values("
        + map["ID"].asString() + ", '"
        + map["Name"].asString() + "', '"
        + map["Level"].asString() + "', '"
        + map["Exp"].asString() + "', '"
        + map["RingCount"].asString() + "', '"
        + map["GoldCount"].asString() + "', '"
        + map["WoodCount"].asString() + "', '"
        + map["GoldCapacity"].asString() + "', '"
        + map["WoodCapacity"].asString() + "')";
        
//        CCLOG("sql: %s", sql.c_str());
        executeUpdate(sql);
    }
}

void DBManager::createTownStateInfo()
{
    ValueVector data;
    loadCsvData(CSV_TOWNSTATEINFO, data);
    
    string timeStamp = GM()->getIntToStr(GM()->getTimeStamp());
    string sql = "create table TownStateInfo(TownID integer primary key autoincrement, Type, LastHarvest)";
    executeUpdate(sql);
    
    for (int i = 1; i < data.size(); i++) {
        ValueMap& map = data.at(i).asValueMap();
        sql = "insert into TownStateInfo values("
        + map["TownID"].asString() + ", '"
        + map["Type"].asString() + "', '"
        + timeStamp + "')";
        
//        CCLOG("sql: %s", sql.c_str());
        executeUpdate(sql);
    }
}

void DBManager::createMiwuStateInfo()
{
    ValueVector data;
    loadCsvData(CSV_MIWUSTATEINFO, data);
    
    string timeStamp = GM()->getIntToStr(GM()->getTimeStamp());
    string sql = "create table MiwuStateInfo(MiwuID integer primary key autoincrement, Type)";
    executeUpdate(sql);
    
    for (int i = 1; i < data.size(); i++) {
        ValueMap& map = data.at(i).asValueMap();
        sql = "insert into MiwuStateInfo values("
        + map["MiwuID"].asString() + ", '"
        + map["Type"].asString() + "')";
        
//        CCLOG("sql: %s", sql.c_str());
        executeUpdate(sql);
    }
}

void DBManager::createBuildingListInfo()
{
    ValueVector data;
    loadCsvData(CSV_BUILDINGLISTINFO, data);
    
    string timeStamp = GM()->getIntToStr(GM()->getTimeStamp());
    string sql = "create table BuildingListInfo(ID integer primary key autoincrement, BuildingID, PositionX, PositionY, BuildState, LastBuildTime, LastGoldHarvest, LastWoodHarvest)";
    executeUpdate(sql);
    
    for (int i = 1; i < data.size(); i++) {
        ValueMap& map = data.at(i).asValueMap();
        sql = "insert into BuildingListInfo values("
        + map["ID"].asString() + ", '"
        + map["BuildingID"].asString() + "', '"
        + map["PositionX"].asString() + "', '"
        + map["PositionY"].asString() + "', '"
        + map["BuildState"].asString() + "', '"
        + timeStamp + "', '"
        + timeStamp + "', '"
        + timeStamp + "')";
        
//        CCLOG("sql: %s", sql.c_str());
        executeUpdate(sql);
    }
}

void DBManager::createSoilderListInfo()
{
    ValueVector data;
    loadCsvData(CSV_SOILDER_LISTINFO, data);
    
    string timeStamp = GM()->getIntToStr(GM()->getTimeStamp());
    string sql = "create table SoilderListInfo(ID integer primary key autoincrement, SoilderID, Count)";
    executeUpdate(sql);
    
    for (int i = 1; i < data.size(); i++) {
        ValueMap& map = data.at(i).asValueMap();
        sql = "insert into SoilderListInfo values("
        + map["ID"].asString() + ", '"
        + map["SoilderID"].asString() + "', '"
        + map["Count"].asString() + "')";
        
//        CCLOG("sql: %s", sql.c_str());
        executeUpdate(sql);
    }
}

void DBManager::createHeroListInfo()
{
    ValueVector data;
    loadCsvData(CSV_HERO_LISTINFO, data);
    
    string timeStamp = GM()->getIntToStr(GM()->getTimeStamp());
    string sql = "create table HeroListInfo(ID integer primary key autoincrement, HeroID, Exp, State)";
    executeUpdate(sql);
    
    for (int i = 1; i < data.size(); i++) {
        ValueMap& map = data.at(i).asValueMap();
        sql = "insert into HeroListInfo values("
        + map["ID"].asString() + ", '"
        + map["HeroID"].asString() + "', '"
        + map["Exp"].asString() + "', '"
        + map["State"].asString() + "')";
        
//        CCLOG("sql: %s", sql.c_str());
        executeUpdate(sql);
    }
}



// 加载CSV数据
void DBManager::loadCsvData(std::string file, ValueVector& data)
{
    Csv csv = Csv(file.c_str());
    for (int i = 0; i < csv.getRowCount(); i++) {
        ValueMap map;
        for (int j = 0; j < csv[i].size(); j++) {
            map[csv[0][j]] = csv[i][j];
        }
        data.push_back((Value)map);
    }
}


void DBManager::createMyObj()
{
	//ValueVector data;
	//loadCsvData(CSV_HERO_LISTINFO, data);

	string timeStamp = GM()->getIntToStr(GM()->getTimeStamp());
	string sql = "create table MyObj(ID integer primary key, Lv)";
	executeUpdate(sql);

	//添加两条默认的士兵，分别是步兵和箭手
	for (int i = 1; i < 6; ++i) {
		DBM()->insertMyObj(i, 1);
	}

	//添加两个英雄
	DBM()->insertMyObj(5, 1);
	DBM()->insertMyObj(6, 1);
}

static map<int, ValueMap >* myObj = new map<int, ValueMap >();
map<int, ValueMap >* DBManager::getMyObj() {
	
	myObj->clear();

	string sql = "select * from MyObj";
	ValueVector vv = DBM()->executeQuery(sql);

	for (int i = 0; i < vv.size(); i++) {
		ValueMap& row = vv.at(i).asValueMap();
		int x = row["ID"].asInt();
		int lv = row["Lv"].asInt();
		(*myObj)[row["ID"].asInt()] = row;
	}

	return myObj;
}

ValueMap* DBManager::getMyObjById(int id) {

	this->getMyObj();
	auto& it = myObj->find(id);
	if (it != myObj->end()) {
		return &(it->second);
	}
	return nullptr;
}

void DBManager::updatMyObj(int type, int lv) {
	// 更新设施位置(sqlite)
	string sql = "update MyObj set Lv='" + Value(lv).asString()
		+ "' where ID=" + Value(type).asString() + ";";
	//    CCLOG("%s", sql.c_str());
	DBM()->executeUpdate(sql);
}

void DBManager::insertMyObj(int type, int lv) {
	// 更新设施位置(sqlite)
	string sql = "insert into MyObj values("
		+ Value(type).asString() + ", '"
		+ Value(lv).asString() + "')";
	//    CCLOG("%s", sql.c_str());
	DBM()->executeUpdate(sql);
}

void DBManager::createMySetting()
{
	string sql = "create table MySetting(ID integer primary key autoincrement, SettingIndex, ObjId, x, y)";
	executeUpdate(sql);
}

void DBManager::saveMySetting(int settingIndex, Vector<Node*> nodes) {
	//清空旧的设置
	this->cleanMySetting(settingIndex);

	//保存新的设置
	string sql = "";
	for (auto it : nodes) {
		ValueMap& values = *((ValueMap*)(it->getUserData()));
		sql = sql + "insert into MySetting(SettingIndex, ObjId, x, y) values('" 
			+ Value(settingIndex).asString() + "', '"
			+ values["ID"].asString() + "', '"
			+ Value((int)it->getPositionX()).asString() + "', '"
			+ Value((int)it->getPositionY()).asString() + "'"
			+ ");";
	}
	executeUpdate(sql);
}

void DBManager::cleanMySetting(int settingIndex) {

	string sql = "delete from MySetting where SettingIndex = '" + Value(settingIndex).asString() + "'";
	executeUpdate(sql);
}

ValueVector DBManager::getMySetting(int settingIndex) {
	string sql = "select * from MySetting where SettingIndex = '" + Value(settingIndex).asString() + "'";
	return executeQuery(sql);
}



//创建装备表
void DBManager::createMyEquip() {
	string sql = "create table MyEquip(ID integer primary key autoincrement, EquipId, IsWear)";
	executeUpdate(sql);
}

//获取所有的装备
ValueVector DBManager::getMyEquips() 
{
	string sql = "select * from MyEquip";
	return executeQuery(sql);
}

ValueVector DBManager::getMyEquipsById(int myEquipId)
{
	char sql[512] = "\0";
	sprintf(sql, "select * from MyEquip where ID = %d", myEquipId);
	return executeQuery(sql);
}

void DBManager::insertMyEquip(int myEquipId) 
{
	char sql[512] = "\0";
	sprintf(sql, "insert into MyEquip(EquipId, IsWear) values( '%d', 'false' )", myEquipId);
	executeUpdate(sql);
}

void DBManager::deleteMyEquip(int id)
{
	char sql[512] = "\0";
	sprintf(sql, "delete MyEquip where ID = %d", id);
	executeUpdate(sql);
}

void DBManager::updateMyEquip(int id, bool isWear)
{
	char sql[512] = "\0";
	sprintf(sql, "update MyEquip set IsWear = '%s' where ID = %d", Value(isWear).asString().c_str(), id);
	executeUpdate(sql);
}