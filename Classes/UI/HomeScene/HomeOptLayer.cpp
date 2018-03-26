#include "HomeOptLayer.h"
#include "Model/BuildingSprite.h"
#include "UI/DialogScene/BuildingInfoDialog.h"
#include "UI/DialogScene/BuildingUpgradeDialog.h"
#include "UI/DialogScene/LabDialog.h"
#include "UI/DialogScene/CampDialog.h"
#include "UI/DialogScene/HeroHotelDialog.h"

bool HomeOptLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    loadData();
    showUI();
    
    return true;
}

void HomeOptLayer::loadData()
{
    
}

void HomeOptLayer::showUI()
{
    Size WIN_SIZE = Director::getInstance()->getWinSize();
    
    // ��ť
    btnInfo = Button::create(IMG_BUTTON_INFOOPT);
    btnUpgrade = Button::create(IMG_BUTTON_UPGRADEOPT);
    btnEnter = Button::create(IMG_BUTTON_ENTEROPT);
    
    btnInfo->setPosition(Vec2(WIN_SIZE.width/2 - 120, -120));
    btnUpgrade->setPosition(Vec2(WIN_SIZE.width/2, -120));
    btnEnter->setPosition(Vec2(WIN_SIZE.width/2 + 120, -120));
    
    btnInfo->addTouchEventListener(CC_CALLBACK_2(HomeOptLayer::infoCallback, this));
    btnUpgrade->addTouchEventListener(CC_CALLBACK_2(HomeOptLayer::upgradeCallback, this));
    btnEnter->addTouchEventListener(CC_CALLBACK_2(HomeOptLayer::enterCallback, this));
    
    this->addChild(btnInfo);
    this->addChild(btnUpgrade);
    this->addChild(btnEnter);
    
    // ����
    auto lbInfo = Text::create("��Ϣ", FONT_ARIAL, 20);
    auto lbUpgrade = Text::create("����", FONT_ARIAL, 20);
    auto lbEnter = Text::create("����", FONT_ARIAL, 20);
    lbInfo->setPosition(Vec2(btnInfo->getContentSize().width/2, lbInfo->getContentSize().height));
    lbUpgrade->setPosition(Vec2(btnUpgrade->getContentSize().width/2, lbUpgrade->getContentSize().height));
    lbEnter->setPosition(Vec2(btnEnter->getContentSize().width/2, lbEnter->getContentSize().height));
    btnInfo->addChild(lbInfo);
    btnUpgrade->addChild(lbUpgrade, 0, "lbUpgrade");
    btnEnter->addChild(lbEnter);
    
    // ����
    label = Text::create("˾�", FONT_ARIAL, 32);
    label->setPosition(Vec2(WIN_SIZE.width/2, 140));
    this->addChild(label, 1);
    label->setOpacity(0);
}


// IDΪ��ʩ��ID
void HomeOptLayer::show(BuildingSprite* buildingSprite)
{
    _buildingSprite = nullptr;
    _buildingSprite = buildingSprite;

    Size WIN_SIZE = Director::getInstance()->getWinSize();

    btnInfo->stopAllActions();
    btnUpgrade->stopAllActions();
    btnEnter->stopAllActions();
    label->stopAllActions();
    
    // ���ñ���
    label->setOpacity(255);
    label->setColor(Color3B::WHITE);
    string title = _buildingSprite->_name + "(�ȼ�" + GM()->getIntToStr(_buildingSprite->_level) + ")";
    label->setString(title);
    
    // �Ƿ�����,����������
    auto lbUpgrade = (Text*)btnUpgrade->getChildByName("lbUpgrade");
    if (_buildingSprite->_baseLevelRequire != -1) {
        if (_buildingSprite->_buildState == BUILDING_STATE_BUILDING) {
            lbUpgrade->setString("������");
            btnUpgrade->loadTextureNormal(IMG_BUTTON_CANTUPGRADEOPT);
        }
        else {
            lbUpgrade->setString("����");
            btnUpgrade->loadTextureNormal(IMG_BUTTON_UPGRADEOPT);
        }
    }
    else {
        lbUpgrade->setString("������");
        btnUpgrade->loadTextureNormal(IMG_BUTTON_CANTUPGRADEOPT);
    }
    
    // �о�������Ӫ��Ӣ���ùݿɽ���
    if (_buildingSprite->_type != BUILDING_TYPE_Camp
        && _buildingSprite->_type != BUILDING_TYPE_HeroHotel
        && _buildingSprite->_type != BUILDING_TYPE_ResearchLab) {
        btnInfo->setPosition(Vec2(WIN_SIZE.width/2 - 60, -120));
        btnUpgrade->setPosition(Vec2(WIN_SIZE.width/2 + 60, -120));
        btnEnter->setPosition(Vec2(0, -120));
        showAction(btnInfo, 0.1f);
        showAction(btnUpgrade, 0.1f);
    }
    else {
        btnInfo->setPosition(Vec2(WIN_SIZE.width/2 - 120, -120));
        btnUpgrade->setPosition(Vec2(WIN_SIZE.width/2, -120));
        btnEnter->setPosition(Vec2(WIN_SIZE.width/2 + 120, -120));
        showAction(btnInfo, 0.1f);
        showAction(btnUpgrade, 0.1f);
        showAction(btnEnter, 0.1f);
    }
}
void HomeOptLayer::showAction(Button* btn, float delay)
{
    btn->runAction(MoveBy::create(delay, Vec2(0, 180)));
}


void HomeOptLayer::hide(BuildingSprite* buildingSprite)
{
    _buildingSprite = nullptr;

    btnInfo->stopAllActions();
    btnUpgrade->stopAllActions();
    btnEnter->stopAllActions();
    label->stopAllActions();
    
    hideAction(btnInfo, 0.1f);
    hideAction(btnUpgrade, 0.1f);
    hideAction(btnEnter, 0.1f);
    label->setOpacity(0);
}
void HomeOptLayer::hideAction(Button* btn, float delay)
{
    btn->runAction(MoveBy::create(delay, Vec2(0, -200)));
}


void HomeOptLayer::infoCallback(Ref* sender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED) {
        if (_buildingSprite != nullptr) {
//            CCLOG("infoCallback");
            auto infoDialog = BuildingInfoDialog::create(_buildingSprite);
            this->getParent()->addChild(infoDialog, 99);
        }
    }
}

void HomeOptLayer::upgradeCallback(Ref* sender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED) {
        if (_buildingSprite != nullptr) {
            if(_buildingSprite->_baseLevelRequire != -1
               && _buildingSprite->_buildState != BUILDING_STATE_BUILDING) {
                auto upgradeDialog = BuildingUpgradeDialog::create(_buildingSprite);
                this->getParent()->addChild(upgradeDialog, 99);
            }
            else {
                // ������ or ��������
            }
        }
    }
}

void HomeOptLayer::enterCallback(Ref* sender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED) {
        if (_buildingSprite != nullptr) {
            // �о���
            if(_buildingSprite->_type == BUILDING_TYPE_ResearchLab) {
                auto dialog = LabDialog::create();
                this->getParent()->addChild(dialog, 99);
            }
            // ��Ӫ
            else if(_buildingSprite->_type == BUILDING_TYPE_Camp) {
                auto dialog = CampDialog::create();
                this->getParent()->addChild(dialog, 99);
            }
            // Ӣ���ù�
            else if(_buildingSprite->_type == BUILDING_TYPE_HeroHotel) {
                auto dialog = HeroHotelDialog::create();
                this->getParent()->addChild(dialog, 99);
            }
        }
    }
}

