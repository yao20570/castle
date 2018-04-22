#pragma once

#include "Utils/PublicDefault.h"

class MessageTip 
{
public:
	MessageTip(Node* tipParent);	
	virtual ~MessageTip();

	void addTip(string tip);

private:
	void showTip();

protected:
	Node* _tipParent;
	list<Layout*> uis;
	list<string> tips;
};