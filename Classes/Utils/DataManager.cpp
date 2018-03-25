#include "DataManager.h"


DataManager* DataManager::_g = nullptr;
DataManager* DataManager::getInstance()
{
    if (nullptr == _g) {
        _g = new DataManager();
        _g->init();
    }
    return _g;
}

DataManager::DataManager()
{
    _player.clear();
    _town.clear();
    _miwu.clear();
    
    _indexPlayerLevel.clear();
    _indexTown.clear();
    _indexMiwu.clear();
}

DataManager::~DataManager()
{
    _g = nullptr;
}

bool DataManager::init()
{
    loadPlayerInfo();
    loadPlayerLevelInfo();
    loadTownInfo();
    loadMiwuInfo();
    loadBuildingInfo();
    loadBuildingLimitInfo();
    loadSoilderInfo();
    loadSoilderLimitInfo();
    loadHeroInfo();
    loadSkillInfo();
    loadBattleInfo();
    
    return true;
}


//--------------------------------------------------------------------------------
// load
//--------------------------------------------------------------------------------

// ���������Ϣ
// sqlite   : PlayerInfo
void DataManager::loadPlayerInfo()
{
    // �����Ϣ��(sqlite)
    // loadCsvData(CSV_PLAYERINFO, _player);
    
    string sql = "select * from PlayerInfo";
    _player = DBM()->executeQuery(sql);
}


// ���صȼ���Ϣ
// csv      : PlayerLevelInfo
void DataManager::loadPlayerLevelInfo()
{
    loadCsvData(CSV_PLAYERLEVELINFO, _playerLevel);
    bindingIndex(_playerLevel, _indexPlayerLevel, "Level");
}


// ���سǳ���Ϣ
// csv      : TownInfo
// sqlite   : TownStateInfo
void DataManager::loadTownInfo()
{
    loadCsvData(CSV_TOWNINFO, _town);
    bindingIndex(_town, _indexTown, "TownID");
    
    // �ǳ�״̬��(sqlite)
//    Csv csv = Csv(CSV_TOWNSTATEINFO);
//    for (int i = 0; i < csv.getRowCount(); i++) {
//        string townID = csv[i]["TownID"];
//        ValueMap& map = _town[_indexTown[townID]].asValueMap();
//        map["Type"] = csv[i]["Type"];
//        map["LastHarvest"] = csv[i]["LastHarvest"];
//    }
    
    string sql = "select * from TownStateInfo";
    ValueVector vv = DBM()->executeQuery(sql);
    for (int i = 0; i < vv.size(); i++) {
        ValueMap& vm = vv.at(i).asValueMap();
        
        string townID = vm["TownID"].asString();
        ValueMap& map = _town[_indexTown[townID]].asValueMap();
        
        map["Type"] = vm["Type"].asString();
        map["LastHarvest"] = vm["LastHarvest"].asString();
    }
}


// ����������Ϣ
// csv      : MiwuInfo
// sqlite   : MiwuStateInfo
void DataManager::loadMiwuInfo()
{
    loadCsvData(CSV_MIWUINFO, _miwu);
    bindingIndex(_miwu, _indexMiwu, "MiwuID");
    
    // ����״̬��(sqlite)
//    Csv csv = Csv(CSV_MIWUSTATEINFO);
//    for (int i = 0; i < csv.getRowCount(); i++) {
//        string miwuID = csv[i]["MiwuID"];
//        ValueMap& map = _miwu[_indexMiwu[miwuID]].asValueMap();
//        map["Type"] = csv[i]["Type"];
//    }
    
    string sql = "select * from MiwuStateInfo";
    ValueVector vv = DBM()->executeQuery(sql);
    for (int i = 0; i < vv.size(); i++) {
        ValueMap& vm = vv.at(i).asValueMap();
        
        string miwuID = vm["MiwuID"].asString();
        ValueMap& map = _miwu[_indexMiwu[miwuID]].asValueMap();
        
        map["Type"] = vm["Type"].asString();
    }
}


