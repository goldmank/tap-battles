#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Pref.h"
#include "SystemUtils.h"
#include "GameRecorder.h"
#include "GameRecording.h"
#include "../cocos/ui/UIImageView.h"
#include "MuGames.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace network;
using namespace ui;

#define ACTION_WARRIOR  0
#define ACTION_DEFENDER 1
#define ACTION_SLOWER   2
#define ACTION_MAGICAN  3
#define ACTION_HEALER   4

#define SPEED_FACTOR    50

#define FRONT_Z 10000

// ===========================================================
// Constants
// ===========================================================

static const char * fontList[] =
{
#if ((CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC))
    "Junegull",
    "bubbleboddy Fat",
    "bubbleboddy light Light"
#else
    "fonts/junegull.ttf",
    "fonts/Bobbleboddy.ttf",
    "fonts/Bobbleboddy_light.ttf"
#endif
};

// ===========================================================
// Initialization & cleanup
// ===========================================================

GameScene::GameScene()
{
    srand(time(NULL));
    m_bDone = false;
    m_iSelectedAction = -1;
    m_fLastEnemyLaunchTime = 0;
}

GameScene::~GameScene()
{
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

// ===========================================================
// CCLayer methods
// ===========================================================

CCScene* GameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// ===========================================================
// PurchaseListener methods
// ===========================================================

void GameScene::onPurchaseSucceeded(const char * prodId, const char * receipt, const char * purchase)
{
//    SystemUtils::getInstance()->hideLoadingScreen();
//    
//    ProductInfo * pProductInfo = SystemUtils::getInstance()->getProductById(prodId);
//    CCLOG("pProductInfo=%p",pProductInfo);
//    if (NULL != pProductInfo)
//    {
//        CCLOG("pProductInfo->getId()=%s",pProductInfo->getId().c_str());
//        CCLOG("pProductInfo->getName()=%s",pProductInfo->getName().c_str());
//        CCLOG("pProductInfo->getLocalPrice()=%s",pProductInfo->getLocalPrice().c_str());
//        CCLOG("pProductInfo->getData()=%p",pProductInfo->getData());
//        
//        char fixedName[20];
//        memset(fixedName, 0, sizeof(fixedName));
//        const char * name = pProductInfo->getName().c_str();
//        const char * spacePtr = strchr(name, ' ');
//        if (NULL != spacePtr)
//        {
//            int index = spacePtr - name;
//            strncpy(fixedName, name, index);
//        }
//        else
//        {
//            strcpy(fixedName, name);
//        }
//        CCLOG("fixedName=%s",fixedName);
//        int coins = atoi(fixedName);
//        CCLOG("stars=%d",coins);
//        if (coins > 0 && coins < 10000)
//        {
//            Pref::getInstance()->incCoins(coins);
//            Pref::getInstance()->incPurchasedCoins(coins);
//            Pref::getInstance()->save();
//            
//            char eventData[100];
//            memset(eventData, 0, sizeof(eventData));
//            sprintf(eventData, "{\"product_id\":\"%s\", \"coins\":%d, \"price\":\"%s\"}", pProductInfo->getId().c_str(), coins, pProductInfo->getLocalPrice().c_str());
//            MuGames::getInstance()->track("transactionEnd", eventData);
//            
//            m_iBounsCount = Pref::getInstance()->getCoinsCount();
//            updateBounsText();
//            
//            startStarParticles(m_pBounsText->getPosition(), TAG_BOUNS);
//        }
//    }
}

void GameScene::onPurchaseFailed(const char * prodId, const char * msg)
{
    //SystemUtils::getInstance()->hideLoadingScreen();
}

void GameScene::onPurchaseCancelled()
{
    //SystemUtils::getInstance()->hideLoadingScreen();
}

// ===========================================================
// Chartboost methods
// ===========================================================

void GameScene::didCompleteRewardedVideo(int reward)
{
//    MuGames::getInstance()->track("rewardVideoFinished");
//    
//    Pref::getInstance()->incCoins(reward);
//    Pref::getInstance()->save();
//    
//    m_iBounsCount = Pref::getInstance()->getCoinsCount();
//    updateBounsText();
//    
//    // show win notification
//    Sprite * pCoinIcon = Sprite::createWithSpriteFrameName("cheap_coin.png");
//    
//    LayerColor * pBanner = LayerColor::create(Color4B(255, 255, 255, 255), getScreenWidth(), pCoinIcon->boundingBox().size.height * 1.2f);
//    
//    pCoinIcon->setPosition(pBanner->boundingBox().size.width * 0.1f, pBanner->boundingBox().size.height / 2);
//    pBanner->addChild(pCoinIcon);
//    
//    char text[100];
//    sprintf(text, "You won %d coins", reward);
//    CCLabelTTF * pText = CCLabelTTF::create(text, fontList[0], pCoinIcon->boundingBox().size.height * 0.4f);
//    pText->setColor(Color3B(0, 0, 0));
//    pText->setPosition(pCoinIcon->getPositionX() + pCoinIcon->boundingBox().size.width * 0.45f + pText->boundingBox().size.width / 2, pBanner->boundingBox().size.height / 2);
//    pBanner->addChild(pText);
//    
//    m_pGameOverLayer->addChild(pBanner);
//    
//    pBanner->setPositionY(-pBanner->boundingBox().size.height);
//    MoveTo * pMoveTo = MoveTo::create(0.5f, Vec2(0, 0));
//    MoveTo * pMoveOutTo = MoveTo::create(0.5f, Vec2(0, -pBanner->boundingBox().size.height));
//    CCFiniteTimeAction* pAnimDone = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeSprite));
//    CCFiniteTimeAction* rewardMessageShown = CCCallFuncN::create(this, callfuncN_selector(GameScene::rewardMessageShown));
//    pBanner->runAction(Sequence::create(DelayTime::create(0.5f), EaseExponentialOut::create(pMoveTo), rewardMessageShown, DelayTime::create(2.0f), EaseExponentialOut::create(pMoveOutTo), pAnimDone, NULL));
}

// ===========================================================
// BaseScene methods
// ===========================================================

void GameScene::handleInit(float screenWidth, float screenHeight)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
 
//    SystemUtils::getInstance()->prepareVideoAd();
//    SystemUtils::getInstance()->setPurchaseListener(this);
//    SystemUtils::getInstance()->initGameCenter();
//    SystemUtils::getInstance()->restoreFacebookSession();
    
    const char * batches[] = { "game0", "game1", NULL };
    loadBatch(batches, "pvr");
    
    Sprite * pBg = Sprite::createWithSpriteFrameName("bg.png");
    pBg->setPosition(screenWidth / 2, screenHeight / 2);
    addChild(pBg);
    
    strcpy(m_armyName, "rom");
    strcpy(m_oppArmyName, "vik");
    m_iLife = 100;
    m_iOppLife = 100;
    
    m_iCurrZ = FRONT_Z - 2;
    
    initBases();
    initButtons();
    
    m_pPauseButton = Sprite::createWithSpriteFrameName("pause_button.png");
    m_pPauseButton->setPosition(getScreenWidth() - m_pPauseButton->boundingBox().size.width * 0.7f, getScreenHeight() - m_pPauseButton->boundingBox().size.height * 0.7f);
    addChild(m_pPauseButton);
}

