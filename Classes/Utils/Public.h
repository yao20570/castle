#ifndef __Public_H__
#define __Public_H__

// cocos2d
#include "cocos2d.h"
using namespace cocos2d;
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocostudio;
using namespace cocos2d::ui;

// Helper
#include "../Helper/Sqlite3/sqlite3.h"
#include "../Helper/CSVParser/CSVParser.h"
using namespace CSVParser;


// class
// Manager
class ActManager;
class DataManager;
class DBManager;
class GlobalManager;

// Model
class Notice;
class BuildProcess;
class HarvestBubble;
class TownSprite;
class MiwuSprite;
class BuildingSprite;
class NewBuilding;
class BulletSprite;
class Robot;
class Building;
class Soilder;
class Hero;

// PanZoomLayer
class PanZoomLayer;
// HelloScene
class HelloScene;
// WorldScene
class WorldScene;
class WorldMapLayer;
class WorldHudLayer;
// HomeScene
class HomeScene;
class HomeMapLayer;
class HomeHudLayer;
class HomeOptLayer;
// ChapterScene
class BattleScene;
class BattleMapLayer;
class BattleHudLayer;
class AIManager;

// Dialog
class TownDialog;
class BuildingInfoDialog;
class BuildingUpgradeDialog;
class NewBuildDialog;
class SoilderInfoDialog;
class CampDialog;
class LabDialog;
class HeroInfoDialog;
class HeroHotelDialog;
class GameOverDialog;


// Manager
#include "../Utils/Config.h"
#include "../Utils/DataManager.h"
#include "../Utils/DBManager.h"
#include "../Utils/GlobalManager.h"

// Model
#include "../Model/Notice.h"
#include "../Model/BuildProcess.h"
#include "../Model/HarvestBubble.h"
#include "../Model/TownSprite.h"
#include "../Model/MiwuSprite.h"
#include "../Model/BuildingSprite.h"
#include "../Model/NewBuilding.h"
#include "../Model/BulletSprite.h"
#include "../Model/Robot.h"
#include "../Model/Building.h"
#include "../Model/Soilder.h"
#include "../Model/Hero.h"

// PanZoomLayer
#include "../UI/PanZoomLayer.h"
// HelloScene
#include "../UI/HelloScene/HelloScene.h"
// WorldScene
#include "../UI/WorldScene/WorldScene.h"
#include "../UI/WorldScene/WorldMapLayer.h"
#include "../UI/WorldScene/WorldHudLayer.h"
// HomeScene
#include "../UI/HomeScene/HomeScene.h"
#include "../UI/HomeScene/HomeMapLayer.h"
#include "../UI/HomeScene/HomeHudLayer.h"
#include "../UI/HomeScene/HomeOptLayer.h"
// BattleScene
#include "../UI/BattleScene/BattleScene.h"
#include "../UI/BattleScene/BattleMapLayer.h"
#include "../UI/BattleScene/BattleHudLayer.h"
#include "../UI/BattleScene/AIManager.h"


// Dialog
#include "../UI/DialogScene/TownDialog.h"
#include "../UI/DialogScene/BuildingInfoDialog.h"
#include "../UI/DialogScene/BuildingUpgradeDialog.h"
#include "../UI/DialogScene/NewBuildDialog.h"
#include "../UI/DialogScene/SoilderInfoDialog.h"
#include "../UI/DialogScene/CampDialog.h"
#include "../UI/DialogScene/LabDialog.h"
#include "../UI/DialogScene/HeroInfoDialog.h"
#include "../UI/DialogScene/HeroHotelDialog.h"
#include "../UI/DialogScene/GameOverDialog.h"


#endif // __Public_H__