// ���ؽ�����Ϣ
// csv      : BuildingInfo
// sqlite   : BuildingListInfo
void DataManager::loadBuildingInfo()
{
    // ��ȡ���н�����Ϣ
    loadCsvData(CSV_BUILDINGINFO, _buildingInfo);
    bindingIndex(_buildingInfo, _indexBuildingInfo, "BuildingID");

    
    // ��ȡ��ҽ����б�(sqlite)
//    Csv csv = Csv(CSV_BUILDINGLISTINFO);
//    for (int i = 0; i < csv.getRowCount(); i++) {
//        ValueMap map;
//        
//        // i == 0 Ϊ�ֶ�ͷ
//        if (i == 0) {
//            map["BuildingID"] = 0;
//            _building.push_back((Value)map);
//            continue;
//        }
//
//        //[1] ����BuildingID���� _buildingInfo �в��Ҷ�Ӧ��������Ϣ
//        int buildingID = GM()->getStrToInt(csv[i]["BuildingID"]);
//        ValueMap& buildingInfo = getBuildingInfo(buildingID);
//        for (auto it = buildingInfo.begin(); it != buildingInfo.end(); it++) {
//            string key = (*it).first;
//            string value = (*it).second.asString();
//            map[key] = value;
//        }
//        //[2] ��ȡ��ҽ�����Ϣ
//        for (int j = 0; j < csv[i].size(); j++) {
//            map[csv[0][j]] = csv[i][j];
//        }
//        //[3] ��ȡ���⽨����ID
//        if(map["Type"].asInt() == BUILDING_TYPE_BaseTower) {
//            _baseID = i; // �����±�
//        }
//        else if(map["Type"].asInt() == BUILDING_TYPE_Camp) {
//            _campID = i;
//        }
//        else if(map["Type"].asInt() == BUILDING_TYPE_ResearchLab) {
//            _labID = i;
//        }
//        
//        _building.push_back((Value)map);
//    }
    
    string sql = "select * from BuildingListInfo";
    ValueVector vv = DBM()->executeQuery(sql);
    for (int i = 0; i < vv.size(); i++) {
        ValueMap& vm = vv.at(i).asValueMap();

        ValueMap map;

        // i == 0 Ϊ�ֶ�ͷ
        if (i == 0) {
            map["BuildingID"] = 0;
            _building.push_back((Value)map);
            continue;
        }

        //[1] ����BuildingID���� _buildingInfo �в��Ҷ�Ӧ��������Ϣ
        int buildingID = GM()->getStrToInt(vm["BuildingID"].asString());
        ValueMap& buildingInfo = getBuildingInfo(buildingID);
        for (auto it = buildingInfo.begin(); it != buildingInfo.end(); it++) {
            string key = (*it).first;
            string value = (*it).second.asString();
            map[key] = value;
        }
        //[2] ��ȡ��ҽ�����Ϣ
        for (auto it = vm.begin(); it != vm.end(); it++) {
            map[it->first] = it->second.asString();
        }
        //[3] ��ȡ���⽨����ID
        if(map["Type"].asInt() == BUILDING_TYPE_BaseTower) {
            _baseID = i; // �����±�
        }
        else if(map["Type"].asInt() == BUILDING_TYPE_Camp) {
            _campID = i;
        }
        else if(map["Type"].asInt() == BUILDING_TYPE_ResearchLab) {
            _labID = i;
        }
        
        _building.push_back((Value)map);
    }
    
    bindingIndex(_building, _indexBuilding, "ID");
}


// ���ؽ������Ʊ���Ϣ��˾��ȼ���Ӧ������������
// csv      : BuildingLimitInfo
void DataManager::loadBuildingLimitInfo()
{
    loadCsvData(CSV_BUILDINGLIMITINFO, _buildingLimit);
    bindingIndex(_buildingLimit, _indexBuildingLimit, "Level");
}


// ����ʿ����Ϣ
// csv      : SoilderInfo
// sqlite   : SoilderListInfo
void DataManager::loadSoilderInfo()
{
    // ��ȡ����ʿ����Ϣ
    loadCsvData(CSV_SOILDER_INFO, _soilderInfo);
    bindingIndex(_soilderInfo, _indexSoilderInfo, "SoilderID");
    
    // ��ȡ���ʿ���б�(sqlite)
//    Csv csv = Csv(CSV_SOILDER_LISTINFO);
//    for (int i = 0; i < csv.getRowCount(); i++) {
//        ValueMap map;
//        
//        // i == 0 Ϊ�ֶ�ͷ
//        if (i == 0) {
//            map["SoilderID"] = 0;
//            _soilder.push_back((Value)map);
//            continue;
//        }
//        
//        //[1] ����SoilderID���� _soilderInfo �в��Ҷ�Ӧʿ������Ϣ
//        int soilderID = GM()->getStrToInt(csv[i]["SoilderID"]);
//        ValueMap& soilderInfo = getSoilderInfo(soilderID);
//        for (auto it = soilderInfo.begin(); it != soilderInfo.end(); it++) {
//            string key = (*it).first;
//            string value = (*it).second.asString();
//            map[key] = value;
//        }
//        //[2] ��ȡ���ʿ����Ϣ
//        for (int j = 0; j < csv[i].size(); j++) {
//            map[csv[0][j]] = csv[i][j];
//        }
//        
//        _soilder.push_back((Value)map);
//    }
    
    string sql = "select * from SoilderListInfo";
    ValueVector vv = DBM()->executeQuery(sql);
    for (int i = 0; i < vv.size(); i++) {
        ValueMap& vm = vv.at(i).asValueMap();

        ValueMap map;

        // i == 0 Ϊ�ֶ�ͷ
        if (i == 0) {
            map["SoilderID"] = 0;
            _soilder.push_back((Value)map);
            continue;
        }

        //[1] ����SoilderID���� _soilderInfo �в��Ҷ�Ӧʿ������Ϣ
        int soilderID = GM()->getStrToInt(vm["SoilderID"].asString());
        ValueMap& soilderInfo = getSoilderInfo(soilderID);
        for (auto it = soilderInfo.begin(); it != soilderInfo.end(); it++) {
            string key = (*it).first;
            string value = (*it).second.asString();
            map[key] = value;
        }
        //[2] ��ȡ���ʿ����Ϣ
        for (auto it = vm.begin(); it != vm.end(); it++) {
            map[it->first] = it->second.asString();
        }
        
        _soilder.push_back((Value)map);
    }

    bindingIndex(_soilder, _indexSoilder, "ID");
}


