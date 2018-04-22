#pragma once

#include "Utils/Public.h"

#define CFG() ConfigMgr::getInstance()

class ConfigMgr : public cocos2d::Ref
{
public:
	ConfigMgr();
	virtual ~ConfigMgr();
	virtual bool init();
	static ConfigMgr* getInstance();
	map<int, ValueMap>* loadConfig(std::string file, const string& key);

public:
	map<int, ValueMap>* getObjPos();

	ValueMap* getHeroInfoById(int id);
	ValueMap* getSoilderInfoById(int id);
	ValueMap* getObjInfoById(int id);

	ValueMap* getEquipInfoById(int id);
private:
	static ConfigMgr* _g;
	sqlite3* _pdb;
	map<string, map<int, ValueMap>> tables;

};
