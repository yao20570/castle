#include "BuildingUpgradeDialog.h"


BuildingUpgradeDialog* BuildingUpgradeDialog::create(BuildingSprite* buildingSprite)
{
    BuildingUpgradeDialog *pRet = new(std::nothrow) BuildingUpgradeDialog();
    if (pRet && pRet->init(buildingSprite)) {
        pRet->autorelease();
        return pRet;
    }
    else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}


bool BuildingUpgradeDialog::init(BuildingSprite* buildingSprite)
{
    if ( !Layer::init() ) {
        return false;
    }
    
    _buildingSprite = buildingSprite;
    
    this->setScale(0.0f);
    showDialog();
    
    return true;
}


void BuildingUpgradeDialog::showDialog()
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


    auto ui = GUIReader::getInstance()->widgetFromJsonFile(UI_DIALOG_BUILDING_UPGRADE);
    this->addChild(ui, 1, "UI");
    
    // �رհ�ť
    auto btnClose = Helper::seekWidgetByName(ui, "CloseButton");
    btnClose->addTouchEventListener(CC_CALLBACK_2(BuildingUpgradeDialog::btnCallback, this));
    
    // ������ť
    auto btnUpgrade = Helper::seekWidgetByName(ui, "UpgradeButton");
    btnUpgrade->addTouchEventListener(CC_CALLBACK_2(BuildingUpgradeDialog::upgradeCallback, this));
    
    showNowInfo();
    showNextInfo();
    showRequire();
}


// ��ʾ��ǰ�ȼ���Ϣ
void BuildingUpgradeDialog::showNowInfo()
{
    auto ui = (Widget*)this->getChildByName("UI");
    
    string t = _buildingSprite->_name + " Lv." + GM()->getIntToStr(_buildingSprite->_level) + " -> " + GM()->getIntToStr(_buildingSprite->_level+1);
    auto title = (Text*)Helper::seekWidgetByName(ui, "Title");
    auto healthPoint = (Text*)Helper::seekWidgetByName(ui, "HealthPoint");
    auto image = (ImageView*)Helper::seekWidgetByName(ui, "BuildingImage");
    
    title->setString(t);
    healthPoint->setString(GM()->getIntToStr(_buildingSprite->_healthPoint));
    image->loadTexture(GM()->getBuildingIMG(_buildingSprite->_type));
    
    switch (_buildingSprite->_type) {
        case BUILDING_TYPE_ArrowTower:
        case BUILDING_TYPE_Cannon:
        case BUILDING_TYPE_Laser: {
            auto lbDamage = (Text*)Helper::seekWidgetByName(ui, "lbDamage");
            auto lbAttackSpeed = (Text*)Helper::seekWidgetByName(ui, "lbAttackSpeed");
            auto lbShootRange = (Text*)Helper::seekWidgetByName(ui, "lbShootRange");
            auto damage = (Text*)Helper::seekWidgetByName(ui, "Damage");
            auto attackSpeed = (Text*)Helper::seekWidgetByName(ui, "AttackSpeed");
            auto shootRange = (Text*)Helper::seekWidgetByName(ui, "ShootRange");
            
            lbDamage->setVisible(true);
            lbAttackSpeed->setVisible(true);
            lbShootRange->setVisible(true);
            damage->setString(GM()->getIntToStr(_buildingSprite->_damage));
            attackSpeed->setString(GM()->getIntToStr(_buildingSprite->_attackSpeed));
            shootRange->setString(GM()->getIntToStr(_buildingSprite->_shootRange));
        }
            break;
        case BUILDING_TYPE_MineFactory: {
            auto lbGoldProduct = (Text*)Helper::seekWidgetByName(ui, "lbGoldProduct");
            auto goldProduct = (Text*)Helper::seekWidgetByName(ui, "GoldProduct");
            
            lbGoldProduct->setVisible(true);
            goldProduct->setString(GM()->getIntToStr(_buildingSprite->_goldProduct));
        }
            break;
        case BUILDING_TYPE_WoodFactory: {
            auto lbWoodProduct = (Text*)Helper::seekWidgetByName(ui, "lbWoodProduct");
            auto WoodProduct = (Text*)Helper::seekWidgetByName(ui, "WoodProduct");
            
            lbWoodProduct->setVisible(true);
            WoodProduct->setString(GM()->getIntToStr(_buildingSprite->_woodProduct));
        }
            break;
        case BUILDING_TYPE_BaseTower: {
            auto lbGoldCapacity = (Text*)Helper::seekWidgetByName(ui, "lbGoldCapacity");
            auto lbWoodCapacity = (Text*)Helper::seekWidgetByName(ui, "lbWoodCapacity");
            auto goldCapacity = (Text*)Helper::seekWidgetByName(ui, "GoldCapacity");
            auto woodCapacity = (Text*)Helper::seekWidgetByName(ui, "WoodCapacity");
            
            lbGoldCapacity->setVisible(true);
            lbWoodCapacity->setVisible(true);
            goldCapacity->setString(GM()->getIntToStr(_buildingSprite->_goldCapacity));
            woodCapacity->setString(GM()->getIntToStr(_buildingSprite->_woodCapacity));
            
            // BuildingLimit
            auto scrollView = (ScrollView*)Helper::seekWidgetByName(ui, "ScrollView");
            scrollView->setVisible(true);
        }
            break;
        default:
            break;
    }
}