void GameScene::handleUpdate(float dt)
{
    if (m_bDone)
    {
        return;
    }
    
    m_fLastEnemyLaunchTime += dt;
    if (m_fLastEnemyLaunchTime > 2)
    {
        m_fLastEnemyLaunchTime = 0;
        launchOppSolider(m_pOppBases[rand() % 4], rand() % 5);
    }
    
    for (int i = 0; i < m_soliders.size(); i++)
    {
        Sprite * pSolider = m_soliders[i];
        SoliderData * pData = (SoliderData *)pSolider->getUserData();
        
        float newY = pSolider->getPositionY() + dt * pData->m_iSpeed * SPEED_FACTOR;
        pSolider->setPositionY(newY);
    }
    for (int i = 0; i < m_oppSoliders.size(); i++)
    {
        Sprite * pSolider = m_oppSoliders[i];
        SoliderData * pData = (SoliderData *)pSolider->getUserData();
        
        float newY = pSolider->getPositionY() - dt * pData->m_iSpeed * SPEED_FACTOR;
        pSolider->setPositionY(newY);
    }
    
    checkSoliderCollisions();
    checkBasesCollisions();
    checkOppBasesCollisions();
}

void GameScene::onImageDownloaded(const char * imageURL)
{
}

bool GameScene::handleOnTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* event)
{
    if (m_bDone)
    {
        return true;
    }
    
    m_pTouchedBase = NULL;
    m_pTouchedAction = NULL;
    
    for (int i = 0; NULL == m_pTouchedBase && i < NUM_OF_BASES; i++)
    {
        if (isClicked(m_pBases[i], pTouch))
        {
            m_pTouchedBase = m_pBases[i];
            m_pTouchedBase->setColor(Color3B(200, 200, 200));
        }
    }
    
    if (NULL == m_pTouchedBase)
    {
        for (int i = 0; NULL == m_pTouchedAction && i < NUM_OF_ACTIONS; i++)
        {
            if (isClicked(m_pActions[i], pTouch))
            {
                m_pTouchedAction = m_pActions[i];
                m_pTouchedAction->setScale(0.95f);
            }
        }
    }
    
    return true;
}

