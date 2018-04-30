#ifndef __BASE_SCENE_H__
#define __BASE_SCENE_H__

#include "cocos2d.h"
#include "../cocos/network/HttpClient.h"

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

class BaseScene : public cocos2d::LayerColor
{
public:
	// =========================================================== 
    // Initialization & cleanup
    // ===========================================================

	BaseScene();
	virtual ~BaseScene();

	// =========================================================== 
    // CCLayer methods
    // ===========================================================

    virtual bool init();

	// =========================================================== 
    // Public methods
    // ===========================================================

	void update(float dt);
        
protected:
	
    // ===========================================================
    // LoadingImageDetails
    // ===========================================================
    
    struct LoadingImageDetailsItem {
        cocos2d::CCNode * pContainer;
        cocos2d::CCPoint pos;
    };
    
    struct LoadingImageDetails {
        std::vector<LoadingImageDetailsItem> items;
    };
    
    // ===========================================================
    // Types
    // ===========================================================
    
    typedef std::pair<std::string, LoadingImageDetails> URLMapPair;
    
    // ===========================================================
    // Fields
    // ===========================================================
    
    virtual bool handleOnTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) { return false; }
    virtual void handleOnTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {}
    virtual void handleOnTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {}
    virtual void handleOnTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event) {}
    
	// =========================================================== 
    // Common methods
    // ===========================================================
	
	virtual void handleInit(float width, float height);
	virtual void handleUpdate(float dt);
    
    virtual void onImageDownloaded(const char * imageURL) {}
    
	// =========================================================== 
    // Helper methods
    // ===========================================================

	int getScreenWidth();
	int getScreenHeight();
	bool isClicked(cocos2d::CCNode * pSprite, cocos2d::Touch * pTouch);
	float getTime();
	void switchScene(cocos2d::CCScene * pScene);
    virtual cocos2d::ccColor3B getFillColor();
    void loadBatch(const char ** batches, const char * postfix);
    void playBackgroundMusic();
    void stopBackgroundMusic();
    void playSound(const char * name);
    LoadingImageDetails getLoadingImageDetails(const char * url);
    void addLoadingImageDetails(const char * url, cocos2d::CCNode * pNode, cocos2d::CCPoint pos);
    void removeLoadingImageDetails(const char * url);
    void clearAllLoadingImageDetails();
    bool isImageExist(const char * url);
    void downloadImage(const char * url);
    const char * getImageCacheFileName(char * dest, const char * url);
    cocos2d::CCTexture2D * loadTexture(const char * url);
    
private:

	// =========================================================== 
    // Fields
    // ===========================================================

    cocos2d::EventListenerTouchOneByOne * m_pTouchListener;
	int m_iScreenWidth;
	int m_iScreenHeight;
	float m_fTime;
    std::map<std::string, LoadingImageDetails> m_mapUrlToSprite;
    std::set<std::string> m_downloadingImages;
    
	// =========================================================== 
    // Private methods
    // ===========================================================

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void imageLoadedCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
};

#endif // __BASE_SCENE_H__
