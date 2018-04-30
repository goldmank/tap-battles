#include "AppDelegate.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "SystemUtils.h"
#include "Pref.h"
#include "GameScene.h"
#include "SplashScene.h"

using namespace cocos2d;
using namespace CocosDenshion;

typedef struct tagResource
{
    cocos2d::CCSize size;
    char directory[100];
} Resource;

static Resource smallResource  =  { cocos2d::Size(640, 960),   "iphone" };
static Resource mediumResource =  { cocos2d::Size(768, 1024),  "ipad"   };
static Resource largeResource  =  { cocos2d::Size(1536, 2048), "ipadhd" };
static cocos2d::Size designResolutionSize = mediumResource.size;

AppDelegate::AppDelegate(SystemUtils * pSystemUtils)
{
    SystemUtils::initInstance(pSystemUtils);
}

AppDelegate::~AppDelegate()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    
    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages,
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching()
{    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
		glview = GLViewImpl::create("My Game");
		director->setOpenGLView(glview);
	}
    
    // turn on display FPS
    //director->setDisplayStats(true);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);
    
    Size frameSize = glview->getFrameSize();
    CCLOG("frameSize: %.2f, %.2f", frameSize.width, frameSize.height);
    
    std::vector<std::string> resDirOrders;
//    if (frameSize.height > mediumResource.size.height || frameSize.width >= 1136)
//    {
//        resDirOrders.push_back(largeResource.directory);
//        director->setContentScaleFactor(largeResource.size.width / designResolutionSize.width);
//    }
//    else
//    {
        resDirOrders.push_back(mediumResource.directory);
        director->setContentScaleFactor(mediumResource.size.width / designResolutionSize.width);
    //}
    auto fileUtils = FileUtils::getInstance();
    fileUtils->setSearchPaths(resDirOrders);
    
    // create a scene. it's an autorelease object
    auto scene = GameScene::scene();
    
    // run
    director->runWithScene(scene);
    
    SystemUtils::getInstance()->multiplayerLogin();
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    CCDirector::sharedDirector()->pause();
    
    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->stopAnimation();
    CCDirector::sharedDirector()->resume();
    CCDirector::sharedDirector()->startAnimation();
    
    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    
    SystemUtils::getInstance()->multiplayerLogin();
}