// ���ر�Ӫʿ���������Ʊ�
// csv  : SoilderLimitInfo
void DataManager::loadSoilderLimitInfo()
{
    loadCsvData(CSV_SOILDER_LIMITINFO, _soilderLimit);
    bindingIndex(_soilderLimit, _indexSoilderLimit, "Level");
}


// ����Ӣ����Ϣ
// csv      : HeroInfo
// sqlite   : HeroListInfo
void DataManager::loadHeroInfo()
{
    loadCsvData(CSV_HERO_INFO, _heroInfo);
    bindingIndex(_heroInfo, _indexHeroInfo, "HeroID");
    
    // �������Ӣ����Ϣ(sqlite)
//    Csv csv = Csv(CSV_HERO_LISTINFO);
//    for (int i = 0; i < csv.getRowCount(); i++) {
//        ValueMap map;
//        
//        // i == 0 Ϊ�ֶ�ͷ
//        if (i == 0) {
//            map["HeroID"] = 0;
//            _hero.push_back((Value)map);
//            continue;
//        }
//        
//        //[1] ����HeroID���� _HeroInfo �в��Ҷ�ӦӢ�۵���Ϣ
//        int heroID = GM()->getStrToInt(csv[i]["HeroID"]);
//        ValueMap& heroInfo = getHeroInfo(heroID);
//        for (auto it = heroInfo.begin(); it != heroInfo.end(); it++) {
//            string key = (*it).first;
//            string value = (*it).second.asString();
//            map[key] = value;
//        }
//        //[2] ��ȡ���Ӣ����Ϣ
//        for (int j = 0; j < csv[i].size(); j++) {
//            map[csv[0][j]] = csv[i][j];
//        }
//        
//        _hero.push_back((Value)map);
//    }
    
    string sql = "select * from HeroListInfo";
    ValueVector vv = DBM()->executeQuery(sql);
    for (int i = 0; i < vv.size(); i++) {
        ValueMap& vm = vv.at(i).asValueMap();

        ValueMap map;

        // i == 0 Ϊ�ֶ�ͷ
        if (i == 0) {
            map["HeroID"] = 0;
            _hero.push_back((Value)map);
            continue;
        }

        //[1] ����HeroID���� _HeroInfo �в��Ҷ�ӦӢ�۵���Ϣ
        int heroID = GM()->getStrToInt(vm["HeroID"].asString());
        ValueMap& heroInfo = getHeroInfo(heroID);
        for (auto it = heroInfo.begin(); it != heroInfo.end(); it++) {
            string key = (*it).first;
            string value = (*it).second.asString();
            map[key] = value;
        }
        //[2] ��ȡ���Ӣ����Ϣ
        for (auto it = vm.begin(); it != vm.end(); it++) {
            map[it->first] = it->second.asString();
        }
        
        _hero.push_back((Value)map);
    }
    
    bindingIndex(_hero, _indexHero, "ID");
}


// ���ؼ�����Ϣ
// csv      : SkillInfo
void DataManager::loadSkillInfo()
{
    loadCsvData(CSV_SKILL_INFO, _skillInfo);
    bindingIndex(_skillInfo, _indexSkillInfo, "SkillID");
}


