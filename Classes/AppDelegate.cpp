#include "AppDelegate.h"
#include "HelloWorldScene.h"

// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

#include "Utils/Public.h"

USING_NS_CC;

static float scale = 0.6; //窗口缩放比列
static cocos2d::Size designResolutionSize = cocos2d::Size(640, 1136);
static cocos2d::Size smallResolutionSize = cocos2d::Size(320, 480 );
static cocos2d::Size mediumResolutionSize = cocos2d::Size(768, 1024);
static cocos2d::Size largeResolutionSize = cocos2d::Size(1536, 2048);

//static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
//static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
//static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
//static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);




AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("chengshi", cocos2d::Rect(0, 0, scale * designResolutionSize.width, scale * designResolutionSize.height));
#else
        glview = GLViewImpl::create("chengshi");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    //// Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }
	

    register_all_packages();

    // create a scene. it's an autorelease object
	loadResources();
    auto scene = HelloWorld::createScene();

	//auto scene = HelloScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}

#include "Utils\JsonManager.h"


void test(){
	//long long x = GM()->getMTimeStamp();
	//int i = 1;

	//timeval tv;
 //   gettimeofday(&tv, NULL);
 //   INT64 xx = (INT64)tv.tv_sec * 1000 + (INT64)tv.tv_usec / 1000; 

	
	//JsonMgr()->getSkillById(0);

}


void AppDelegate::loadResources()
{

	test();

	//// 创建数据库
	DBM()->createTable();



	// 加载搜索路径
	FileUtils::getInstance()->addSearchPath("images");

	//// 加载动画
	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/skill/NewAnimation.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/skill/NewAnimation_simayupugong.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/skill/NewAnimation_zhouyupugong.ExportJson");

	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/skill/NewAnimation_jineng.ExportJson");

	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/soilder/Anim_Bowman.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/hero/Anim_Fighter.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/soilder/Anim_Qibing.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/hero/Anim_MeatShield.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/hero/Anim_Hero_Fashi.ExportJson");


	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/hero/Anim_Hero_Caocao.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/hero/Anim_Hero_Daqiao.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/hero/Anim_Hero_Ganning.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/hero/Anim_Hero_Guanyu.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/hero/Anim_Hero_Liubei.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/hero/Anim_Hero_Luxun.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/hero/Anim_Hero_LvBu.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/hero/Anim_Hero_Machao.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/hero/Anim_Hero_Simayi.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/hero/Anim_Hero_Sunshangxiang.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/hero/Anim_Hero_Xiahoudun.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/hero/Anim_Hero_Zhangliao.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/hero/Anim_Hero_Zhenji.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/hero/Anim_Hero_Zhouyu.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/hero/Anim_Hero_Zhugeliang.ExportJson");

	ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/hero/NewAnimation_Hero_Dongzuo.ExportJson");
}