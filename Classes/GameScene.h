#ifndef __GAME_SCENE_TAP_BATTLES_H__
#define __GAME_SCENE_TAP_BATTLES_H__

#include "cocos2d.h"
#include "BaseScene.h"
#include "Pref.h"
#include "PurchaseListener.h"
#include "Challenge.h"

#define NUM_OF_BASES 4
#define NUM_OF_ACTIONS 5

class GameRecorder;
class GameRecording;
class MultiplayerMessage;

class GameScene : public BaseScene, public PurchaseListener
{
public:
    
    // ===========================================================
    // Initialization & cleanup
    // ===========================================================
    
    GameScene();
    ~GameScene();
    
    // ===========================================================
    // CCLayer methods
    // ===========================================================
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(GameScene);
    
    // ===========================================================
    // PurchaseListener methods
    // ===========================================================
    
    void onPurchaseSucceeded(const char * prodId, const char * receipt, const char * purchase);
    void onPurchaseFailed(const char * prodId, const char * msg);
    void onPurchaseCancelled();
    
    // ===========================================================
    // Chartboost methods
    // ===========================================================
    
    void didCompleteRewardedVideo(int reward);
    
    // ===========================================================
    // Mutliplayer events
    // ===========================================================
    
    void onGameRecordingLoaded(GameRecording * pRecording) {}
    void onLoadChallengesDone(std::vector<Challenge> challenges) {}
    void onQuickMatchFound(const char * gameId, int birdType, const char * fbId, const char * name) {}
    void onLoginDone() {}
    void onInviteDone(std::vector<std::string> invitedIds) {}
    void onSendChallengeDone(bool succeeded) {}
  
protected:
    
    // ===========================================================
    // BaseScene methods
    // ===========================================================
    
    void handleInit(float width, float height);
    void handleUpdate(float dt);
    
    void onImageDownloaded(const char * imageURL);
    
    bool handleOnTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void handleOnTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void handleOnTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void handleOnTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);
    
private:
    
    // ===========================================================
    // SoliderData
    // ===========================================================
    
    struct SoliderData {
        int m_iType;
        int m_iSpeed;
        int m_iLife;
    };
    
    // ===========================================================
    // Fields
    // ===========================================================
    
    bool m_bDone;
    cocos2d::Sprite * m_pBases[NUM_OF_BASES];
    cocos2d::Sprite * m_pOppBases[NUM_OF_BASES];
    cocos2d::Sprite * m_pActions[NUM_OF_ACTIONS];
    cocos2d::Sprite * m_pTouchedBase;
    cocos2d::Sprite * m_pTouchedAction;
    std::vector<cocos2d::Sprite *> m_soliders;
    std::vector<cocos2d::Sprite *> m_oppSoliders;
    int m_iSelectedAction;
    char m_armyName[30];
    char m_oppArmyName[30];
    float m_fLastEnemyLaunchTime;
    int m_iLife;
    int m_iOppLife;
    int m_iCurrZ;
    cocos2d::CCProgressTimer * m_pLife;
    cocos2d::CCProgressTimer * m_pOppLife;
    cocos2d::Sprite * m_pPauseButton;
    
    // ===========================================================
    // Private methods
    // ===========================================================
    
    void initBases();
    void initButtons();
    bool launchSolider(cocos2d::Sprite * pBase, int type);
    void decSpriteZ(cocos2d::Node * pBase);
    void onLaunchSoliderDone(cocos2d::Node * pSender);
    void launchOppSolider(cocos2d::Sprite * pBase, int type);
    void onLaunchOppSoliderDone(cocos2d::Node * pSender);
    void reEnableBase(cocos2d::Node * pSender);
    void checkSoliderCollisions();
    void startKillAnim(cocos2d::Sprite * pSolider);
    void removeSprite(cocos2d::Node * pSender);
    void onKillAnimDone(cocos2d::Node * pSender);
    void checkBasesCollisions();
    void checkOppBasesCollisions();
    int solideToSoliderMatch(SoliderData * pMe, SoliderData * pOther);
};

#endif // __GAME_SCENE_TAP_BATTLES_H__