// ���عؿ���Ϣ
void DataManager::loadBattleInfo()
{
    loadCsvData(CSV_BATTLE_INFO, _battleBuilding);
    
    for (int i = 1; i < _battleBuilding.size(); i++) {
        ValueMap& map = _battleBuilding.at(i).asValueMap();
        
        // ����BuildingID���� _buildingInfo �в��Ҷ�Ӧ��������Ϣ
        int buildingID = GM()->getStrToInt(map["BuildingID"].asString());
        ValueMap& buildingInfo = getBuildingInfo(buildingID);
        for (auto it = buildingInfo.begin(); it != buildingInfo.end(); it++) {
            string key = (*it).first;
            string value = (*it).second.asString();
            map[key] = value;
        }
    }
}


// ����CSV����
void DataManager::loadCsvData(std::string file, ValueVector& data)
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

// ���±�
void DataManager::bindingIndex(ValueVector &data, map<string, int>& index, string ID)
{
    for (int i = 0; i < data.size(); i++) {
        ValueMap map = data.at(i).asValueMap();
        index[map[ID].asString()] = i;
    }
}

// ��ӡ���
void DataManager::printData(ValueVector& data)
{
    for (int i = 0; i < data.size(); i++) {
        ValueMap map = data.at(i).asValueMap();
        for (auto it = map.begin(); it != map.end(); it++) {
            printf("%s:%s , ", (*it).first.c_str(), (*it).second.asString().c_str());
        }
        puts("");
    }
}


//--------------------------------------------------------------------------------
// update   :   need update sqlite
//--------------------------------------------------------------------------------

// ���������Ϣ
// sqlite   : PlayerInfo
void DataManager::updatePlayerExp(int exp)
{
    ValueMap& map = _player.at(1).asValueMap();
    
    int preExp = map["Exp"].asInt();
    int newExp = preExp + exp;
    int expRequire = getPlayerExpRequire(map["Level"].asInt());
    
    if (expRequire == -1) { // ������
        
    }
    else {
        // ����
        if (newExp >= expRequire) {
            map["Level"] = map["Level"].asInt() + 1;
            newExp -= expRequire;
        }
        map["Exp"] = newExp;
        
        // ���µȼ���������Ϣ(sqlite)
        string sql = "update PlayerInfo set Level='" + map["Level"].asString()
        + "', Exp='" + map["Exp"].asString()
        +"' where ID=" + map["ID"].asString() + ";";
//        CCLOG("%s", sql.c_str());
        DBM()->executeUpdate(sql);
    }
}

// ���½����Դ
// sqlite   : PlayerInfo
void DataManager::updateGold(int gold)
{
    ValueMap& map = _player.at(1).asValueMap();
    map["GoldCount"] = gold;
    
    // ���½����Դ(sqlite)
    string sql = "update PlayerInfo set GoldCount='" + map["GoldCount"].asString()
    +"' where ID=" + map["ID"].asString() + ";";
//    CCLOG("%s", sql.c_str());
    DBM()->executeUpdate(sql);
}

// ����ľ����Դ
// sqlite   : PlayerInfo
void DataManager::updateWood(int wood)
{
    ValueMap& map = _player.at(1).asValueMap();
    map["WoodCount"] = wood;
    
    // ����ľ����Դ(sqlite)
    string sql = "update PlayerInfo set WoodCount='" + map["WoodCount"].asString()
    +"' where ID=" + map["ID"].asString() + ";";
//    CCLOG("%s", sql.c_str());
    DBM()->executeUpdate(sql);
}

// ���½������
// sqlite   : PlayerInfo
void DataManager::updateGoldCapacity(int capacity)
{
    ValueMap& map = _player.at(1).asValueMap();
    map["GoldCapacity"] = capacity;
    
    // ���½������(sqlite)
    string sql = "update PlayerInfo set GoldCapacity='" + map["GoldCapacity"].asString()
    +"' where ID=" + map["ID"].asString() + ";";
//    CCLOG("%s", sql.c_str());
    DBM()->executeUpdate(sql);
}

// ����ľ������
// sqlite   : PlayerInfo
void DataManager::updateWoodCapacity(int capacity)
{
    ValueMap& map = _player.at(1).asValueMap();
    map["WoodCapacity"] = capacity;
    
    // ����ľ������(sqlite)
    string sql = "update PlayerInfo set WoodCapacity='" + map["WoodCapacity"].asString()
    +"' where ID=" + map["ID"].asString() + ";";
//    CCLOG("%s", sql.c_str());
    DBM()->executeUpdate(sql);
}

void DataManager::updateRing(int addRing)
{
    ValueMap& map = _player.at(1).asValueMap();
    map["RingCount"] = map["RingCount"].asInt() + addRing;
    
    // ����ָ������(sqlite)
    string sql = "update PlayerInfo set RingCount='" + map["RingCount"].asString()
    +"' where ID=" + map["ID"].asString() + ";";
    //    CCLOG("%s", sql.c_str());
    DBM()->executeUpdate(sql);
}