void GameScene::handleOnTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* event)
{
    if (m_bDone)
    {
        return;
    }
    
    if (NULL != m_pTouchedBase && !isClicked(m_pTouchedBase, pTouch))
    {
        m_pTouchedBase->setColor(Color3B(255, 255, 255));
        m_pTouchedBase = NULL;
    }
    else if (NULL != m_pTouchedAction && !isClicked(m_pTouchedAction, pTouch))
    {
        m_pTouchedAction->setScale(1);
        m_pTouchedAction = NULL;
    }
}

void GameScene::handleOnTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* event)
{
    if (m_bDone)
    {
        return;
    }
    
    const char * actions[NUM_OF_ACTIONS] = {
        "deactivated_sword.png",
        "deactivated_shield.png",
        "deactivated_ice1.png",
        "deactivated_switch.png",
        "deactivated_fast.png"
    };
    
    if (NULL != m_pTouchedBase)
    {
        if (isClicked(m_pTouchedBase, pTouch))
        {
            if (-1 != m_iSelectedAction)
            {
                if (launchSolider(m_pTouchedBase, m_iSelectedAction))
                {
                    Sprite * pButton = (Sprite *)m_pActions[m_iSelectedAction]->getUserData();
                    Sprite * pCounterBg = (Sprite *)pButton->getUserData();
                    CCLabelTTF * pText = (CCLabelTTF *)pCounterBg->getChildren().at(0);
                    
                    int count = pText->getTag() - 1;
                    
                    pText->setTag(count);
                    char text[10];
                    sprintf(text, "%d", count);
                    pText->setString(text);
                    
                    if (0 == count)
                    {
                        pCounterBg->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("grey_counter.png"));
                        
                        pButton->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("grey_base.png"));
                        
                        m_pActions[m_iSelectedAction]->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(actions[m_iSelectedAction]));
                        
                        m_iSelectedAction = -1;
                    }
                }
            }
        }
        m_pTouchedBase->setColor(Color3B(255, 255, 255));
        m_pTouchedBase = NULL;
    }
    else if (NULL != m_pTouchedAction)
    {
        if (isClicked(m_pTouchedAction, pTouch))
        {
            if (-1 != m_iSelectedAction)
            {
                Sprite * pButton = (Sprite *)m_pActions[m_iSelectedAction]->getUserData();
                
                pButton->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("grey_base.png"));
                ((Sprite *)pButton->getUserData())->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("grey_counter.png"));
                m_pActions[m_iSelectedAction]->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(actions[m_iSelectedAction]));
            }
            m_iSelectedAction = -1;
            
            Sprite * pButton = (Sprite *)m_pActions[m_pTouchedAction->getTag()]->getUserData();
            Sprite * pCounterBg = (Sprite *)pButton->getUserData();
            CCLabelTTF * pText = (CCLabelTTF *)pCounterBg->getChildren().at(0);
            if (pText->getTag() > 0)
            {
                m_iSelectedAction = m_pTouchedAction->getTag();
                
                pCounterBg->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("gold_counter.png"));
                
                pButton->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("gold_base.png"));
                
                m_pActions[m_iSelectedAction]->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(&actions[m_iSelectedAction][2]));
            }
        }
        m_pTouchedAction->setScale(1);
        m_pTouchedAction = NULL;
    }
}