// ��ʾ��һ�ȼ�������Ϣ
void BuildingUpgradeDialog::showNextInfo()
{
    ValueMap& map = DM()->getBuildingNextInfo(_buildingSprite->_BuildingID);

    auto ui = (Widget*)this->getChildByName("UI");
    
    auto healthPoint = (Text*)Helper::seekWidgetByName(ui, "AddHealthPoint");
    healthPoint->setString("+" + GM()->getIntToStr(map["HealthPoint"].asInt() - _buildingSprite->_healthPoint));

    switch (_buildingSprite->_type) {
        case BUILDING_TYPE_ArrowTower:
        case BUILDING_TYPE_Cannon:
        case BUILDING_TYPE_Laser: {
            auto damage = (Text*)Helper::seekWidgetByName(ui, "AddDamage");
            auto attackSpeed = (Text*)Helper::seekWidgetByName(ui, "AddAttackSpeed");
            auto shootRange = (Text*)Helper::seekWidgetByName(ui, "AddShootRange");
            
            damage->setString("+" + GM()->getIntToStr(map["Damage"].asInt() - _buildingSprite->_damage));
            attackSpeed->setString("+" + GM()->getIntToStr(map["AttackSpeed"].asInt() - _buildingSprite->_attackSpeed));
            shootRange->setString("+" + GM()->getIntToStr(map["ShootRange"].asInt() - _buildingSprite->_shootRange));
        }
            break;
        case BUILDING_TYPE_MineFactory: {
            auto goldProduct = (Text*)Helper::seekWidgetByName(ui, "AddGoldProduct");
            
            goldProduct->setString("+" + GM()->getIntToStr(map["GoldProduct"].asInt() - _buildingSprite->_goldProduct));
        }
            break;
        case BUILDING_TYPE_WoodFactory: {
            auto WoodProduct = (Text*)Helper::seekWidgetByName(ui, "AddWoodProduct");
            
            WoodProduct->setString("+" + GM()->getIntToStr(map["WoodProduct"].asInt() - _buildingSprite->_woodProduct));
        }
            break;
        case BUILDING_TYPE_BaseTower: {
            auto goldCapacity = (Text*)Helper::seekWidgetByName(ui, "AddGoldCapacity");
            auto woodCapacity = (Text*)Helper::seekWidgetByName(ui, "AddWoodCapacity");
            
            goldCapacity->setString("+" + GM()->getIntToStr(map["GoldCapacity"].asInt() - _buildingSprite->_goldCapacity));
            woodCapacity->setString("+" + GM()->getIntToStr(map["WoodCapacity"].asInt() - _buildingSprite->_woodCapacity));
            
            // BuildingLimit
            auto mineFactory = (Text*)Helper::seekWidgetByName(ui, "MineFactoryLimit");
            auto woodFactory = (Text*)Helper::seekWidgetByName(ui, "WoodFactoryLimit");
            auto arrowTower = (Text*)Helper::seekWidgetByName(ui, "ArrowTowerLimit");
            auto cannon = (Text*)Helper::seekWidgetByName(ui, "CannonLimit");
            auto laser = (Text*)Helper::seekWidgetByName(ui, "LaserLimit");
            
            auto baseLevel = DM()->getBaseTowerLevel();
            auto nowLimit = DM()->getBuildingLimit(baseLevel);
            auto nextLimit = DM()->getBuildingLimit(baseLevel + 1);
            auto mineDelta = nextLimit["MineFactoryCapacity"].asInt() - nowLimit["MineFactoryCapacity"].asInt();
            auto woodDelta = nextLimit["WoodFactoryCapacity"].asInt() - nowLimit["WoodFactoryCapacity"].asInt();
            auto arrowDelta = nextLimit["ArrowTowerCapacity"].asInt() - nowLimit["ArrowTowerCapacity"].asInt();
            auto cannonDelta = nextLimit["CannonCapacity"].asInt() - nowLimit["CannonCapacity"].asInt();
            auto laserDelta = nextLimit["LaserCapacity"].asInt() - nowLimit["LaserCapacity"].asInt();
            
            mineFactory->setString("+" + GM()->getIntToStr(mineDelta));
            woodFactory->setString("+" + GM()->getIntToStr(woodDelta));
            arrowTower->setString("+" + GM()->getIntToStr(arrowDelta));
            cannon->setString("+" + GM()->getIntToStr(cannonDelta));
            laser->setString("+" + GM()->getIntToStr(laserDelta));
        }
            break;
        default:
            break;
    }
}