// ���³ǳ����͡��ջ�ʱ��
// sqlite   : TownStateInfo
void DataManager::updateTownType(int townID, int type)
{
    string stamp = GM()->getIntToStr(GM()->getTimeStamp());
    ValueMap& map = _town.at(_indexTown[GM()->getIntToStr(townID)]).asValueMap();
    map["Type"] = type;
    map["LastHarvest"] = stamp;
    
    // ���³ǳ����͡��ջ�ʱ��(sqlite)
    string sql = "update TownStateInfo set Type='" + map["Type"].asString()
    + "', LastHarvest='" + map["LastHarvest"].asString()
    +"' where TownID=" + map["TownID"].asString() + ";";
//    CCLOG("%s", sql.c_str());
    DBM()->executeUpdate(sql);
}

// ���³ǳ��ջ�ʱ��
// sqlite   : TownStateInfo
void DataManager::updateTownHarvest(int townID, int lastHarvest)
{
    ValueMap& map = _town.at(_indexTown[GM()->getIntToStr(townID)]).asValueMap();
    map["LastHarvest"] = lastHarvest;
    
    // ���³ǳ��ջ�ʱ��(sqlite)
    string sql = "update TownStateInfo set LastHarvest='" + map["LastHarvest"].asString()
    +"' where TownID=" + map["TownID"].asString() + ";";
//    CCLOG("%s", sql.c_str());
    DBM()->executeUpdate(sql);
}


// ������������
// sqlite   : MiwuStateInfo
void DataManager::updateMiwuType(int level)
{
    for (int i = 1; i < _miwu.size(); i++) {
        ValueMap& miwu = _miwu[i].asValueMap();
        if (miwu["Level"].asInt() <= level) {
            miwu["Type"] = MIWU_TYPE_UNLOCK;
            
            // ��������״̬(sqlite)
            string sql = "update MiwuStateInfo set Type='" + miwu["Type"].asString()
            +"' where MiwuID=" + miwu["MiwuID"].asString() + ";";
//            CCLOG("%s", sql.c_str());
            DBM()->executeUpdate(sql);
        }
    }
}


// ������ʩλ��
// sqlite   : BuildingListInfo
void DataManager::updateBuildingPos(int ID, Vec2 pos)
{
    ValueMap& map = _building.at(_indexBuilding[GM()->getIntToStr(ID)]).asValueMap();
    map["PositionX"] = (int)pos.x;
    map["PositionY"] = (int)pos.y;
    
    // ������ʩλ��(sqlite)
    string sql = "update BuildingListInfo set PositionX='" + map["PositionX"].asString()
    + "', PositionY='" + map["PositionY"].asString()
    +"' where ID=" + map["ID"].asString() + ";";
//    CCLOG("%s", sql.c_str());
    DBM()->executeUpdate(sql);
}


// �ı佨��״̬
// sqlite   : BuildingListInfo
void DataManager::updateBuildingBuildState(int ID, int state)
{
    ValueMap& map = _building.at(_indexBuilding[GM()->getIntToStr(ID)]).asValueMap();
    map["LastBuildTime"] = GM()->getTimeStamp();
    map["BuildState"] = state;
    
    // ������ʩλ��(sqlite)
    string sql = "update BuildingListInfo set LastBuildTime='" + map["LastBuildTime"].asString()
    + "', BuildState='" + map["BuildState"].asString()
    +"' where ID=" + map["ID"].asString() + ";";
//    CCLOG("%s", sql.c_str());
    DBM()->executeUpdate(sql);
}