void GameScene::handleOnTouchCancelled(cocos2d::Touch* pTouch, cocos2d::Event* event)
{
    handleOnTouchEnded(pTouch, event);
}

// ===========================================================
// Private methods
// ===========================================================

void GameScene::initBases()
{
    float x = getScreenWidth() * 0.14f;
    for (int i = 0; i < NUM_OF_BASES; i++)
    {
        Sprite * pLine = Sprite::createWithSpriteFrameName("road.png");
        pLine->setPosition(x, getScreenHeight() / 2);
        addChild(pLine);
        
        x += getScreenWidth() * 0.245f;
    }

    Sprite * pLine = (Sprite *)getChildren().at(getChildrenCount() - 1);
    
    x = getScreenWidth() * 0.14f;
    for (int i = 0; i < NUM_OF_BASES; i++)
    {
        m_pBases[i] = Sprite::createWithSpriteFrameName("rom_warrior_appear_0001.png");
        m_pBases[i]->setPosition(x, pLine->getPositionY() - pLine->boundingBox().size.height / 2 + m_pBases[i]->boundingBox().size.height * 0.4f);
        m_pBases[i]->setTag(-1);
        addChild(m_pBases[i], FRONT_Z);
        
        m_pOppBases[i] = Sprite::createWithSpriteFrameName("vik_warrior_app_0001.png");
        m_pOppBases[i]->setPosition(x, pLine->getPositionY() + pLine->boundingBox().size.height / 2 + m_pBases[i]->boundingBox().size.height * 0.4f);
        m_pOppBases[i]->setTag(-1);
        addChild(m_pOppBases[i], 1);
        
        x += getScreenWidth() * 0.245f;
    }
}

