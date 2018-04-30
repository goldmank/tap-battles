#ifndef __BASE_SCENE_FLYGUY_H__
#define __BASE_SCENE_FLYGUY_H__

#include "cocos2d.h"
#include "BaseScene.h"

class SplashScene : public BaseScene
{
public:
    
    // ===========================================================
    // Initialization & cleanup
    // ===========================================================
    
    SplashScene();
    ~SplashScene();
    
    // ===========================================================
    // CCLayer methods
    // ===========================================================
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(SplashScene);
    
protected:
    
    // ===========================================================
    // BaseScene methods
    // ===========================================================
    
    void handleInit(float width, float height);
    void handleUpdate(float dt);
    
    bool handleOnTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void handleOnTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void handleOnTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void handleOnTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);
    
private:
    
    // ===========================================================
    // Fields
    // ===========================================================
    
    cocos2d::LayerColor * m_pMuLayer;
    cocos2d::Sprite * m_pMuBg;
    
    // ===========================================================
    // Private methods
    // ===========================================================
    
    void onMuLogoDone(cocos2d::Node * pSender);
    void onFlamboLogoDone(cocos2d::Node * pSender);
};

#endif // __BASE_SCENE_FLYGUY_H__
