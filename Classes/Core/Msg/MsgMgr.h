#pragma once

#include "Utils/PublicDefault.h"

#include "Core/Msg/MsgDefine.h"

#define MsgCenter() MsgMgr::getInstance()

class MsgMgr
{
public:
	MsgMgr();
	~MsgMgr();	
	static MsgMgr* getInstance();
	void init();

	void addListener(const char* msgType, void* obj, function<void(void*)> callback);
	void delListener(const char* msgType, void* obj);

	void dispatch(const char* msgType, void* data);
private:
	static MsgMgr* _g;

	map<const char*, map<void*, function<void(void*)>>> _msgs;
};