void GameScene::initButtons()
{
    Sprite * pPanel = Sprite::createWithSpriteFrameName("bottom_dark_panel.png");
    pPanel->setPosition(getScreenWidth() / 2, pPanel->boundingBox().size.height / 2);
    addChild(pPanel);
    
    const char * actions[NUM_OF_ACTIONS] = {
        "deactivated_sword.png",
        "deactivated_shield.png",
        "deactivated_ice1.png",
        "deactivated_switch.png",
        "deactivated_fast.png"
    };
    float x = pPanel->boundingBox().size.width * 0.14f;
    float y = pPanel->boundingBox().size.height * 0.5f;
    for (int i = 0; i < NUM_OF_ACTIONS; i++)
    {
        Sprite * pButton = Sprite::createWithSpriteFrameName("grey_base.png");
        pButton->setPosition(x, y);
        pPanel->addChild(pButton);
        
        m_pActions[i] = Sprite::createWithSpriteFrameName(actions[i]);
        m_pActions[i]->setPosition(x, y);
        m_pActions[i]->setTag(i);
        m_pActions[i]->setUserData(pButton);
        pPanel->addChild(m_pActions[i]);
        
        Sprite * pCounterBg = Sprite::createWithSpriteFrameName("grey_counter.png");
        pCounterBg->setPosition(pButton->getPositionX(), pButton->getPositionY() - pButton->boundingBox().size.height / 2);
        pPanel->addChild(pCounterBg);
        pButton->setUserData(pCounterBg);
        
        CCLabelTTF * pCountText = CCLabelTTF::create("10", fontList[0], pCounterBg->boundingBox().size.height * 0.54f);
        pCountText->setPosition(pCounterBg->boundingBox().size.width / 2, pCounterBg->boundingBox().size.height * 0.53f);
        pCountText->setTag(10);
        pCounterBg->addChild(pCountText);
        
        x += getScreenWidth() * 0.15f;
    }
    
    Sprite * pLifeBarBg = Sprite::createWithSpriteFrameName("life_bar_base.png");
    pLifeBarBg->setPosition(pPanel->boundingBox().size.width / 2, pPanel->boundingBox().size.height - pLifeBarBg->boundingBox().size.height * 0.7f);
    pPanel->addChild(pLifeBarBg);
    
    Sprite * pFillSprite = Sprite::createWithSpriteFrameName("life_bar_green.png");
    m_pLife = CCProgressTimer::create(pFillSprite);
    m_pLife->setType(ProgressTimer::Type::BAR);
    m_pLife->setBarChangeRate(Vec2(1,0));
    m_pLife->setMidpoint(Vec2(0, 0));
    m_pLife->setPosition(pLifeBarBg->getPosition());
    m_pLife->setPercentage(100);
    pPanel->addChild(m_pLife);
}

bool GameScene::launchSolider(cocos2d::Sprite * pBase, int type)
{
    if (-1 != pBase->getTag())
    {
        return false;
    }
    
    pBase->setTag(type);
    
    char prefix[50];
    switch (type)
    {
        case ACTION_WARRIOR : sprintf(prefix, "%s_warrior_appear_00", m_armyName); break;
        case ACTION_DEFENDER : sprintf(prefix, "%s_defender_app_00", m_armyName); break;
        case ACTION_MAGICAN : sprintf(prefix, "%s_magician_app_00", m_armyName); break;
        case ACTION_SLOWER : sprintf(prefix, "%s_slower_app_00", m_armyName); break;
        case ACTION_HEALER : sprintf(prefix, "%s_healer_app_00", m_armyName); break;
    }
    
    Vector<SpriteFrame*> array(18);
    for (int i = 1; i <= 18; i++)
    {
        char str[50];
        sprintf(str, "%s%02d.png", prefix, i);
        array.pushBack(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str));
    }
    Animation * pAnimFrames = Animation::createWithSpriteFrames(array, 50.0f/1000.0f);
    CCFiniteTimeAction* pAnimDone = CCCallFuncN::create(this, callfuncN_selector(GameScene::onLaunchSoliderDone));
    pBase->runAction(Sequence::createWithTwoActions(CCAnimate::create(pAnimFrames), pAnimDone));
    
    return true;
}

