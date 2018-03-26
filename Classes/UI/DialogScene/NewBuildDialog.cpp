#include "NewBuildDialog.h"
#include "Model/NewBuilding.h"
#include "Model/Notice.h"
#include "UI/HomeScene/HomeScene.h"
#include "UI/HomeScene/HomeMapLayer.h"

NewBuildDialog* NewBuildDialog::create()
{
    NewBuildDialog *pRet = new(std::nothrow) NewBuildDialog();
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


bool NewBuildDialog::init()
{
    if ( !Layer::init() ) {
        return false;
    }
    
    this->setScale(0.0f);
    showDialog();
    
    return true;
}


void NewBuildDialog::showDialog()
{
    // Action
    auto scale = ScaleTo::create(0.3f, 1.0f);
    auto act = EaseBackOut::create(scale);
    this->runAction(act);
    
    auto ui = GUIReader::getInstance()->widgetFromJsonFile(UI_DIALOG_NEWBUILD);
    this->addChild(ui, 1, "UI");
    
    // �رհ�ť
    auto btnClose = Helper::seekWidgetByName(ui, "CloseButton");
    btnClose->addTouchEventListener(CC_CALLBACK_2(NewBuildDialog::closeCallback, this));
    
    // ������ʩ��ť
    auto btnMine = Helper::seekWidgetByName(ui, "MineFactoryButton");
    auto btnWood = Helper::seekWidgetByName(ui, "WoodFactoryButton");
    auto btnArrow = Helper::seekWidgetByName(ui, "ArrowTowerButton");
    auto btnCannon = Helper::seekWidgetByName(ui, "CannonButton");
    auto btnLaser = Helper::seekWidgetByName(ui, "LaserButton");
    btnMine->addTouchEventListener(CC_CALLBACK_2(NewBuildDialog::buildCallback, this));
    btnWood->addTouchEventListener(CC_CALLBACK_2(NewBuildDialog::buildCallback, this));
    btnArrow->addTouchEventListener(CC_CALLBACK_2(NewBuildDialog::buildCallback, this));
    btnCannon->addTouchEventListener(CC_CALLBACK_2(NewBuildDialog::buildCallback, this));
    btnLaser->addTouchEventListener(CC_CALLBACK_2(NewBuildDialog::buildCallback, this));
    
    showInfo();
}


void NewBuildDialog::showInfo()
{
    // ���������б� BuildingLimitInfo
    auto baseLevel = DM()->getBaseTowerLevel();
    auto limitMap = DM()->getBuildingLimit(baseLevel);
    
    
    auto ui = (Widget*)this->getChildByName("UI");
    
    // ������Դ����
    gold = (Text*)Helper::seekWidgetByName(ui, "GoldCount");
    wood = (Text*)Helper::seekWidgetByName(ui, "WoodCount");
    gold->setString(GM()->getIntToStr(DM()->getGoldCount()));
    wood->setString(GM()->getIntToStr(DM()->getWoodCount()));
    
    // ���ý�����������
    auto mineFactory = (Text*)Helper::seekWidgetByName(ui, "MineFactoryCount");
    auto woodFactory = (Text*)Helper::seekWidgetByName(ui, "WoodFactoryCount");
    auto arrowTower = (Text*)Helper::seekWidgetByName(ui, "ArrowTowerCount");
    auto cannon = (Text*)Helper::seekWidgetByName(ui, "CannonCount");
    auto laser = (Text*)Helper::seekWidgetByName(ui, "LaserCount");
    
    mineCount = DM()->getBuildingCountForType(BUILDING_TYPE_MineFactory);
    woodCount = DM()->getBuildingCountForType(BUILDING_TYPE_WoodFactory);
    arrowCount = DM()->getBuildingCountForType(BUILDING_TYPE_ArrowTower);
    cannonCount = DM()->getBuildingCountForType(BUILDING_TYPE_Cannon);
    laserCount = DM()->getBuildingCountForType(BUILDING_TYPE_Laser);
    mineLimit = limitMap["MineFactoryCapacity"].asInt();
    woodLimit = limitMap["WoodFactoryCapacity"].asInt();
    arrowLimit = limitMap["ArrowTowerCapacity"].asInt();
    cannonLimit = limitMap["CannonCapacity"].asInt();
    laserLimit = limitMap["LaserCapacity"].asInt();
    
    mineFactory->setString(GM()->getIntToStr(mineCount) + "/" + GM()->getIntToStr(mineLimit));
    woodFactory->setString(GM()->getIntToStr(woodCount) + "/" + GM()->getIntToStr(woodLimit));
    arrowTower->setString(GM()->getIntToStr(arrowCount) + "/" + GM()->getIntToStr(arrowLimit));
    cannon->setString(GM()->getIntToStr(cannonCount) + "/" + GM()->getIntToStr(cannonLimit));
    laser->setString(GM()->getIntToStr(laserCount) + "/" + GM()->getIntToStr(laserLimit));
    
    // ���ɽ������Ϊ��ɫ
    if (mineCount >= mineLimit) {
        auto btnMine = Helper::seekWidgetByName(ui, "MineFactoryButton");
        auto imgMine = Helper::seekWidgetByName(ui, "Building_MineFactory");
        btnMine->setColor(Color3B::GRAY);
        imgMine->setColor(Color3B::GRAY);
    }
    if (woodCount >= woodLimit) {
        auto btnWood = Helper::seekWidgetByName(ui, "WoodFactoryButton");
        auto imgWood = Helper::seekWidgetByName(ui, "Building_WoodFactory");
        btnWood->setColor(Color3B::GRAY);
        imgWood->setColor(Color3B::GRAY);
    }
    if (arrowCount >= arrowLimit) {
        auto btnArrow = Helper::seekWidgetByName(ui, "ArrowTowerButton");
        auto imgArrow = Helper::seekWidgetByName(ui, "Building_ArrowTower");
        btnArrow->setColor(Color3B::GRAY);
        imgArrow->setColor(Color3B::GRAY);
    }
    if (cannonCount >= cannonLimit) {
        auto btnCannon = Helper::seekWidgetByName(ui, "CannonButton");
        auto imgCannon = Helper::seekWidgetByName(ui, "Building_Cannon");
        btnCannon->setColor(Color3B::GRAY);
        imgCannon->setColor(Color3B::GRAY);
    }
    if (laserCount >= laserLimit) {
        auto btnLaser = Helper::seekWidgetByName(ui, "LaserButton");
        auto imgLaser = Helper::seekWidgetByName(ui, "Building_Laser");
        btnLaser->setColor(Color3B::GRAY);
        imgLaser->setColor(Color3B::GRAY);
    }
    
}


void NewBuildDialog::hideDialog()
{
    // Action
    auto scale = ScaleTo::create(0.3f, 0.0f);
    auto ease = EaseBackIn::create(scale);
    auto func = CallFunc::create(CC_CALLBACK_0(NewBuildDialog::removeDialog, this));
    auto act = Sequence::create(ease, func, nullptr);
    this->runAction(act);
}


void NewBuildDialog::removeDialog()
{
    this->removeFromParent();
}


void NewBuildDialog::showNotice(string text)
{
//    CCLOG("showNotice");
    
    auto WIN_SIZE = Director::getInstance()->getWinSize();
    auto notice = Notice::create(text);
    this->getParent()->addChild(notice, 999);
}


// �����µ���ʩ
void NewBuildDialog::buildCallback(Ref* sender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED) {
        auto homeScene = (HomeScene*)this->getParent();
        auto mapLayer = (HomeMapLayer*)homeScene->_mapLayer;
        
        auto btn = (Button*)sender;
        int mygold = GM()->getStrToInt(gold->getString());
        
        string name = btn->getName();

        // ����ɿ�
        if (name == "MineFactoryButton") {
            // ������ʩ�������Ƿ�����
            if (mineCount >= mineLimit) {
                showNotice("����˾��ȼ����ɽ������Ĳɿ�");
                return;
            }
            // �������Ƿ��㹻
            if (mygold < 200) {
                showNotice("�����Դ����");
                return;
            }
            
            mapLayer->detachAllBuilding();
            
            GM()->_newBuild = true;
            if (GM()->_build != nullptr) GM()->_build->removeFromParent();
            GM()->_build = NewBuilding::create(BUILDING_TYPE_MineFactory, 200);
            mapLayer->addChild(GM()->_build);
        }
        
        // ����ľ�ĳ�
        else if(name == "WoodFactoryButton") {
            
            // ������ʩ�������Ƿ�����
            if (woodCount >= woodLimit) {
                showNotice("����˾��ȼ����ɽ�������ľ�ĳ�");
                return;
            }
            // �������Ƿ��㹻
            if (mygold < 200) {
                showNotice("�����Դ����");
                return;
            }
            
            mapLayer->detachAllBuilding();
            
            GM()->_newBuild = true;
            if (GM()->_build != nullptr) GM()->_build->removeFromParent();
            GM()->_build = NewBuilding::create(BUILDING_TYPE_WoodFactory, 200);
            mapLayer->addChild(GM()->_build);
        }
        
        // �������
        else if(name == "ArrowTowerButton") {
            
            // ������ʩ�������Ƿ�����
            if (arrowCount >= arrowLimit) {
                showNotice("����˾��ȼ����ɽ������ļ���");
                return;
            }
            // �������Ƿ��㹻
            if (mygold < 300) {
                showNotice("�����Դ����");
                return;
            }
            
            mapLayer->detachAllBuilding();

            GM()->_newBuild = true;
            if (GM()->_build != nullptr) GM()->_build->removeFromParent();
            GM()->_build = NewBuilding::create(BUILDING_TYPE_ArrowTower, 300);
            mapLayer->addChild(GM()->_build);
        }
        
        // ��������
        else if(name == "CannonButton") {
            
            // ������ʩ�������Ƿ�����
            if (cannonCount >= cannonLimit) {
                showNotice("����˾��ȼ����ɽ�����������");
                return;
            }
            // �������Ƿ��㹻
            if (mygold < 500) {
                showNotice("�����Դ����");
                return;
            }
            
            mapLayer->detachAllBuilding();
            
            GM()->_newBuild = true;
            if (GM()->_build != nullptr) GM()->_build->removeFromParent();
            GM()->_build = NewBuilding::create(BUILDING_TYPE_Cannon, 500);
            mapLayer->addChild(GM()->_build);
        }
        
        // ���켤����
        else if(name == "LaserButton") {
            
            // ������ʩ�������Ƿ�����
            if (laserCount >= laserLimit) {
                showNotice("����˾��ȼ����ɽ������ļ�����");
                return;
            }
            // �������Ƿ��㹻
            if (mygold < 800) {
                showNotice("�����Դ����");
                return;
            }
            
            mapLayer->detachAllBuilding();
            
            GM()->_newBuild = true;
            if (GM()->_build != nullptr) GM()->_build->removeFromParent();
            GM()->_build = NewBuilding::create(BUILDING_TYPE_Laser, 800);
            mapLayer->addChild(GM()->_build);
        }
        
        btn->setTouchEnabled(false);
        hideDialog();
    }
}


void NewBuildDialog::closeCallback(Ref *sender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED) {
        auto btn = (Button*)sender;
        btn->setTouchEnabled(false);
        hideDialog();
    }
}