// �ı佨���ȼ�
// sqlite   : BuildingListInfo
void DataManager::updateBuildingLevel(int ID)
{
    ValueMap& map = _building.at(_indexBuilding[GM()->getIntToStr(ID)]).asValueMap();
    map["BuildingID"] = map["BuildingID"].asInt() + 1;
    map["BuildState"] = BUILDING_STATE_FINISHED;
 
    // ������ֵ
    ValueMap& data = getBuildingInfo(map["BuildingID"].asInt());
    map["Level"]                = data["Level"].asInt();
    map["HealthPoint"]          = data["HealthPoint"].asInt();
    map["GoldRequire"]          = data["GoldRequire"].asInt();
    map["WoodRequire"]          = data["WoodRequire"].asInt();
    map["TimeRequire"]          = data["TimeRequire"].asInt();
    map["BaseLevelRequire"]     = data["BaseLevelRequire"].asInt();
    map["PlayerLevelRequire"]   = data["PlayerLevelRequire"].asInt();
    map["ExpReward"]            = data["ExpReward"].asInt();
    // ��������
    map["GoldProduct"]          = data["GoldProduct"].asInt();
    map["WoodProduct"]          = data["WoodProduct"].asInt();
    // ��Դ����
    map["GoldCapacity"]         = data["GoldCapacity"].asInt();
    map["WoodCapacity"]         = data["WoodCapacity"].asInt();
    // ��������
    map["Damage"]               = data["Damage"].asInt();
    map["AttackSpeed"]          = data["AttackSpeed"].asInt();
    map["ShootRange"]           = data["ShootRange"].asInt();
    map["DamageRange"]          = data["DamageRange"].asInt();
    
    
    // ������ʩ�ȼ�(sqlite)
    // �������ݿ� : BuildingID / BuildState
    string sql = "update BuildingListInfo set BuildingID='" + map["BuildingID"].asString()
    + "', BuildState='" + map["BuildState"].asString()
    +"' where ID=" + map["ID"].asString() + ";";
//    CCLOG("%s", sql.c_str());
    DBM()->executeUpdate(sql);
}

// ����ջ�ʱ��
// sqlite   : BuildingListInfo
void DataManager::updateLastGoldHarvest(int ID, int lastHarvest)
{
    ValueMap& map = _building.at(_indexBuilding[GM()->getIntToStr(ID)]).asValueMap();
    map["LastGoldHarvest"] = lastHarvest;
    
    // ������ʩ�ȼ�(sqlite)
    string sql = "update BuildingListInfo set LastGoldHarvest='" + map["LastGoldHarvest"].asString()
    +"' where ID=" + map["ID"].asString() + ";";
//    CCLOG("%s", sql.c_str());
    DBM()->executeUpdate(sql);
}

// ľ���ջ�ʱ��
// sqlite   : BuildingListInfo
void DataManager::updateLastWoodHarvest(int ID, int lastHarvest)
{
    ValueMap& map = _building.at(_indexBuilding[GM()->getIntToStr(ID)]).asValueMap();
    map["LastWoodHarvest"] = lastHarvest;
    
    // ������ʩ�ȼ�(sqlite)
    string sql = "update BuildingListInfo set LastWoodHarvest='" + map["LastWoodHarvest"].asString()
    +"' where ID=" + map["ID"].asString() + ";";
//    CCLOG("%s", sql.c_str());
    DBM()->executeUpdate(sql);
}

// �½�����
// sqlite   : BuildingListInfo
int DataManager::addNewBuilding(int type, Vec2 pos)
{
    int ID = _building.size();
    int buildingID = 10000 + type * 100 + 01;
    int positionX = pos.x + EPS;
    int positionY = pos.y + EPS;
    int buildState = BUILDING_STATE_FINISHED;
    int lastBuildTime = GM()->getTimeStamp();
    int lastGoldHarvest = lastBuildTime;
    int lastWoodHarvest = lastBuildTime;
    
    ValueMap map;
    
    //[1] ����BuildingID���� _buildingInfo �в��Ҷ�Ӧ��������Ϣ
    ValueMap& buildingInfo = getBuildingInfo(buildingID);
    for (auto it = buildingInfo.begin(); it != buildingInfo.end(); it++) {
        string key = (*it).first;
        string value = (*it).second.asString();
        map[key] = value;
    }
    //[2] ��ȡ��ҽ�����Ϣ
    map["ID"] = ID;
    map["BuildingID"] = buildingID;
    map["PositionX"] = positionX;
    map["PositionY"] = positionY;
    map["BuildState"] = buildState;
    map["LastBuildTime"] = lastBuildTime;
    map["LastGoldHarvest"] = lastGoldHarvest;
    map["LastWoodHarvest"] = lastWoodHarvest;
    //[3] ����_building��
    _indexBuilding[map["ID"].asString()] = ID;
    _building.push_back((Value)map);
    
    
    // ��������ʩ(sqlite)
    string sql = "insert into BuildingListInfo values("
    + map["ID"].asString() + ", '"
    + map["BuildingID"].asString() + "', '"
    + map["PositionX"].asString() + "', '"
    + map["PositionY"].asString() + "', '"
    + map["BuildState"].asString() + "', '"
    + map["LastBuildTime"].asString() + "', '"
    + map["LastGoldHarvest"].asString() + "', '"
    + map["LastWoodHarvest"].asString() + "')";
//    CCLOG("%s", sql.c_str());
    DBM()->executeUpdate(sql);
    
    return ID;
}


