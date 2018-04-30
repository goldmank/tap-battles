#ifndef __SYSTEM_UTILS_IOS__
#define __SYSTEM_UTILS_IOS__

#include "SystemUtils.h"
#include "Challenge.h"

class SystemUtilsIOS : public SystemUtils
{
public:

    // ===========================================================
    // Initialization & cleanup
    // ===========================================================
    
    SystemUtilsIOS();
    ~SystemUtilsIOS();
    
	// =========================================================== 
    // Public methods
    // ===========================================================
	
    void initGameCenter();
    void showAchivments();
	void showLeaderboard();
	void sendScore(int score);
    void sendAchivment(const char * name);
    void shareScore(int score);
    void invite(int score);
	void purchase(int count);
	void rateUs();
	void onExit();
	void trackPage(const char * page);
    void trackEvent(const char * event, const char * paramName, const char * paramValue);
    void trackTimedEvent(const char * event);
    bool isVideoAdReady();
    void prepareVideoAd();
    void showVideoAd();
    bool isInterstitialAdReady();
    void prepareInterstitialAd();
    void showInterstitialAd();
    void showLoadingScreen();
    void hideLoadingScreen();
    
    void multiplayerLogin();
    void sendGameRecording(GameRecording * pGameRecording, std::vector<std::string> invitedIds);
    void loadGameRecording(const char * gameId);
    void loadChallenges(bool cacheOnly = false);
    void findQuickMatch();
    void clearChallange(std::string gameId);
    
    void notifyUserMinActionComplete();
    
    void restoreFacebookSession();
    void loginToFacebook();
    
private:
    
    // ===========================================================
    // Fields
    // ===========================================================
    
    bool m_bShowingLoading;
    void * m_pLoadingView;
    void * m_pLoading;
    bool m_bLoginToInvite;
    int m_iScoreForPostLoginInvite;
    std::vector<std::string> m_fbIds;
    
    // ===========================================================
    // Private methods
    // ===========================================================
    
    void onLoadGameRecordingDone(GameRecording * pGameRecording);
    void onLoadChallengesDone(std::vector<Challenge> challenges);
    void onQuickMatchFound(const char * gameId, int birdType, const char * fbId, const char * name);
    void getUserDetails();
    void updateFbUserList();
    void restoreFbUserList();
};

#endif // __SYSTEM_UTILS_IOS__
