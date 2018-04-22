#include "Dlg/Main/MessageTip.h"




MessageTip::MessageTip(Node* tipParent)
	:_tipParent(tipParent)
{
}

MessageTip::~MessageTip() {

}


void MessageTip::addTip(string tip) {
	this->tips.push_back(tip);

	this->showTip();
}

void MessageTip::showTip() {

	//��ȡtip������
	string tip = "";
	{
		auto it = tips.begin();
		if (it == tips.end()) {
			return;
		}
		else {
			tip = *it;
			tips.pop_front();
		}
	}

	//��ȡչʾtip��UI
	Layout* uiTip = nullptr;
	{
		auto it = uis.begin();
		if (it == uis.end()) {
			uiTip = (Layout*)GUIReader::getInstance()->widgetFromJsonFile("ui/DlgMain/message_tip.json");
			uiTip->setAnchorPoint(Vec2(0.5, 0.5));
			uiTip->setName("message_tip");
			this->_tipParent->addChild(uiTip);
		}
		else {
			uiTip = *it;
			uis.pop_front();
		}
		Size winSize = Director::getInstance()->getWinSize();
		uiTip->setPositionX(winSize.width / 2);
		uiTip->setPositionY(winSize.height / 2);
		uiTip->setVisible(true);

		Sequence* seq = Sequence::create(
			MoveTo::create(0.4, uiTip->getPosition() + Vec2(0, 50)),
			//CCCallFunc::create([uiTip, this]() { this->showTip(); }),
			DelayTime::create(0.4),
			CCCallFunc::create([uiTip, this]() { uiTip->setVisible(false); this->uis.push_back(uiTip); }),
			nullptr);
		uiTip->runAction(seq);
	}
	
	//����ui��tip����
	Text* labContent = (Text*)Helper::seekWidgetByName(uiTip, "lab_content");
	labContent->setString(tip);
}