// ����ʿ��������+delta
// sqlite   : SoilderListInfo
void DataManager::updateSoilderCount(int ID, int delta)
{
    string id = GM()->getIntToStr(ID);
    ValueMap& map = _soilder[ _indexSoilder[id] ].asValueMap();
    map["Count"] = map["Count"].asInt() + delta;
    
    // ����ʿ������(sqlite)
    string sql = "update SoilderListInfo set Count='" + map["Count"].asString()
    +"' where ID=" + map["ID"].asString() + ";";
//    CCLOG("%s", sql.c_str());
    DBM()->executeUpdate(sql);
}

// ����ʿ���ȼ�
// sqlite   : SoilderListInfo
void DataManager::updateSoilderLevel(int ID)
{
    ValueMap& map = _soilder.at(_indexSoilder[GM()->getIntToStr(ID)]).asValueMap();
    map["SoilderID"] = map["SoilderID"].asInt() + 1;
    
    // ������ֵ
    ValueMap& data = getSoilderInfo(map["SoilderID"].asInt());
    map["Level"]                = data["Level"].asInt();
    map["HealthPoint"]          = data["HealthPoint"].asInt();
    map["Damage"]               = data["Damage"].asInt();
    map["AttackSpeed"]          = data["AttackSpeed"].asInt();
    map["ShootRange"]           = data["ShootRange"].asInt();
    map["LabLevelRequire"]      = data["LabLevelRequire"].asInt();
    map["GoldRequire"]          = data["GoldRequire"].asInt();
    
    // ����ʿ���ȼ�(sqlite)
    // �������ݿ� SoilderID
    string sql = "update SoilderListInfo set SoilderID='" + map["SoilderID"].asString()
    +"' where ID=" + map["ID"].asString() + ";";
//    CCLOG("%s", sql.c_str());
    DBM()->executeUpdate(sql);
}


// ����Ӣ�۾���ֵ������
// sqlite   : HeroListInfo
void DataManager::updateHeroExp(int ID, int addExp)
{
    string id = GM()->getIntToStr(ID);
    ValueMap& map = _hero[ _indexHero[id] ].asValueMap();
    map["Exp"] = map["Exp"].asInt() + addExp;
    
    while (map["Exp"].asInt() >= map["ExpRequire"].asInt()) {
        map["Exp"] = map["Exp"].asInt() - map["ExpRequire"].asInt();
        updateHeroLevel(ID);
    }
    
    // ����Ӣ�۾���(sqlite)
    string sql = "update HeroListInfo set Exp='" + map["Exp"].asString()
    +"' where ID=" + map["ID"].asString() + ";";
//    CCLOG("%s", sql.c_str());
    DBM()->executeUpdate(sql);
}

// ����Ӣ�۵ȼ�
void DataManager::updateHeroLevel(int ID)
{
    string id = GM()->getIntToStr(ID);
    ValueMap& map = _hero[ _indexHero[id] ].asValueMap();
    int heroID = map["HeroID"].asInt();
    
    ValueMap& data = getHeroNextInfo(heroID);
    map["HeroID"]       = data["HeroID"];
    map["Level"]        = data["Level"];
    map["HealthPoint"]  = data["HealthPoint"].asInt();
    map["Damage"]       = data["Damage"].asInt();
    map["AttackSpeed"]  = data["AttackSpeed"].asInt();
    map["ShootRange"]   = data["ShootRange"].asInt();
    map["ExpRequire"]   = data["ExpRequire"].asInt();
    
    // ����Ӣ�۵ȼ�(sqlite)
    // �������ݿ� ��HeroID
    string sql = "update HeroListInfo set HeroID='" + map["HeroID"].asString()
    +"' where ID=" + map["ID"].asString() + ";";
//    CCLOG("%s", sql.c_str());
    DBM()->executeUpdate(sql);
}


//--------------------------------------------------------------------------------
// get
//--------------------------------------------------------------------------------

// ��ȡ��Ҿ�������
int DataManager::getPlayerExpRequire(int level)
{
    string lev = GM()->getIntToStr(level);
    return (_playerLevel[ _indexPlayerLevel[lev] ].asValueMap() )["ExpRequire"].asInt();
}
// ��ȡ��ҵȼ�
int DataManager::getPlayerLevel()
{
    ValueMap& map = _player.at(1).asValueMap();
    return map["Level"].asInt();
}

// ��ȡ�����Դ����
int DataManager::getGoldCount()
{
    ValueMap& map = _player.at(1).asValueMap();
    return map["GoldCount"].asInt();
}
// ��ȡľ����Դ����
int DataManager::getWoodCount()
{
    ValueMap& map = _player.at(1).asValueMap();
    return map["WoodCount"].asInt();
}
// ��ȡ�����Դ����
int DataManager::getGoldCapacity()
{
    ValueMap& map = _player.at(1).asValueMap();
    return map["GoldCapacity"].asInt();
}

