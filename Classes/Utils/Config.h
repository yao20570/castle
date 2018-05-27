#ifndef __ConfigInfo_H__
#define __ConfigInfo_H__

typedef long long LL;
//typedef Size Size;
//typedef Rect Rect;


//--------------------------------------------------------------------------------
// Config
//--------------------------------------------------------------------------------


// �����ֵ
static const float  EPS                 = 1e-6;
static const float  INF                 = 0x7fffffff;
static const float  LIMIT_DELTA         = 10.0f;
static const float  MAP_X               = 1005;
static const float  MAP_Y               = 1373;
static const float  SX                  = 1030 ;
static const float  SY                  = 1400 ;
static const float  TILED_WIDTH         = 45;
static const float  TILED_HEIGHT        = 32.5;
static const int    TILED_TOTAL_X       = 38;
static const int    TILED_TOTAL_Y       = 38;
static const Size TILED_SIZE          = Size(45, 32.5);
static const int DX[] = {1, 1, 1, 0, -1, -1, -1, 0, 0};
static const int DY[] = {1, 0, -1, -1, -1, 0, 1, 1, 0};


// ��ͼ�ǳ�״̬
static const int TOWN_TYPE_HOME         = 1;    // ��ҳǳ�
static const int TOWN_TYPE_CHAPTER      = 2;    // �ؿ��ǳ�
static const int TOWN_TYPE_LIBERATE     = 3;    // ��ųǳ�
// ����״̬
static const int MIWU_TYPE_LOCK         = 1;    // δ����
static const int MIWU_TYPE_UNLOCK       = 2;    // �ѽ���
// ����ؿ�״̬
static const int CHAPTER_TYPE_SEEK      = 1;    // ���
static const int CHAPTER_TYPE_FIGHT     = 1;    // ����
// �ջ�״̬
static const int HARVEST_TYPE_GOLD      = 1;    // �ջ���
static const int HARVEST_TYPE_WOOD      = 2;    // �ջ�ľ��
static const int HARVEST_TYPE_BOTH      = 3;    // �ջ��ҡ�ľ��
// ��������
static const int BUILDING_TYPE_BaseTower    = 1;    // ˾�
static const int BUILDING_TYPE_Raider       = 2;    // �״�
static const int BUILDING_TYPE_HeroHotel    = 3;    // Ӣ���ù�
static const int BUILDING_TYPE_Camp         = 4;    // ��Ӫ
static const int BUILDING_TYPE_ResearchLab  = 5;    // �о���
static const int BUILDING_TYPE_MineFactory  = 6;    // �ɿ�
static const int BUILDING_TYPE_WoodFactory  = 7;    // ľ�ĳ�
static const int BUILDING_TYPE_ArrowTower   = 8;    // ����
static const int BUILDING_TYPE_Cannon       = 9;    // ����
static const int BUILDING_TYPE_Laser        = 10;   // ������
// ����״̬
static const int BUILDING_STATE_FINISHED    = 1;    // �ѽ���
static const int BUILDING_STATE_BUILDING    = 2;    // ���ڽ���

// ʿ������
static const int SOILDER_TYPE_FIGHTER       = 1;    // ����ʿ��
static const int SOILDER_TYPE_BOWMAN        = 2;    // ���鹭����
static const int SOILDER_TYPE_GUNNER        = 3;    // ��������
static const int SOILDER_TYPE_MEAT          = 4;    // �������

// Ӣ������
static const int HERO_TYPE_ARAGORN          = 1;    // Aragorn
// Ӣ��״̬
static const int HERO_STATE_REST            = 1;    // ��Ϣ��
static const int HERO_STATE_BATTLE          = 2;    // ��ս��

// ��Ϸ����
static const int GAME_OVER_SUCCESS          = 1;    // ʤ��
static const int GAME_OVER_FAILED           = 2;    // ʧ��

// ͼƬ
// Background
static const char* IMG_LOADING_BG       = "images/background/LoadingBG.png";
static const char* IMG_WORLD_BG         = "images/background/WorldBG.png";
static const char* IMG_HOME_BG          = "images/background/HomeBG.png";
static const char* IMG_CHAPTER_BG       = "images/background/ChapterBG.png";
static const char* IMG_GRAY_BG          = "images/background/GrayBG.png";

// WorldMap
static const char* IMG_TOWN_HOME        = "images/TownHome.png";
static const char* IMG_TOWN_CHAPTER     = "images/TownChapter.png";
static const char* IMG_TOWN_LIBERATE    = "images/TownLiberate.png";
static const char* IMG_MIWU             = "images/Miwu.png";
// Property
static const char* IMG_BUBBLE           = "images/Bubble.png";
static const char* IMG_GOLD             = "images/Gold.png";
static const char* IMG_WOOD             = "images/Wood.png";
static const char* IMG_RING             = "images/Ring.png";
// Button
static const char* IMG_BUTTON_INFOOPT       = "images/button/InfoOpt.png";
static const char* IMG_BUTTON_UPGRADEOPT    = "images/button/UpgradeOpt.png";
static const char* IMG_BUTTON_CANTUPGRADEOPT= "images/button/CantUpgradeOpt.png";
static const char* IMG_BUTTON_ENTEROPT      = "images/button/EnterOpt.png";
static const char* IMG_BUTTON_CANCLE        = "images/button/building_cancle.png";
static const char* IMG_BUTTON_OK            = "images/button/building_ok.png";
// Process
static const char* IMG_BUILD_PRO        = "images/pro_building.png";
static const char* IMG_BUILD_PRO_BK     = "images/pro_building_bk.png";
static const char* IMG_BUILD_PRO_ENEMY  = "images/pro_building_enemy.png";