// ��ʾ��Դ������Ϣ
void BuildingUpgradeDialog::showRequire()
{
    auto ui = (Widget*)this->getChildByName("UI");
 
    auto goldRequire = (Text*)Helper::seekWidgetByName(ui, "GoldRequire");
    auto woodRequire = (Text*)Helper::seekWidgetByName(ui, "WoodRequire");
    auto timeRequire = (Text*)Helper::seekWidgetByName(ui, "TimeRequire");
    
    _goldRequire = _buildingSprite->_goldRequire;
    _woodRequire = _buildingSprite->_woodRequire;
    _timeRequire = _buildingSprite->_timeRequire;
    
    goldRequire->setString(GM()->getIntToStr(_goldRequire));
    woodRequire->setString(GM()->getIntToStr(_woodRequire));
    timeRequire->setString(GM()->getIntToStr(_timeRequire) + "s");
}


void BuildingUpgradeDialog::hideDialog()
{
    // Action
    auto scale = ScaleTo::create(0.3f, 0.0f);
    auto ease = EaseBackIn::create(scale);
    auto func = CallFunc::create(CC_CALLBACK_0(BuildingUpgradeDialog::removeDialog, this));
    auto act = Sequence::create(ease, func, nullptr);
    this->runAction(act);
}


void BuildingUpgradeDialog::removeDialog()
{
    this->removeFromParent();
}


// �رհ�ť
void BuildingUpgradeDialog::btnCallback(Ref *sender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED) {
        auto btn = (Button*)sender;
        btn->setTouchEnabled(false);
        hideDialog();
    }
}

// ������ť
void BuildingUpgradeDialog::upgradeCallback(Ref* sender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED) {
   
        // ��ҵȼ�or˾��ȼ�����
        int flag = -1;
        if(_buildingSprite->_type == BUILDING_TYPE_BaseTower) {
            int playerLevel = DM()->getPlayerLevel();
            if (_buildingSprite->_playerLevelRequire > playerLevel) {
                flag = 1;
            }
        }
        else {
            int baseLevel = DM()->getBaseTowerLevel();
            if(_buildingSprite->_baseLevelRequire > baseLevel) {
                flag = 2;
            }
        }
        
        // ��ҵȼ�����
        if (flag == 1) {
            auto notice = Notice::create("��Ҫ��ҵȼ�" + GM()->getIntToStr(_buildingSprite->_playerLevelRequire));
            this->getParent()->addChild(notice, 999);
        }
        // ˾��ȼ�����
        else if(flag == 2) {
            auto notice = Notice::create("��Ҫ˾��ȼ�" + GM()->getIntToStr(_buildingSprite->_baseLevelRequire));
            this->getParent()->addChild(notice, 999);
        }
        // ��Դ����
        else if (_goldRequire > DM()->getGoldCount() || _woodRequire > DM()->getWoodCount()) {
            auto notice = Notice::create("��Դ����");
            this->getParent()->addChild(notice, 999);
        }
        // ����
        else {
            auto btn = (Button*)sender;
            btn->setTouchEnabled(false);
            
            // ���½�ҡ�ľ����Դ
            auto homeScene = (HomeScene*)this->getParent();
            auto hudLayer = (HomeHudLayer*)homeScene->_hudLayer;
            hudLayer->addGold(-_goldRequire);
            hudLayer->addWood(-_woodRequire);
            
            // ������������
            _buildingSprite->upgrade();
            
            // ����HomeOptLayer
            auto optLayer = homeScene->_optLayer;
            optLayer->show(_buildingSprite);
            
            hideDialog();
        }
    }
}

