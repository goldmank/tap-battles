#include "SplashScene.h"
#include "SimpleAudioEngine.h"
#include "Pref.h"
#include "SystemUtils.h"
#include "GameScene.h"
#include "MuGames.h"

using namespace cocos2d;
using namespace CocosDenshion;

// ===========================================================
// Initialization & cleanup
// ===========================================================

SplashScene::SplashScene()
{
}

SplashScene::~SplashScene()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("musplash.plist");
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

// ===========================================================
// CCLayer methods
// ===========================================================

CCScene* SplashScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    SplashScene *layer = SplashScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// ===========================================================
// BaseScene methods
// ===========================================================

void SplashScene::handleInit(float screenWidth, float screenHeight)
{
    MuGames::getInstance()->login();
    
    Sprite * pLogo = Sprite::create("flambo_logo.png");
    pLogo->setPosition(screenWidth * 0.5f, screenHeight * 0.5f);
    addChild(pLogo);
    
    const char * batches[] = { "musplash", NULL };
    loadBatch(batches, "png");
    
    m_pMuLayer = LayerColor::create(Color4B(19, 19, 19, 255), screenWidth, screenHeight);
    addChild(m_pMuLayer);
    
    m_pMuBg = Sprite::createWithSpriteFrameName("mu_splash_bg.png");
    m_pMuBg->setPosition(screenWidth / 2, screenHeight / 2);
    m_pMuLayer->addChild(m_pMuBg);
    
    Sprite * pMuLogo = Sprite::createWithSpriteFrameName("01_00015.png");
    pMuLogo->setPosition(screenWidth / 2, screenHeight / 2);
    m_pMuLayer->addChild(pMuLogo);
    
    float startDelay = 0.1f;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    startDelay = 2;
#endif
           
    char spriteName[50];
    Vector<SpriteFrame*> array(100);
    for (int i = 15; i <= 99; i++)
    {
        sprintf(spriteName, "01_000%02d.png", i);
        array.pushBack(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(spriteName));
    }
    CCAnimation * pAnimFrames = CCAnimation::createWithSpriteFrames(array, 30.0f / 1000.0f);
    CCAnimate * pAction = CCAnimate::create(pAnimFrames);
    CCFiniteTimeAction* pDone = CCCallFuncN::create(this, callfuncN_selector(SplashScene::onMuLogoDone));
    pMuLogo->runAction(Sequence::create(DelayTime::create(startDelay), pAction, pDone, NULL));
    
    m_pMuBg->runAction(Sequence::createWithTwoActions(DelayTime::create(startDelay), FadeTo::create(0.2f, 0)));
}

void SplashScene::handleUpdate(float dt)
{
}

bool SplashScene::handleOnTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* event)
{
    return true;
}

void SplashScene::handleOnTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* event)
{
}

void SplashScene::handleOnTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* event)
{
}

void SplashScene::handleOnTouchCancelled(cocos2d::Touch* pTouch, cocos2d::Event* event)
{
}

// ===========================================================
// Private methods
// ===========================================================

void SplashScene::onMuLogoDone(cocos2d::Node * pSender)
{
    for (int i = 0; i < m_pMuLayer->getChildrenCount(); i++)
    {
        m_pMuLayer->getChildren().at(i)->runAction(FadeTo::create(0.4f, 0));
    }
    m_pMuLayer->runAction(FadeTo::create(0.4f, 0));
        
    CCFiniteTimeAction* pDone = CCCallFuncN::create(this, callfuncN_selector(SplashScene::onFlamboLogoDone));
    runAction(Sequence::createWithTwoActions(DelayTime::create(2), pDone));
}

void SplashScene::onFlamboLogoDone(cocos2d::Node * pSender)
{
    switchScene(GameScene::scene());
}