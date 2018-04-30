#include "Pref.h"
#include <stdio.h>
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
using namespace cocos2d;
using namespace std;

// ===========================================================
// Static memeber
// ===========================================================

Pref * Pref::s_pInstance = NULL;

// ===========================================================
// Singleton methods
// ===========================================================

Pref * Pref::getInstance()
{
	if (NULL == s_pInstance)
	{
		s_pInstance = new Pref();
	}
	return s_pInstance;
}

// =========================================================== 
// Initialization & cleanup
// ===========================================================

Pref::Pref()
{
	m_iBestScore = 0;
    m_iGameCounts = 0;
    m_bSoundEnabled = 1;
    m_bSoundsLoaded = false;
    m_bShowGuide = true;
    m_iSelectedBird = 0;
    memset(m_birds, 0, sizeof(m_birds));
    m_birds[0] = true;
    m_iCoins = 0;
    m_iBackground = BACKGROUND_MOUNTAIN;
    m_tSettingsUpdate = 0;
    m_tLastChallangesLoadTime = 0;
    m_bGalleryHintShown = false;
    m_clickedShareForCoinsBanner = false;
    memset(&m_settings, 0, sizeof(m_settings));
    m_settings.parse("50;1;2;3;4;550;20;6;10;0.3;50;50;50;100;100;100;100;150;150;150;150;200;200;200;200;250;250;250;250;5;50;50;50;100;100;100;100;250;250;150;150;200;300;300;200;200;250;250;300;350");
   
    m_tInstallTime = 0;
    memset(m_fbId, 0, sizeof(m_fbId));
    memset(m_userName, 0, sizeof(m_userName));   
    m_iMultiplayerWins = 0;
    m_iPurchasedCoins = 0;
    m_iSeenVideoCount = 0;
    m_bMadeAShare = false;
    memset(m_achivments, 0, sizeof(m_achivments));
    memset(m_userId, 0, sizeof(m_userId));
    if (!restore()) // first time
    {
        time(&m_tInstallTime);
        save();
    }
    
    m_bVideoAdClicked = false;
}

Pref::~Pref()
{
}

// =========================================================== 
// Public methods
// ===========================================================

void Pref::setBestScore(int score)
{
    m_iBestScore = score;
}

int Pref::getBestScore()
{
    return m_iBestScore;
}

bool Pref::isSoundEnabled()
{
    return m_bSoundEnabled == 1;
}

void Pref::enableSound(bool enable)
{
    m_bSoundEnabled = enable ? 1 : 0;
}

