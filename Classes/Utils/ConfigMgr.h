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
	string getWord(int id);

	map<int, ValueMap>* getObjPos();

	ValueMap* getHeroInfoById(int id);
	ValueMap* getSoilderInfoById(int id);
	ValueMap* getObjInfoById(int id);
	ValueMap* getObjInfoByType(int mainType, int subType);

	ValueMap* getEquipInfoById(int id);

	ValueMap* getMissionMainById(int id);
	ValueMap* getMissionSumById(int id);
	ValueMap* getMissionObjById(int id);

	ValueMap* getSkillEffectById(int id);

	map<int, ValueMap>* getPlayerById(int id);
private:
	static ConfigMgr* _g;
	sqlite3* _pdb;
	map<string, map<int, ValueMap>> tables;

};
