#pragma once

#include "json/rapidjson.h"
#include "json/document.h"//前两个解析
#include "json/writer.h"
#include "json/stringbuffer.h"//后两个做添加修改删除操作


#include "Utils/Public.h"


using namespace rapidjson;

#define JsonMgr() JsonManager::getInstance()

class JsonManager : public cocos2d::Ref
{
public:
	JsonManager();
	virtual ~JsonManager();
	virtual bool init();
	static JsonManager* getInstance();
	rapidjson::Document& loadJson(std::string file);

public:
	rapidjson::Value* getSkillById(int id);

private:
	static JsonManager* _g;
	map<string, rapidjson::Document> tables;

};
