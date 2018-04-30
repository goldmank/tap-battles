#ifndef __SYSTEM_UTILS__
#define __SYSTEM_UTILS__

#include "cocos2d.h"
#include <stdio.h>
#include <string>
#include "PurchaseListener.h"
#include "ProductInfo.h"

class GameRecording;

class SystemUtils
{
public:
    
	// ===========================================================
    // Singleton methods
    // ===========================================================

	static void initInstance(SystemUtils * pSystemUtils);
	static SystemUtils * getInstance();

	// =========================================================== 
    // Public methods
    // ===========================================================
	
    virtual void initGameCenter() {}
    virtual void showAchivments() {}
	virtual void showLeaderboard() {}
	virtual void sendScore(int score) {}
    virtual void sendAchivment(const char * name) {}
    virtual void shareScore(int score) {}
    virtual void invite(int score) {}
	virtual void purchase(int count) {}
	virtual void rateUs() {}
	virtual void trackPage(const char * page) {}
    virtual void trackEvent(const char * event, const char * paramName, const char * paramValue) {}
    virtual void trackTimedEvent(const char * event) {}
    virtual bool isVideoAdReady() { return false; }
    virtual void prepareVideoAd() {}
    virtual void showVideoAd() {}
    virtual bool isInterstitialAdReady() { return false; }
    virtual void prepareInterstitialAd() {}
    virtual void showInterstitialAd() {}
    virtual void showLoadingScreen() {}
	virtual void hideLoadingScreen() {}
    virtual void multiplayerLogin() {}
    virtual void sendGameRecording(GameRecording * pGameRecording, std::vector<std::string> invitedIds) {}
    virtual void loadGameRecording(const char * gameId) {}
    virtual void loadChallenges(bool cacheOnly = false) {}
    virtual void findQuickMatch() {}
    virtual void clearChallange(std::string gameId) {}
    virtual void notifyUserMinActionComplete() {}
    virtual void restoreFacebookSession() {}
    virtual void loginToFacebook() {}
    
    void setPurchaseListener(PurchaseListener * listener) { m_pPurchaseListener = listener; }
    PurchaseListener * getPurchaseListener() { return m_pPurchaseListener; }
    
    void initProducts(std::vector<ProductInfo> & products);
    void addProductInfo(const ProductInfo & prodInfo);
    ProductInfo * getProductByIndex(int index);
    ProductInfo * getProductById(const char * itemId);
    
protected:

	// =========================================================== 
    // Constructor
    // ===========================================================

	SystemUtils();

private:

	static SystemUtils * s_pInstance;
    
    PurchaseListener * m_pPurchaseListener;
    std::vector<ProductInfo> m_products;
};

#endif // __SYSTEM_UTILS__