void GameScene::onLaunchSoliderDone(cocos2d::Node * pBase)
{
    ((Sprite *)pBase)->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("rom_warrior_appear_0001.png"));
    
    int type = pBase->getTag();
    
    char prefix[50];
    switch (type)
    {
        case ACTION_WARRIOR : sprintf(prefix, "%s_warrior_walk_00", m_armyName); break;
        case ACTION_DEFENDER : sprintf(prefix, "%s_defender_walk_00", m_armyName); break;
        case ACTION_MAGICAN : sprintf(prefix, "%s_magician_walk_00", m_armyName); break;
        case ACTION_SLOWER : sprintf(prefix, "%s_slower_walk_00", m_armyName); break;
        case ACTION_HEALER : sprintf(prefix, "%s_healer_walk_00", m_armyName); break;
    }
    
    char spriteName[50];
    switch (type)
    {
        case ACTION_WARRIOR : sprintf(spriteName, "%s01.png", prefix); break;
        case ACTION_DEFENDER : sprintf(spriteName, "%s01.png", prefix); break;
        case ACTION_MAGICAN : sprintf(spriteName, "%s01.png", prefix); break;
        case ACTION_SLOWER : sprintf(spriteName, "%s01.png", prefix); break;
        case ACTION_HEALER : sprintf(spriteName, "%s01.png", prefix); break;
    }
    Sprite * pSolider = Sprite::createWithSpriteFrameName(spriteName);
    pSolider->setPosition(pBase->getPosition());
    addChild(pSolider, FRONT_Z);
    
    SoliderData * pData = new SoliderData();
    pData->m_iLife = 1;
    pData->m_iSpeed = 1;
    pData->m_iType = type;
    pSolider->setUserData(pData);
    
    m_soliders.push_back(pSolider);
    
    Vector<SpriteFrame*> array(10);
    for (int i = 1; i <= 10; i++)
    {
        char str[50];
        sprintf(str, "%s%02d.png", prefix, i);
        array.pushBack(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str));
    }
    Animation * pAnimFrames = Animation::createWithSpriteFrames(array, 50.0f/1000.0f);
    pAnimFrames->setLoops(-1);
    pSolider->runAction(CCAnimate::create(pAnimFrames));

    pBase->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CCCallFuncN::create(this, callfuncN_selector(GameScene::reEnableBase))));
    
    pSolider->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CCCallFuncN::create(this, callfuncN_selector(GameScene::decSpriteZ))));
}

void GameScene::decSpriteZ(cocos2d::Node * pBase)
{
    pBase->setZOrder(pBase->getZOrder() - 1);
}

void GameScene::launchOppSolider(cocos2d::Sprite * pBase, int type)
{
    if (-1 != pBase->getTag())
    {
        return;
    }
    
    pBase->setTag(type);
    
    char prefix[50];
    switch (type)
    {
        case ACTION_WARRIOR : sprintf(prefix, "%s_warrior_app_00", m_oppArmyName); break;
        case ACTION_DEFENDER : sprintf(prefix, "%s_defender_app_00", m_oppArmyName); break;
        case ACTION_MAGICAN : sprintf(prefix, "%s_mag_app_00", m_oppArmyName); break;
        case ACTION_SLOWER : sprintf(prefix, "%s_slw_app_00", m_oppArmyName); break;
        case ACTION_HEALER : sprintf(prefix, "%s_healer_app_00", m_oppArmyName); break;
    }
    
    Vector<SpriteFrame*> array(18);
    for (int i = 1; i <= 18; i++)
    {
        char str[50];
        sprintf(str, "%s%02d.png", prefix, i);
        array.pushBack(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str));
    }
    Animation * pAnimFrames = Animation::createWithSpriteFrames(array, 50.0f/1000.0f);
    CCFiniteTimeAction* pAnimDone = CCCallFuncN::create(this, callfuncN_selector(GameScene::onLaunchOppSoliderDone));
    pBase->runAction(Sequence::createWithTwoActions(CCAnimate::create(pAnimFrames), pAnimDone));
}

