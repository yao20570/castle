#include "MsgMgr.h"

MsgMgr* MsgMgr::_g = nullptr;
MsgMgr* MsgMgr::getInstance()
{
	if (nullptr == _g) {
		_g = new MsgMgr();
		_g->init();
	}
	return _g;
}

MsgMgr::MsgMgr()
{
}

MsgMgr::~MsgMgr()
{
}

void MsgMgr::init()
{
}

void MsgMgr::addListener(const char* msgType, void* obj, function<void(void*)> callback){
	//添加消息列表
	auto it = _msgs.find(msgType);
	if (it == _msgs.end()){
		_msgs.emplace(msgType, map<void*, function<void(void*)>>());
		it = _msgs.find(msgType);
	}

	//注册对象侦听
	auto cb = it->second.find(obj);
	if (cb == it->second.end()){
		it->second.insert(pair<void*, function<void(void*)>>(obj, callback));
	}

}

void MsgMgr::delListener(const char* msgType, void* obj){
	auto it = _msgs.find(msgType);
	if (it == _msgs.end()){
		return;
	}

	auto cb = it->second.find(obj);
	if (cb != it->second.end()){
		it->second.erase(cb);
	}
}

void MsgMgr::dispatch(const char* msgType, void* data){
	auto it = _msgs.find(msgType);
	if (it == _msgs.end()){
		return;
	}

	for (auto& cb : it->second){
		cb.second(data);
	}
}