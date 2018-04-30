#ifndef __MU_GAMES__
#define __MU_GAMES__

#include "cocos2d.h"
#include "../cocos2d/cocos/network/HttpClient.h"

class MuGames : public cocos2d::Node
{
public:
    
    // ===========================================================
    // Singleton methods
    // ===========================================================
    
    static MuGames * getInstance();
    
    // ===========================================================
    // Public methods
    // ===========================================================
    
    void init(const char * versionName, const char * deviceId, const char * os, const char * device);
    void login();
    void track(const char *eventName, const char * data = NULL, int tryCount = 3);
    void flush();
    
protected:
    
    // ===========================================================
    // Constructor
    // ===========================================================
    
    MuGames();
    
private:
    
    // ===========================================================
    // StatEvent
    // ===========================================================
    
    struct StatEvent
    {
        char m_name[40];
        char m_data[100];
    };
    
    // ===========================================================
    // Static members
    // ===========================================================
    
    static MuGames * s_pInstance;
    
    // ===========================================================
    // Fields
    // ===========================================================
    
    std::string m_versionName;
    std::string m_deviceId;
    std::string m_deviceType;
    std::string m_os;
    std::string m_defToken;
    std::string m_defTokenData;
    std::string m_token;
    std::string m_tokenData;
    std::string m_uuid;
    std::string m_sid;
    std::string m_guestLoginURL;
    bool m_bLoggedIn;
    FILE * m_pPendingStatsDB;
    std::list<StatEvent> m_pendingStats;
    int m_iLoginTry;
    bool m_bDoHandshakeAfterInit;
    
    // ===========================================================
    // Private methods
    // ===========================================================
    
    void callHandsake();
    void callLogin();
    void handleHandsake(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void handleLogin(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void handleEvent(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void genDefaultToken();
    
    void loadPendingStats();
    void savePendingStats();
    std::string getDBFilePath();
    void addPendingStat(const char * eventName, const char * eventData);
    void addHeaders(std::vector<std::string> & headers, const char * apiVersion = "1.0");
};

#endif // __MU_GAMES__