void Pref::preloadSounds()
{
    if (m_bSoundsLoaded)
    {
        return;
    }
    m_bSoundsLoaded = true;
    
    SimpleAudioEngine::getInstance()->preloadEffect("audio/coin.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("audio/e3.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("audio/ehit.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("audio/finish.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("audio/tap.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("audio/wing.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("audio/buy.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("audio/balloon.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("audio/boing.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("audio/strech.mp3");
}

bool Pref::isBirdUnlocked(int index)
{
    return m_birds[index];
}

void Pref::unlockBird(int index)
{
    m_birds[index] = true;
}

int Pref::getUnlockedBirdsCount()
{
    int count = 0;
    
    for (int i = 0; i < MAX_BIRDS; i++)
    {
        if (m_birds[i])
        {
            count++;
        }
    }
    
    return count;
}

void Pref::updateSettings(const char * str)
{
    Settings s;
    memset(&s, 0, sizeof(s));
    if (s.parse(str))
    {
        m_settings = s;
        save();
    }
}

int Pref::getDaysSinceInstall()
{
    time_t now;
    time(&now);
    return difftime(now, m_tInstallTime) / (60 * 60 * 24);
}

void Pref::setUserDetails(const char * fbId, const char * username)
{
    strcpy(m_fbId, fbId);
    strcpy(m_userName, username);
}

const char * Pref::getFbId()
{
    return m_fbId;
}

const char * Pref::getUserName()
{
    if ('\0' == m_userName[0])
    {
        return "GUEST";
    }
    return m_userName;
}

int Pref::getMultiplayerLevel()
{
    if (m_iMultiplayerWins < 1)
    {
        return 1;
    }
    else if (m_iMultiplayerWins < 2)
    {
        return 2;
    }
    else if (m_iMultiplayerWins < 22)
    {
        return 2 + (m_iMultiplayerWins - 1) / 3;
    }
    else
    {
        return 4 + (m_iMultiplayerWins / 5);
    }
}

bool Pref::isUsingBackgroundTooMuch()
{
    int lastUsed = -1;
    int useCount = 0;
    
    for (std::list<int>::iterator it = m_lastUsedBackgrounds.begin(); it != m_lastUsedBackgrounds.end(); ++it)
    {
        int curr = *it;
        if (curr == lastUsed)
        {
            useCount++;
        }
        else
        {
            useCount = 0;
            lastUsed = curr;
        }
    }
    
    CCLOG("bg use count: %d", useCount);
    
    return useCount > 5;
}

void Pref::addUsedBackground(int bgId)
{
    m_lastUsedBackgrounds.push_back(bgId);
    if (m_lastUsedBackgrounds.size() > 10)
    {
        m_lastUsedBackgrounds.pop_front();
    }
}

void Pref::updateFbUsers(char * data)
{
    m_fbUsers.clear();
    
    char * token = strtok (data, ";");
    while (token != NULL)
    {
        std::string idStr = std::string(token);
        
        token = strtok(NULL, ";");
        
        std::string nameStr = std::string(token);
        
        FbUser u;
        //u.i
        
        token = strtok(NULL, ";");
    }
}

FbUser * Pref::getRandomFbUser()
{
    return NULL;
}

void Pref::setUserId(const char * uid)
{
    memset(m_userId, 0, sizeof(m_userId));
    strcpy(m_userId, uid);
}

void Pref::save()
{
	string path = getFilePath();
	FILE *fp = fopen(path.c_str(), "w");

	if (!fp)
	{
		CCLOG("can not create file %s", path.c_str());
		return;
	}
	
	fwrite(&m_iBestScore, sizeof(int), 1, fp);
    fwrite(&m_iGameCounts, sizeof(m_iGameCounts), 1, fp);
    fwrite(&m_bSoundEnabled, sizeof(m_bSoundEnabled), 1, fp);
    fwrite(&m_bShowGuide, sizeof(m_bShowGuide), 1, fp);
    fwrite(&m_iSelectedBird, sizeof(m_iSelectedBird), 1, fp);
    fwrite(&m_birds, sizeof(m_birds), 1, fp);
    fwrite(&m_iCoins, sizeof(m_iCoins), 1, fp);
    fwrite(&m_iBackground, sizeof(m_iBackground), 1, fp);
    fwrite(&m_tInstallTime, sizeof(m_tInstallTime), 1, fp);
    fwrite(&m_tSettingsUpdate, sizeof(m_tSettingsUpdate), 1, fp);
    fwrite(&m_settings, sizeof(m_settings), 1, fp);
    fwrite(&m_fbId, sizeof(m_fbId), 1, fp);
    fwrite(&m_userName, sizeof(m_userName), 1, fp);
    fwrite(&m_iMultiplayerWins, sizeof(m_iMultiplayerWins), 1, fp);
    fwrite(&m_bGalleryHintShown, sizeof(m_bGalleryHintShown), 1, fp);
    fwrite(&m_clickedShareForCoinsBanner, sizeof(m_clickedShareForCoinsBanner), 1, fp);
    fwrite(&m_iPurchasedCoins, sizeof(m_iPurchasedCoins), 1, fp);
    fwrite(&m_iSeenVideoCount, sizeof(m_iSeenVideoCount), 1, fp);
    fwrite(&m_bMadeAShare, sizeof(m_bMadeAShare), 1, fp);
    fwrite(&m_achivments, sizeof(m_achivments), 1, fp);
    
    fwrite(&m_userId, sizeof(m_userId), 1, fp);
    
    fclose(fp);
}

// =========================================================== 
// Private methods
// ===========================================================

string Pref::getFilePath()
{
	string path("");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	// In android, every programe has a director under /data/data.
	// The path is /data/data/ + start activity package name.
	// You can save application specific data here.
	path.append("/data/data/com.flambo.tapfly/pref.ini");
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	// You can save file in anywhere if you have the permision.
	path.append("pref.ini");
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	// You can save file in anywhere if you have the permision.
    path.append(CCFileUtils::sharedFileUtils()->getWritablePath());
	path.append("pref.ini");
#endif

	return path;
}

bool Pref::restore()
{
	string path = getFilePath();   
	FILE *fp = fopen(path.c_str(), "r");
	       
	if (!fp)
	{
		CCLOG("can not open file %s", path.c_str());
		return false;
	}
	
	fread(&m_iBestScore, sizeof(int), 1, fp);
    fread(&m_iGameCounts, sizeof(m_iGameCounts), 1, fp);
    fread(&m_bSoundEnabled, sizeof(m_bSoundEnabled), 1, fp);
    fread(&m_bShowGuide, sizeof(m_bShowGuide), 1, fp);
    fread(&m_iSelectedBird, sizeof(m_iSelectedBird), 1, fp);
    fread(&m_birds, sizeof(m_birds), 1, fp);
    fread(&m_iCoins, sizeof(m_iCoins), 1, fp);
    fread(&m_iBackground, sizeof(m_iBackground), 1, fp);
    fread(&m_tInstallTime, sizeof(m_tInstallTime), 1, fp);
    fread(&m_tSettingsUpdate, sizeof(m_tSettingsUpdate), 1, fp);
    fread(&m_settings, sizeof(m_settings), 1, fp);
    fread(&m_fbId, sizeof(m_fbId), 1, fp);
    fread(&m_userName, sizeof(m_userName), 1, fp);
    fread(&m_iMultiplayerWins, sizeof(m_iMultiplayerWins), 1, fp);
    fread(&m_bGalleryHintShown, sizeof(m_bGalleryHintShown), 1, fp);
    fread(&m_clickedShareForCoinsBanner, sizeof(m_clickedShareForCoinsBanner), 1, fp);
    fread(&m_iPurchasedCoins, sizeof(m_iPurchasedCoins), 1, fp);
    fread(&m_iSeenVideoCount, sizeof(m_iSeenVideoCount), 1, fp);
    fread(&m_bMadeAShare, sizeof(m_bMadeAShare), 1, fp);
    fread(&m_achivments, sizeof(m_achivments), 1, fp);
    
    fread(&m_userId, sizeof(m_userId), 1, fp);
    
	fclose(fp);
    
    return true;
}