// ����
static const char* IMG_SKILL_BSKILL     = "images/skill/Skill_30001.png";
static const char* IMG_SKILL_ZSKILL     = "images/skill/Skill_30002.png";
static const char* IMG_SKILL_ZZSKILL    = "images/skill/Skill_30003.png";

// �ӵ�
static const char* IMG_BULLET_ARROW     = "images/bullet/Arrow.png";
static const char* IMG_BULLET_SHELL     = "images/bullet/Shell.png";
static const char* IMG_BULLET_SHELL_RED = "images/bullet/Shell_Red.png";
static const char* IMG_BULLET_FIRE      = "images/bullet/Fire.png";
static const char* IMG_BULLET_LASER     = "images/bullet/Laser.png";
// circle
static const char* IMG_CIRCLE           = "images/Circle.png";


// �������������Ŵ󡢴ݻ�
static const char* IMG_BUILDING_Floor       = "images/building/Floor.png";
static const char* IMG_BUILDING_ArrowTip    = "images/building/ArrowTip.png";
static const char* IMG_BUILDING_BaseTower   = "images/building/BaseTower.png";
static const char* IMG_BUILDING_Raider      = "images/building/Raider.png";
static const char* IMG_BUILDING_HeroHotel   = "images/building/HeroHotel.png";
static const char* IMG_BUILDING_Camp        = "images/building/Camp.png";
static const char* IMG_BUILDING_ResearchLab = "images/building/ResearchLab.png";
static const char* IMG_BUILDING_MineFactory = "images/building/MineFactory.png";
static const char* IMG_BUILDING_WoodFactory = "images/building/WoodFactory.png";
static const char* IMG_BUILDING_ArrowTower  = "images/building/ArrowTower.png";
static const char* IMG_BUILDING_Cannon      = "images/building/Cannon.png";
static const char* IMG_BUILDING_Laser       = "images/building/Laser.png";
static const char* IMG_BUILDING_BaseTower_Big   = "images/building/BaseTower_Big.png";
static const char* IMG_BUILDING_Raider_Big      = "images/building/Raider_Big.png";
static const char* IMG_BUILDING_HeroHotel_Big   = "images/building/HeroHotel_Big.png";
static const char* IMG_BUILDING_Camp_Big        = "images/building/Camp_Big.png";
static const char* IMG_BUILDING_ResearchLab_Big = "images/building/ResearchLab_Big.png";
static const char* IMG_BUILDING_MineFactory_Big = "images/building/MineFactory_Big.png";
static const char* IMG_BUILDING_WoodFactory_Big = "images/building/WoodFactory_Big.png";
static const char* IMG_BUILDING_ArrowTower_Big  = "images/building/ArrowTower_Big.png";
static const char* IMG_BUILDING_Cannon_Big      = "images/building/Cannon_Big.png";
static const char* IMG_BUILDING_Laser_Big       = "images/building/Laser_Big.png";
static const char* IMG_BUILDING_BaseTower_Broken   = "images/building/BaseTower_broken.png";
static const char* IMG_BUILDING_Raider_Broken      = "images/building/Raider_broken.png";
static const char* IMG_BUILDING_HeroHotel_Broken   = "images/building/HeroHotel_broken.png";
static const char* IMG_BUILDING_Camp_Broken        = "images/building/Camp_broken.png";
static const char* IMG_BUILDING_ResearchLab_Broken = "images/building/ResearchLab_broken.png";
static const char* IMG_BUILDING_MineFactory_Broken = "images/building/MineFactory_broken.png";
static const char* IMG_BUILDING_WoodFactory_Broken = "images/building/WoodFactory_broken.png";
static const char* IMG_BUILDING_ArrowTower_Broken  = "images/building/ArrowTower_broken.png";
static const char* IMG_BUILDING_Cannon_Broken      = "images/building/Cannon_broken.png";
static const char* IMG_BUILDING_Laser_Broken       = "images/building/Laser_broken.png";
// Tree
static const char* IMG_TREE             = "images/building/Tree.png";