void GameScene::onLaunchOppSoliderDone(cocos2d::Node * pBase)
{
    ((Sprite *)pBase)->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("vik_warrior_app_0001.png"));
    
    int type = pBase->getTag();
    
    char prefix[50];
    switch (type)
    {
        case ACTION_WARRIOR : sprintf(prefix, "%s_warrior_walk_00", m_oppArmyName); break;
        case ACTION_DEFENDER : sprintf(prefix, "%s_def_walk_00", m_oppArmyName); break;
        case ACTION_MAGICAN : sprintf(prefix, "%s_magician_walk_00", m_oppArmyName); break;
        case ACTION_SLOWER : sprintf(prefix, "%s_slower_walk_00", m_oppArmyName); break;
        case ACTION_HEALER : sprintf(prefix, "%s_healer_walk_00", m_oppArmyName); break;
    }
    
    char spriteName[50];
    switch (type)
    {
        case ACTION_WARRIOR : sprintf(spriteName, "%s01.png", prefix); break;
        case ACTION_DEFENDER : sprintf(spriteName, "%s01.png", prefix); break;
        case ACTION_MAGICAN : sprintf(spriteName, "%s01.png", prefix); break;
        case ACTION_SLOWER : sprintf(spriteName, "%s01.png", prefix); break;
        case ACTION_HEALER : sprintf(spriteName, "%s01.png", prefix); break;
    }
    
    Sprite * pSolider = Sprite::createWithSpriteFrameName(spriteName);
    pSolider->setPositionX(pBase->getPositionX() - pBase->boundingBox().size.width * 0.02f);
    pSolider->setPositionY(pBase->getPositionY() - pBase->boundingBox().size.height * 0.055f);
    addChild(pSolider, m_iCurrZ--);
    
    SoliderData * pData = new SoliderData();
    pData->m_iLife = 1;
    pData->m_iSpeed = 1;
    pData->m_iType = type;
    pSolider->setUserData(pData);
    
    m_oppSoliders.push_back(pSolider);
    
    Vector<SpriteFrame*> array(10);
    for (int i = 1; i <= 10; i++)
    {
        char str[50];
        sprintf(str, "%s%02d.png", prefix, i);
        array.pushBack(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str));
    }
    Animation * pAnimFrames = Animation::createWithSpriteFrames(array, 50.0f/1000.0f);
    pAnimFrames->setLoops(-1);
    pSolider->runAction(CCAnimate::create(pAnimFrames));
    
    pBase->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CCCallFuncN::create(this, callfuncN_selector(GameScene::reEnableBase))));
    
    
    Sprite * pSign = Sprite::createWithSpriteFrameName("sign.png");
    pSign->setScale(1.4f);
    pSign->setPosition(pSolider->boundingBox().size.width * 0.55f, pSolider->boundingBox().size.height * 0.99f);
    pSolider->addChild(pSign);
}

void GameScene::reEnableBase(cocos2d::Node * pSender)
{
    pSender->setTag(-1);
}

void GameScene::checkSoliderCollisions()
{
    std::vector<Sprite *>::iterator iter = m_soliders.begin();
    while (iter != m_soliders.end())
    {
        Sprite * pMe = *iter;
        SoliderData * pMeData = (SoliderData *)pMe->getUserData();
        
        Rect meRect = Rect(
                           pMe->getPosition().x - (pMe->getContentSize().width/2),
                           pMe->getPosition().y - (pMe->getContentSize().height/2),
                           pMe->getContentSize().width,
                           pMe->getContentSize().height);
        
        bool removeMe = false;
        bool removeOpp = false;
        std::vector<Sprite *>::iterator oppIter = m_oppSoliders.begin();
        while (!removeMe && !removeOpp && oppIter != m_oppSoliders.end())
        {
            Sprite * pOther = *oppIter;
            SoliderData * pOtherData = (SoliderData *)pOther->getUserData();
            
            Rect otherRect = Rect(
                                  pOther->getPosition().x - (pOther->getContentSize().width/2),
                                  pOther->getPosition().y - (pOther->getContentSize().height/2),
                                  pOther->getContentSize().width,
                                  pOther->getContentSize().height);
            
            if (meRect.intersectsRect(otherRect))
            {
                if (pMeData->m_iLife > 0 && pOtherData->m_iLife > 0) // not dead yet
                {
                    int result = solideToSoliderMatch(pMeData, pOtherData);
                    if (0 == result) // i win
                    {
                        removeOpp = true;
                    }
                    else
                    {
                        removeMe = true;
                    }
                }
            }
            
            if (!removeOpp && !removeMe)
            {
                oppIter++;
            }
        }
        
        if (removeOpp)
        {
            startKillAnim(*oppIter);
            m_oppSoliders.erase(oppIter);
        }
        if (removeMe)
        {
            startKillAnim(*iter);
            iter = m_soliders.erase(iter);
        }
        
        iter++;
    }
}

