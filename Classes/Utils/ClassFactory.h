#pragma once

#include "Utils/PublicDefault.h"
#include "GameState/StateBase.h"
#include "Dlg/DlgEnum.h"
#include "Dlg/DlgBase.h"

class StateBase;

class ClassCreatorBase
{
public:
	ClassCreatorBase(){}
//    virtual Node* Create() = 0;
	virtual DlgBase* Create(StateBase* state) = 0;
};

template<class T>
class ClassCreator : public ClassCreatorBase
{
public:
	//Node* Create()
	//{
	//	return T::create();
	//}

	DlgBase* Create(StateBase* state)
	{
		return T::create(state);
	}
};


class ClassFactory
{
public:
	ClassFactory() {}
	~ClassFactory() {}

public:

	void regClass(const string& className, ClassCreatorBase* creator) {
		if (_classes.find(className) == _classes.end())
		{
			_classes.insert(make_pair(className, creator));
		}
	}
	
	DlgBase* createClass(StateBase* state, const string& className) {
		auto it = _classes.find(className);
		if (it == _classes.end()) {
			assert(true, "create class faile");
			return nullptr;
		}

		DlgBase* node = it->second->Create(state);
		return node;
	}
	
private:
    std::map<std::string, ClassCreatorBase*> _classes;
};