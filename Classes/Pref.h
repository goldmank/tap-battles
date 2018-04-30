#ifndef __PREF_H__
#define __PREF_H__

#include <string>
#include "cocos2d.h"
#include "Settings.h"

#define COLOR_BG 0
#define COLOR_FILL 1
#define COLOR_BORDER 2
#define COLOR_ENEMY 3
#define MAX_BIRDS 100

#define BACKGROUND_MOUNTAIN 0
#define BACKGROUND_CITY     1
#define BACKGROUND_RAIN     2
#define BACKGROUND_SNOW     3

class FbUser
{
    std::string m_name;
    std::string m_fbId;
};

class Pref
{
public:
    
    // ===========================================================
    // Singleton methods
    // ===========================================================

	static Pref * getInstance();
	
	// =========================================================== 
    // Public methods
    // ===========================================================
	
	void setBestScore(int score);
    int getBestScore();
    int getBestScoreUpdateCount();
    bool shouldShowGuide() { return m_bShowGuide; }
    void setShowGuide(bool show) { m_bShowGuide = show; }
    int getNumberOfGames() { return m_iGameCounts; }
    void countGame() { m_iGameCounts++; }
    bool isSoundEnabled();
    void enableSound(bool enable);
    void preloadSounds();
    int getSelectedBird() { return m_iSelectedBird; }
    void setSelectedBird(int index) { m_iSelectedBird = index; }
    bool isBirdUnlocked(int index);
    void unlockBird(int index);
    int getCoinsCount() { return m_iCoins; }
    void decCoinsCount(int count) { m_iCoins -= count; }
    void incCoins(int count) { m_iCoins += count; }
    int getUnlockedBirdsCount();
    int getCurrBackground() { return m_iBackground; }
    void setCurrBackground(int b) { m_iBackground = b; }
    time_t getLastSettingsUpdate() { return m_tSettingsUpdate; }
    void setLastSettingsUpdate(time_t t) { m_tSettingsUpdate = t; }
    void updateSettings(const char * str);
    Settings & getSettings() { return m_settings; }    
    time_t getInstallTime() { return m_tInstallTime; }
    int getDaysSinceInstall();
    void setUserDetails(const char * fbId, const char * username);
    const char * getFbId();
    const char * getUserName();
    int getMultiplayerWins() { return m_iMultiplayerWins; }
    void incMultiplayerWins() { m_iMultiplayerWins++; }
    void setMultiplayerWins(int count) { m_iMultiplayerWins = count; }
    int getMultiplayerLevel();
    void setLastChallangesLoadTime(time_t t) { m_tLastChallangesLoadTime = t; }
    time_t getLastChallangesLoadTime() { return m_tLastChallangesLoadTime; }
    bool isGalleryHintShown() { return m_bGalleryHintShown; }
    void setGalleryHintShown(bool value) { m_bGalleryHintShown = value; }
    bool isClickedShareForCoinsBanner() { return m_clickedShareForCoinsBanner; }
    void setClickedShareForCoinsBanner(bool value) { m_clickedShareForCoinsBanner = value; }
    bool isUsingBackgroundTooMuch();
    void addUsedBackground(int bgId);
    void incPurchasedCoins(int count) { m_iPurchasedCoins += count; }
    int getPurchasedCoins() { return m_iPurchasedCoins; }
    void incSeenVideoCount() { m_iSeenVideoCount++; }
    int getSeenVideoCount() { return m_iSeenVideoCount; }
    void markMadeAShare() { m_bMadeAShare = true; }
    bool isMadeAShare() { return m_bMadeAShare; }
    void markAchivmentUnlocked(int index) { m_achivments[index] = true; }
    bool isAchivmentUnlocked(int index) { return m_achivments[index]; }
    
    void updateFbUsers(char * data);
    FbUser * getRandomFbUser();
    
    void setUserId(const char * uid);
    const char * getUserId() { return m_userId; }
    
    void save();
    
    // ===========================================================
    // Sessions methods
    // ===========================================================
    
    void setVideoAdClicked(bool b) { m_bVideoAdClicked = b; }
    bool isVideoAdClicked() { return m_bVideoAdClicked; }
    
private:	

	// =========================================================== 
    // Fields
    // ===========================================================

	static Pref * s_pInstance;

	int m_iBestScore;
    int m_iGameCounts;
    int m_bSoundEnabled;
    bool m_bSoundsLoaded;
    bool m_bShowGuide;
    int m_iSelectedBird;
    bool m_birds[MAX_BIRDS];
    int m_iCoins;
    int m_iBackground;
    time_t m_tSettingsUpdate;
    Settings m_settings;
    time_t m_tInstallTime;
    bool m_bVideoAdClicked;
    char m_fbId[40];
    char m_userName[100];
    int m_iMultiplayerWins;
    time_t m_tLastChallangesLoadTime;
    bool m_bGalleryHintShown;
    bool m_clickedShareForCoinsBanner;
    std::list<int> m_lastUsedBackgrounds;
    int m_iPurchasedCoins;
    int m_iSeenVideoCount;
    bool m_bMadeAShare;
    bool m_achivments[50];
    std::vector<FbUser> m_fbUsers;
    char m_userId[100];
    
	// =========================================================== 
    // Initialization & cleanup
    // ===========================================================

	Pref();
	~Pref();

	// =========================================================== 
    // Private methods
    // ===========================================================

	std::string getFilePath();	
	bool restore();
};

#endif // __PREF_H__
