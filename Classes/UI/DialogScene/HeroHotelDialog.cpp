#include "HeroHotelDialog.h"


HeroHotelDialog* HeroHotelDialog::create()
{
    HeroHotelDialog *pRet = new(std::nothrow) HeroHotelDialog();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}


bool HeroHotelDialog::init()
{
    if ( !Layer::init() ) {
        return false;
    }
    
    this->setScale(0.0f);
    showDialog();

    return true;
}


void HeroHotelDialog::showDialog()
{
    // Action
    auto scale = ScaleTo::create(0.3f, 1.0f);
    auto act = EaseBackOut::create(scale);
    this->runAction(act);
    

    // ��Ӱ����
    auto WIN_SIZE = Director::getInstance()->getWinSize();
    auto bg = Sprite::create(IMG_GRAY_BG);
    bg->setScale(WIN_SIZE.width/1000.0, WIN_SIZE.height/1000.0);
    bg->setPosition(WIN_SIZE/2);
    this->addChild(bg, -1);
    bg->setOpacity(128);

    auto ui = GUIReader::getInstance()->widgetFromJsonFile(UI_DIALOG_HEROHOTEL);
    this->addChild(ui, 1, "UI");
    
    // �رհ�ť
    auto btnClose = Helper::seekWidgetByName(ui, "CloseButton");
    btnClose->addTouchEventListener(CC_CALLBACK_2(HeroHotelDialog::closeCallback, this));
    
    // ��Ϣ��ť
    auto btnInfo = Helper::seekWidgetByName(ui, "AragornInfoButton");
    btnInfo->addTouchEventListener(CC_CALLBACK_2(HeroHotelDialog::infoCallback, this));
    
    // ��ս��ť
    auto btnBattle = Helper::seekWidgetByName(ui, "AragornButton");
    btnBattle->addTouchEventListener(CC_CALLBACK_2(HeroHotelDialog::battleCallback, this));
    
    showInfo();
}


void HeroHotelDialog::showInfo()
{
    auto ui = (Widget*)this->getChildByName("UI");
    
    // ��ȡӢ����Ϣ
    ValueMap& map = DM()->getHero(1);
    
    auto lbLevel = (Text*)Helper::seekWidgetByName(ui, "AragornLevel");
    lbLevel->setString(map["Level"].asString());
    
}


void HeroHotelDialog::hideDialog()
{
    // Action
    auto scale = ScaleTo::create(0.3f, 0.0f);
    auto ease = EaseBackIn::create(scale);
    auto func = CallFunc::create(CC_CALLBACK_0(HeroHotelDialog::removeDialog, this));
    auto act = Sequence::create(ease, func, nullptr);
    this->runAction(act);
}


void HeroHotelDialog::removeDialog()
{
    this->removeFromParent();
}


void HeroHotelDialog::closeCallback(Ref *sender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED) {
        hideDialog();
    }
}


// ��ʾӢ����Ϣ
void HeroHotelDialog::infoCallback(Ref* sender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED) {
        auto dialog = HeroInfoDialog::create(HERO_TYPE_ARAGORN);
        this->getParent()->addChild(dialog, 999);
    }
}

// ��Ӣ�۲�ս
void HeroHotelDialog::battleCallback(Ref* sender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED) {
        auto notice = Notice::create("��Ӣ���ѽ����ս״̬~~~");
        this->getParent()->addChild(notice, 999);
        
        DM()->updateHeroExp(1, 100);
    }
}

