#include "JsonManager.h"

static const char* SkillJsonPath = "Config/Json/Skill.Json";


JsonManager* JsonManager::_g = nullptr;
JsonManager* JsonManager::getInstance()
{
	if (nullptr == _g) {
		_g = new JsonManager();
		_g->init();
	}
	return _g;
}

JsonManager::JsonManager()
{

}

JsonManager::~JsonManager()
{
	_g = nullptr;
}

bool JsonManager::init()
{
	return true;
}

rapidjson::Document& JsonManager::loadJson(std::string filePath){
	auto it = tables.find(filePath);
	if (it != tables.end()) {
		rapidjson::Document& doc = it->second;
		return doc;
	}

	auto ret = tables.emplace(filePath, rapidjson::Document());
	rapidjson::Document& doc = ret.first->second;
	string jsonstr = FileUtils::getInstance()->getStringFromFile(filePath);
	doc.Parse<rapidjson::kParseDefaultFlags>(jsonstr.c_str());

	return doc;

}

rapidjson::Value& JsonManager::getSkillById(int id){
	auto& doc = this->loadJson(SkillJsonPath);
	rapidjson::Value& list = doc["List"];
	rapidjson::Value& obj = list[cocos2d::Value(id).asString().c_str()];

	return obj;
}