#include "ConfigMgr.h"

static const char* PathObjPosition = "Config/ObjPosition.csv";
static const char* PathObjInfo = "Config/ObjInfo.csv";
static const char* PathHeroInfo = "Config/HeroInfo.csv";
static const char* PathSoilderInfo = "Config/SoilderInfo.csv";

ConfigMgr* ConfigMgr::_g = nullptr;
ConfigMgr* ConfigMgr::getInstance()
{
	if (nullptr == _g) {
		_g = new ConfigMgr();
		_g->init();
	}
	return _g;
}

ConfigMgr::ConfigMgr()
{

}

ConfigMgr::~ConfigMgr()
{
	_g = nullptr;
}

bool ConfigMgr::init()
{
	return true;
}

map<int, ValueMap>* ConfigMgr::loadConfig(std::string file, const string& key) 
{
	auto it = tables.find(file);
	if (it != tables.end()) {
		map<int, ValueMap>& table = it->second;
		return &table;
	}

	auto ret = tables.emplace(file, map<int, ValueMap>());
	map<int, ValueMap>& table = ret.first->second;
	Csv csv = Csv(file.c_str());
	for (int i = 1; i < csv.getRowCount(); i++) {
		ValueMap map;
		for (int j = 0; j < csv[i].size(); j++) {
			map[csv[0][j]] = csv[i][j];
		}
		int k = map[key].asInt();
		table.emplace(k, map);
	}

	return &table;
}

map<int, ValueMap>* ConfigMgr::getObjPos() {
	return loadConfig(PathObjPosition, "ID");
}

ValueMap* ConfigMgr::getHeroInfoById(int id) {
	map<int, ValueMap>* table = loadConfig(PathHeroInfo, "HeroID");
	auto it = table->find(id);
	if (it == table->end()) {
		return nullptr;
	}
	return &(it->second);
}

ValueMap* ConfigMgr::getSoilderInfoById(int id) {
	map<int, ValueMap>* table = loadConfig(PathSoilderInfo, "ID");
	auto it = table->find(id);
	if (it == table->end()) {
		return nullptr;
	}
	return &(it->second);
}

ValueMap* ConfigMgr::getObjInfoById(int id) {
	map<int, ValueMap>* table = loadConfig(PathObjInfo, "ID");
	auto it = table->find(id);
	if (it == table->end()) {
		return nullptr;
	}
	return &(it->second);
}