// ��ȡľ����Դ����
int DataManager::getWoodCapacity()
{
    ValueMap& map = _player.at(1).asValueMap();
    return map["WoodCapacity"].asInt();
}


// ��ȡ�ǳ���Ϣ
ValueMap& DataManager::getTownInfo(int townID)
{
    string id = GM()->getIntToStr(townID);
    return _town[ _indexTown[id] ].asValueMap();
}


// �����б�ID��ȡ������Ϣ
ValueMap& DataManager::getBuilding(int ID)
{
    string id = GM()->getIntToStr(ID);
    return _building[ _indexBuilding[id] ].asValueMap();
}


// ���ݽ���ID����ȡ������Ϣ
ValueMap& DataManager::getBuildingInfo(int buildingID)
{
    string id = GM()->getIntToStr(buildingID);
    return _buildingInfo[ _indexBuildingInfo[id] ].asValueMap();
}

// ����˾��ȼ�����ȡ��������
ValueMap& DataManager::getBuildingLimit(int level)
{
    string lv = GM()->getIntToStr(level);
    return _buildingLimit[ _indexBuildingLimit[lv] ].asValueMap();
}

// ��ȡ��ʩ��һ�ȼ���Ϣ
ValueMap& DataManager::getBuildingNextInfo(int buildingID)
{
    string id = GM()->getIntToStr(buildingID + 1);
    return _buildingInfo[ _indexBuildingInfo[id] ].asValueMap();
}

// ��ȡ˾��ȼ�
int DataManager::getBaseTowerLevel()
{
    ValueMap& map = _building[_baseID].asValueMap();
    return map["Level"].asInt();
}

// ��ȡĳ��type�Ľ�������
int DataManager::getBuildingCountForType(int type)
{
    int cnt = 0;
    
    for (int i = 0; i < _building.size(); i++) {
        ValueMap& map = _building[i].asValueMap();
        if (map["Type"].asInt() == type) {
            cnt++;
        }
    }
    
    return cnt;
}


// �����б�ID����ȡʿ��
ValueMap& DataManager::getSoilder(int ID)
{
    string id = GM()->getIntToStr(ID);
    return _soilder[ _indexSoilder[id] ].asValueMap();
}

// ����SoilderID����ȡʿ����Ϣ
ValueMap& DataManager::getSoilderInfo(int soilderID)
{
    string id = GM()->getIntToStr(soilderID);
    return _soilderInfo[ _indexSoilderInfo[id] ].asValueMap();
}

// ��ȡʿ����һ�ȼ���Ϣ
ValueMap& DataManager::getSoilderNextInfo(int soilderID)
{
    string id = GM()->getIntToStr(soilderID + 1);
    return _soilderInfo[ _indexSoilderInfo[id] ].asValueMap();
}

// ��ȡ��Ӫ��������
int DataManager::getSoilderLimit(int level)
{
    string lv = GM()->getIntToStr(level);
    ValueMap& map = _soilderLimit[ _indexSoilderLimit[lv] ].asValueMap();
    return map["SoilderCapacity"].asInt();
}

// ��ȡ��Ӫ�ȼ�
int DataManager::getCampLevel()
{
    ValueMap& map = _building[_campID].asValueMap();
    return map["Level"].asInt();
}

// ��ȡ�о����ȼ�
int DataManager::getLabLevel()
{
    ValueMap& map = _building[_labID].asValueMap();
    return map["Level"].asInt();
}



// ����Ӣ��ID����ȡ���Ӣ����Ϣ
ValueMap& DataManager::getHero(int ID)
{
    string id = GM()->getIntToStr(ID);
    return _hero[ _indexHero[id] ].asValueMap();
}


// ����HeroID����ȡӢ����Ϣ
ValueMap& DataManager::getHeroInfo(int heroID)
{
    string id = GM()->getIntToStr(heroID);
    return _heroInfo[ _indexHeroInfo[id] ].asValueMap();
}


// ��ȡӢ����һ�ȼ���Ϣ
ValueMap& DataManager::getHeroNextInfo(int heroID)
{
    string id = GM()->getIntToStr(heroID + 1);
    return _heroInfo[ _indexHeroInfo[id] ].asValueMap();
}


// ��ȡ������Ϣ
ValueMap& DataManager::getSkillInfo(int skillID)
{
    string id = GM()->getIntToStr(skillID);
    return _skillInfo[ _indexSkillInfo[id] ].asValueMap();
}


// ��ȡĳ�ؿ���Ϣ
ValueVector DataManager::getBattleBuilding(int townID)
{
    return _battleBuilding;
}

