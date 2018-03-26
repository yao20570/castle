#pragma once

#include "Utils/PublicDefault.h"

class ClassCreatorBase
{
public:
	ClassCreatorBase(){}
    virtual Node* Create() = 0;
};

template<class T>
class ClassCreator : public ClassCreatorBase
{
public:
	Node* Create()
	{
		return T::create();
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
	
	Node* createClass(const string& className) {
		auto it = _classes.find(className);
		if (it == _classes.end()) {
			assert(true, "create class faile");
			return nullptr;
		}

		Node* node = it->second->Create();
		return node;
	}
	
private:
    std::map<std::string, ClassCreatorBase*> _classes;
};