void GameScene::startKillAnim(cocos2d::Sprite * pSolider)
{
    Sprite * pSmoke = Sprite::createWithSpriteFrameName("smoke_expl_0001.png");
    pSmoke->setPosition(pSolider->getPosition());
    pSmoke->setZOrder(pSolider->getZOrder());
    addChild(pSmoke);
    
    Vector<SpriteFrame*> array(8);
    for (int i = 1; i <= 8; i++)
    {
        char str[50];
        sprintf(str, "smoke_expl_00%02d.png", i);
        array.pushBack(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str));
    }
    Animation * pAnimFrames = Animation::createWithSpriteFrames(array, 50.0f/1000.0f);
    CCFiniteTimeAction* pSmokeAnimDone = CCCallFuncN::create(this, callfuncN_selector(GameScene::removeSprite));
    pSmoke->runAction(Sequence::createWithTwoActions(CCAnimate::create(pAnimFrames), pSmokeAnimDone));
    
    CCFiniteTimeAction* pScaleAnimDone = CCCallFuncN::create(this, callfuncN_selector(GameScene::onKillAnimDone));
    pSolider->stopAllActions();
    pSolider->runAction(Sequence::createWithTwoActions(ScaleTo::create(0.25f, 0), pScaleAnimDone));
}

void GameScene::removeSprite(cocos2d::Node * pSender)
{
    removeChild(pSender);
}

void GameScene::onKillAnimDone(cocos2d::Node * pSender)
{
    SoliderData * pData = (SoliderData *)pSender->getUserData();
    delete pData;
    pSender->setUserData(NULL);
    
    removeChild(pSender);
}

void GameScene::checkBasesCollisions()
{
    std::vector<Sprite *>::iterator iter = m_soliders.begin();
    while (iter != m_soliders.end())
    {
        Sprite * pOpp = *iter;
        SoliderData * pMeData = (SoliderData *)pOpp->getUserData();
        
        bool removed = false;
        for (int j = 0; !removed && j < NUM_OF_BASES; j++)
        {
            Sprite * pBase = m_pOppBases[j];
            float dx = fabsf(pBase->getPositionX() - pOpp->getPositionX());
            if (dx < pBase->boundingBox().size.width / 2 && pOpp->getPositionY() + pOpp->boundingBox().size.height * 0.5f > pBase->getPositionY())
            {
                if (ACTION_WARRIOR == pMeData->m_iType)
                {
                    m_iOppLife--;
                    //m_pLife->setPercentage(m_iLife);
                }
                pOpp->setZOrder(pBase->getZOrder());
                removed = true;
            }
        }
        
        if (removed)
        {
            startKillAnim(*iter);
            iter = m_soliders.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void GameScene::checkOppBasesCollisions()
{
    std::vector<Sprite *>::iterator iter = m_oppSoliders.begin();
    while (iter != m_oppSoliders.end())
    {
        Sprite * pOpp = *iter;
        SoliderData * pMeData = (SoliderData *)pOpp->getUserData();
        
        bool removed = false;
        for (int j = 0; !removed && j < NUM_OF_BASES; j++)
        {
            Sprite * pBase = m_pBases[j];
            float dx = fabsf(pBase->getPositionX() - pOpp->getPositionX());
            if (dx < pBase->boundingBox().size.width / 2 && pOpp->getPositionY() - pOpp->boundingBox().size.height * 0.1f < pBase->getPositionY())
            {
                if (ACTION_WARRIOR == pMeData->m_iType)
                {
                    m_iLife--;
                    m_pLife->setPercentage(m_iLife);
                }
                pOpp->setZOrder(pBase->getZOrder());
                removed = true;
            }
        }
        
        if (removed)
        {
            startKillAnim(*iter);
            iter = m_oppSoliders.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

int GameScene::solideToSoliderMatch(SoliderData * pMe, SoliderData * pOther)
{
    return 0;
}