// ʿ������
static const char* ANIM_FIGHTER         = "animation/Anim_Fighter/Anim_Fighter.ExportJson";
static const char* ANIM_BOWMAN          = "animation/Anim_Bowman/Anim_Bowman.ExportJson";
static const char* ANIM_GUNNER          = "animation/Anim_Gunner/Anim_Gunner.ExportJson";
static const char* ANIM_MEATSHIELD      = "animation/Anim_MeatShield/Anim_MeatShield.ExportJson";
// ʿ����������
static const char* ANIM_NAME_FIGHTER         = "Anim_Fighter";
static const char* ANIM_NAME_BOWMAN          = "Anim_Bowman";
static const char* ANIM_NAME_GUNNER          = "Anim_Gunner";
static const char* ANIM_NAME_MEATSHIELD      = "Anim_MeatShield";
// Ӣ�۶���
static const char* ANIM_HERO_ARAGORN    = "animation/Anim_Hero_Aragorn/Anim_Hero_Aragorn.ExportJson";
static const char* ANIM_HERO_LVBU		= "animation/Anim_Hero_LvBu/Anim_Hero_LvBu.ExportJson";
// Ӣ�۶�������
static const char* ANIM_NAME_ARAGORN    = "Anim_Hero_Aragorn";
static const char* ANIM_NAME_LVBU		= "Anim_Hero_LvBu";
// ���ܶ���
static const char* ANIM_SKILL_1         = "animation/Effect0/Effect.ExportJson";
static const char* ANIM_SKILL_2         = "animation/Effort1/Effort1.ExportJson";
// ��������
static const char* ANIM_NAME_SKILL_1    = "Effect";
static const char* ANIM_NAME_SKILL_2    = "Effort1";


// Font
static const char* FONT_ARIAL           = "arial.ttf";


// Music
static const char* OGG_BACKGROUND       = "music/background.mp3";
static const char* OGG_BATTLEGROUND     = "music/Battleground.mp3";

//

// UI
static const char* UI_LAYER_WORLDHUD        = "UI/WorldHudLayer/WorldHudLayer.ExportJson";
static const char* UI_LAYER_HOMEHUD         = "UI/HomeHudLayer/HomeHudLayer.ExportJson";
static const char* UI_DIALOG_BATTLEHUD      = "UI/BattleHudLayer/BattleHudLayer.ExportJson";

static const char* UI_DIALOG_HOME           = "UI/HomeDialog/HomeDialog.ExportJson";
static const char* UI_DIALOG_LIBERATE       = "UI/LiberateDialog/LiberateDialog.ExportJson";
static const char* UI_DIALOG_CHAPTER        = "UI/ChapterDialog/ChapterDialog.ExportJson";

static const char* UI_DIALOG_BUILDING_INFO      = "UI/BuildingInfoDialog/BuildingInfoDialog.ExportJson";
static const char* UI_DIALOG_BUILDING_UPGRADE   = "UI/BuildingUpgradeDialog/BuildingUpgradeDialog.ExportJson";
static const char* UI_DIALOG_NEWBUILD       = "UI/NewBuildDialog/NewBuildDialog.ExportJson";

static const char* UI_DIALOG_SOILDER_INFO   = "UI/SoilderInfoDialog/SoilderInfoDialog.ExportJson";
static const char* UI_DIALOG_CAMP           = "UI/CampDialog/CampDialog.ExportJson";
static const char* UI_DIALOG_LAB            = "UI/LabDialog/LabDialog.ExportJson";
// hero
static const char* UI_DIALOG_HEROHOTEL      = "UI/HeroHotelDialog/HeroHotelDialog.ExportJson";
static const char* UI_DIALOG_HEROINFO       = "UI/HeroInfoDialog/HeroInfoDialog.ExportJson";
// gameOver
static const char* UI_DIALOG_GAMEOVER       = "UI/GameOverDialog/GameOverDialog.ExportJson";


// CSV�ļ�
// player
static const char* CSV_PLAYERINFO           = "CSV/PlayerInfo.csv";
static const char* CSV_PLAYERLEVELINFO      = "CSV/PlayerLevelInfo.csv";
// town / miwu
static const char* CSV_TOWNINFO             = "CSV/TownInfo.csv";
static const char* CSV_TOWNSTATEINFO        = "CSV/TownStateInfo.csv";
static const char* CSV_MIWUINFO             = "CSV/MiwuInfo.csv";
static const char* CSV_MIWUSTATEINFO        = "CSV/MiwuStateInfo.csv";
// building
static const char* CSV_BUILDINGINFO         = "CSV/BuildingInfo.csv";
static const char* CSV_BUILDINGLISTINFO     = "CSV/BuildingListInfo.csv";
static const char* CSV_BUILDINGLIMITINFO    = "CSV/BuildingLimitInfo.csv";
// soilder
static const char* CSV_SOILDER_INFO         = "CSV/SoilderInfo.csv";
static const char* CSV_SOILDER_LIMITINFO    = "CSV/SoilderLimitInfo.csv";
static const char* CSV_SOILDER_LISTINFO     = "CSV/SoilderListInfo.csv";
// hero
static const char* CSV_HERO_INFO            = "CSV/HeroInfo.csv";
static const char* CSV_HERO_LISTINFO        = "CSV/HeroListInfo.csv";
static const char* CSV_SKILL_INFO           = "CSV/HeroSkillInfo.csv";
// battle
static const char* CSV_BATTLE_INFO          = "CSV/BattleInfo.csv";


// sqlite�ļ�
static const char* DB_NAME     = "RingDB.db";


#endif // __ConfigInfo_H__

