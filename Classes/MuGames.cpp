#include "MuGames.h"
#include "../cocos2d/external/json/document.h"
#include "../cocos2d/external/json/writer.h"
#include "../cocos2d/external/json/stringbuffer.h"
#include "MD5.h"

using namespace cocos2d;
using namespace network;

// ===========================================================
// Constants
// ===========================================================

#define API_VERSION 2
#define SERVICE     "DodgyWings"
#define APP_ID      5

// ===========================================================
// Static members
// ===========================================================

MuGames * MuGames::s_pInstance = NULL;

// ===========================================================
// Singleton methods
// ===========================================================

MuGames * MuGames::getInstance()
{
    if (NULL == s_pInstance)
    {
        s_pInstance = new MuGames();
        s_pInstance->autorelease();
        s_pInstance->retain();
    }
    return s_pInstance;
}

// ===========================================================
// Public methods
// ===========================================================

void MuGames::init(const char * versionName, const char * deviceId, const char * os, const char * device)
{
    CCLOG("initMuGames %s, %s, %s, %s", versionName, deviceId, os, device);
    m_versionName = versionName;
    m_deviceId = deviceId;
    m_deviceType = device;
    m_os = os;
    if (m_os.empty())
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        m_os = "Android";
#endif
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        m_os = "iOS";
#endif
    }
    if (m_bDoHandshakeAfterInit)
    {
        m_bDoHandshakeAfterInit = false;
        login();
    }
}

void MuGames::login()
{
    if ('\0' != m_deviceId[0]) // has device id
    {
        callHandsake();
    }
    else
    {
        CCLOG("MuGames::login delayed after init");
        m_bDoHandshakeAfterInit = true;
    }
}

void MuGames::track(const char *eventName, const char * eventData, int tryCount)
{
    if (!m_bLoggedIn)
    {
        addPendingStat(eventName, eventData);
        return;
    }
    
    std::string url = "https://event.mugames.com/api.php?r=Event/1.1/a/reportEvent&token=" + m_defToken + "&tokenData=" + m_defTokenData;
    CCLOG("track [try=%d]: %s", tryCount, eventName);
    CCLOG("url: %s", url.c_str());
    
    rapidjson::Document params;
    params.SetObject();
    params.AddMember("event", eventName, params.GetAllocator());
    
    std::vector<const char *> toRelease;
    
    if (NULL != eventData && '\0' != eventData[0])
    {
        rapidjson::Document dataJSON;
        if (!dataJSON.Parse<0>(eventData).HasParseError())
        {
            rapidjson::Value data(rapidjson::kObjectType);
            
            for (rapidjson::Value::ConstMemberIterator itr = dataJSON.MemberonBegin(); itr != dataJSON.MemberonEnd(); ++itr)
            {
                char * name = new char[100];
                memset(name, 0, sizeof(char) * 100);
                strcpy(name, itr->name.GetString());
                toRelease.push_back(name);
                
                if (itr->value.IsInt())
                {
                    data.AddMember(name, itr->value.GetInt(), params.GetAllocator());
                }
                else if (itr->value.IsString())
                {
                    char * value = new char[100];
                    memset(value, 0, sizeof(char) * 100);
                    strcpy(value, itr->value.GetString());
                    toRelease.push_back(value);
                    
                    data.AddMember(name, value, params.GetAllocator());
                }
                else if (itr->value.IsBool())
                {
                    data.AddMember(name, itr->value.GetBool(), params.GetAllocator());
                }
                else if (itr->value.IsDouble())
                {
                    data.AddMember(name, itr->value.GetDouble(), params.GetAllocator());
                }
            }
            
            params.AddMember("data", data, params.GetAllocator());
        }
    }
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    params.Accept(writer);
    const char * paramsString = buffer.GetString();
    CCLOG("params: %s", paramsString);
    
    int paramsLen = (int)strlen(paramsString);
    char * data = new char[6 + paramsLen];
    sprintf(data, "data=%s", paramsString);
    
    for (int i = 0; i < toRelease.size(); i++)
    {
        const char * str = toRelease[i];
        delete str;
    }
    toRelease.clear();

    HttpRequest* request = new HttpRequest();
    request->setUrl(url.c_str());
    request->setRequestType(HttpRequest::Type::POST);
    
    std::vector<std::string> headers;
    addHeaders(headers, "1.1");
    request->setHeaders(headers);
    
    char tryCountStr[5];
    sprintf(tryCountStr, "%d", tryCount);
    request->setTag(tryCountStr);
    
    StatEvent * pEvent = new StatEvent();
    memset(pEvent, 0, sizeof(StatEvent));
    strcpy(pEvent->m_name, eventName);
    if (NULL != eventData)
    {
        strcpy(pEvent->m_data, eventData);
    }
    request->setUserData(pEvent);
    
    request->setRequestData(data, strlen(data));
    request->setResponseCallback(this, httpresponse_selector(MuGames::handleEvent));
    HttpClient::getInstance()->send(request);
    request->release();
    
    delete data;
}

void MuGames::flush()
{
    if (!m_pendingStats.empty())
    {
        StatEvent event = m_pendingStats.front();
        m_pendingStats.pop_front();
        savePendingStats();
        if (event.m_data[0] != 0) // has data
        {
            track(event.m_name, event.m_data, 3);
        }
        else
        {
            track(event.m_name, NULL, 3);
        }
    }
}

// ===========================================================
// Constructor
// ===========================================================

MuGames::MuGames()
{
    srand(NULL);
    
    genDefaultToken();
    
    m_iLoginTry = 0;
    m_bDoHandshakeAfterInit = false;
    
    m_guestLoginURL = "https://game.mugames.com/api.php?r=DodgyWings/1.0/a/guestLogin";
        
    loadPendingStats();
}

// ===========================================================
// Private methods
// ===========================================================

void MuGames::callHandsake()
{
    std::string url = "https://game.mugames.com/handshake.php?token=" + m_defToken + "&tokenData=" + m_defTokenData;
    CCLOG("callHandsake: %s", url.c_str());
    
    rapidjson::Document params;
    params.SetObject();
    params.AddMember("type", API_VERSION, params.GetAllocator());
    params.AddMember("service", SERVICE, params.GetAllocator());
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    params.Accept(writer);
    const char * paramsString = buffer.GetString();
    CCLOG("params: %s", paramsString);
    
    int paramsLen = (int)strlen(paramsString);
    char * data = new char[6 + paramsLen];
    sprintf(data, "data=%s", paramsString);
    
    HttpRequest* request = new HttpRequest();
    request->setUrl(url.c_str());
    request->setRequestType(HttpRequest::Type::POST);

    std::vector<std::string> headers;
    addHeaders(headers);
    request->setHeaders(headers);

    request->setRequestData(data, strlen(data));
    request->setResponseCallback(this, httpresponse_selector(MuGames::handleHandsake));
    HttpClient::getInstance()->send(request);
    request->release();

    delete data;
}

void MuGames::callLogin()
{
    char sid[50];
    sprintf(sid, "%lu", time(0));
    m_sid = sid;
    
    std::string url = m_guestLoginURL + "&token=" + m_defToken + "&tokenData=" + m_defTokenData;
    CCLOG("callLogin: %s", url.c_str());
    
    HttpRequest* request = new HttpRequest();
    request->setUrl(url.c_str());
    request->setRequestType(HttpRequest::Type::POST);
    
    std::vector<std::string> headers;
    addHeaders(headers);
    request->setHeaders(headers);
    
    request->setResponseCallback(this, httpresponse_selector(MuGames::handleLogin));
    HttpClient::getInstance()->send(request);
    request->release();
}

void MuGames::handleHandsake(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
	CCLOG("handleHandsake: %d", (int)response->getResponseCode());
    if (200 == response->getResponseCode())
    {
        std::string data(response->getResponseData()->begin(), response->getResponseData()->end());
        const char * json = data.c_str();
        CCLOG("json:%s",json);
        rapidjson::Document document;
        if (!document.Parse<0>(json).HasParseError())
        {
            try {
                const rapidjson::Value & response = document["response"];
                if (!response.IsNull())
                {
                    const rapidjson::Value & serviceConfig = response["serviceConfig"];
                    if (!serviceConfig.IsNull())
                    {
                        const rapidjson::Value & services = serviceConfig["services"];
                        if (!services.IsNull())
                        {
                            const rapidjson::Value & guestLogin = services["guestLogin"];
                            if (!guestLogin.IsNull())
                            {
                                const char * url = guestLogin["url"].GetString();
                                CCLOG("guestLogin: %s", url);
                                
                                m_guestLoginURL = url;
                            }
                        }
                    }
                }
            } catch (std::exception e) {}
        }
    }
    else
    {
        CCLOG("handsake failed: %d", (int)response->getResponseCode());
    }
    
    m_iLoginTry = 0;
    callLogin();
}

void MuGames::handleLogin(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    try {
        if (200 == response->getResponseCode())
        {
            std::string data(response->getResponseData()->begin(), response->getResponseData()->end());
            const char * json = data.c_str();
            CCLOG("login-response: %s", json);
            rapidjson::Document document;
            if (!document.Parse<0>(json).HasParseError())
            {
                const rapidjson::Value & response = document["response"];
                if (!response.IsNull())
                {
                    const rapidjson::Value & data = response["data"];
                    if (!data.IsNull())
                    {
                        const char * token = data["token"].GetString();
                        const char * tokenData = data["tokenData"].GetString();
                        m_token = token;
                        m_tokenData = tokenData;
                        
                        char uuid[50];
                        memset(uuid, 0, sizeof(uuid));
                        int tokenDataLen = (int)strlen(tokenData);
                        int startIndex = -1;
                        for (int i = 0; i < tokenDataLen && -1 == startIndex; i++)
                        {
                            if ('_' == tokenData[i])
                            {
                                startIndex = i + 1;
                            }
                        }
                        if (startIndex > 0)
                        {
                            for (int i = startIndex; i < tokenDataLen; i++)
                            {
                                uuid[i - startIndex] = tokenData[i];
                            }
                            m_uuid = uuid;
                        }
                        
                        CCLOG("logged-in");
                        m_bLoggedIn = true;
                        flush();
                    }
                }
            }
        }
        else
        {
            CCLOG("login failed: %d", (int)response->getResponseCode());
            if (m_iLoginTry++ < 3)
            {
                CCLOG("rety login #%d", m_iLoginTry);
                callLogin();
            }
        }
    } catch (std::exception e) {}
}

void MuGames::handleEvent(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    bool isSent = false;
    
    StatEvent * pEvent = (StatEvent *)response->getHttpRequest()->getUserData();
    response->getHttpRequest()->setUserData(NULL);
    
    if (200 == response->getResponseCode())
    {
        std::string data(response->getResponseData()->begin(), response->getResponseData()->end());
        const char * json = data.c_str();
        rapidjson::Document document;
        if (!document.Parse<0>(json).HasParseError())
        {
            CCLOG("%s", data.c_str());
            CCLOG("event sent");
            if (!document["success"].IsNull())
            {
                isSent = document["success"].GetBool();
            }
            else
            {
                if (!document["code"].IsNull())
                {
                    int code = document["code"].GetInt();
                    if (201 == code)
                    {
                        CCLOG("Login session ended - restart session");
                        addPendingStat(pEvent->m_name, pEvent->m_data);
                        m_iLoginTry = 0;
                        callLogin();
                        return;
                    }
                }
            }
        }
    }
    else
    {
        CCLOG("event failed: %d", (int)response->getResponseCode());
    }
    
    if (!isSent)
    {
        const char * tryCount = response->getHttpRequest()->getTag();
        if (NULL != tryCount)
        {
            int tryCountInt = atoi(tryCount);
            if (tryCountInt > 0)
            {
                if (pEvent->m_data[0] != 0) // has data
                {
                    track(pEvent->m_name, pEvent->m_data, tryCountInt - 1);
                }
                else
                {
                    track(pEvent->m_name, NULL, tryCountInt - 1);
                }
            }
            else
            {
                addPendingStat(pEvent->m_name, pEvent->m_data);
            }
        }
    }
    else
    {
        flush(); // send next pending
    }
    
    delete pEvent;
}

void MuGames::genDefaultToken()
{
    m_token = "Qw12Rte321";
    m_tokenData = "123abc";
    
    const char * tokenPostfix = "Qw12Rte321";
    const int tokenDataSize = 10;
    const int tokenDataLeftPadding = 26;
    const int tokenDataRightPadding = 65;

    char builder[tokenDataSize + 1];
    memset(builder, 0, sizeof(builder));
    for (int i = 0; i < tokenDataSize; i++)
    {
        float rnd = (rand() % 1000) / 1000.0f;
        rnd = tokenDataLeftPadding*rnd + tokenDataRightPadding;
        char ch = (char)((int)floorf(rnd));
        builder[i] = ch;
    }
    m_tokenData = builder;
   
    std::string toHash = m_tokenData + tokenPostfix;
    MD5 md5 = MD5(toHash);
    m_token = md5.hexdigest();
    
    CCLOG("token: %s", m_token.c_str());
    CCLOG("tokenData: %s", m_tokenData.c_str());
    
    m_defToken = m_token;
    m_defTokenData = m_tokenData;
}

void MuGames::loadPendingStats()
{
    std::string path = getDBFilePath();
    FILE *fp = fopen(path.c_str(), "r");
    
    if (!fp)
    {
        CCLOG("can not open file %s", path.c_str());
        return;
    }
    
    StatEvent event;
    int readSize = (int)fread(&event, sizeof(StatEvent), 1, fp);
    while (readSize > 0)
    {
        m_pendingStats.push_back(event);
        readSize = (int)fread(&event, sizeof(StatEvent), 1, fp);
    }
    
    fclose(fp);
}

void MuGames::savePendingStats()
{
    std::string path = getDBFilePath();
    FILE *fp = fopen(path.c_str(), "w");
    
    if (!fp)
    {
        CCLOG("can not open file %s", path.c_str());
        return;
    }
    
    for (std::list<StatEvent>::iterator it = m_pendingStats.begin(); it != m_pendingStats.end(); ++it)
    {
        StatEvent event = *it;
        fwrite(&event, sizeof(StatEvent), 1, fp);
    }
    
    fclose(fp);
}

std::string MuGames::getDBFilePath()
{
    std::string path("");
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // In android, every programe has a director under /data/data.
    // The path is /data/data/ + start activity package name.
    // You can save application specific data here.
    path.append("/data/data/com.flambo.tapfly/mu.db");
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    // You can save file in anywhere if you have the permision.
    path.append("mu.db");
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // You can save file in anywhere if you have the permision.
    path.append(CCFileUtils::sharedFileUtils()->getWritablePath());
    path.append("mu.db");
#endif
    
    return path;
}

void MuGames::addPendingStat(const char * eventName, const char * eventData)
{
    StatEvent event;
    memset(&event, 0, sizeof(event));
    strcpy(event.m_name, eventName);
    if (NULL != eventData)
    {
        strcpy(event.m_data, eventData);
    }
    m_pendingStats.push_back(event);
    savePendingStats();
}

void MuGames::addHeaders(std::vector<std::string> & headers, const char * apiVersion)
{
    char data[200];
    
    std::time_t current_time;
    std::time(&current_time);
    struct std::tm *timeinfo = std::localtime(&current_time);
    sprintf(data, "MU-TZ:%d", (int)timeinfo->tm_gmtoff);
    headers.push_back(data);
    
    memset(data, 0, sizeof(data));
    sprintf(data, "MU-AppID: %d", APP_ID);
    headers.push_back(data);
    
    memset(data, 0, sizeof(data));
    sprintf(data, "MU-Api-Version: %s", apiVersion);
    headers.push_back(data);
    
    if (m_uuid.length() > 0)
    {
        memset(data, 0, sizeof(data));
        sprintf(data, "MU-UUID: %s", m_uuid.c_str());
        headers.push_back(data);
    }
    
    if (m_sid.length() > 0)
    {
        memset(data, 0, sizeof(data));
        sprintf(data, "MU-SID: %s", m_sid.c_str());
        headers.push_back(data);
    }
    
    memset(data, 0, sizeof(data));
    sprintf(data, "MU-Version: %s", m_versionName.c_str());
    headers.push_back(data);
    
    memset(data, 0, sizeof(data));
    sprintf(data, "MU-DID: %s", m_deviceId.c_str());
    headers.push_back(data);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    headers.push_back("MU-Platform: 2");
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    headers.push_back("MU-Platform: 1");
#endif
    
    if (m_deviceType.length() > 0)
    {
        memset(data, 0, sizeof(data));
        sprintf(data, "MU-Device-Type: %s", m_deviceType.c_str());
        headers.push_back(data);
    }
    
    memset(data, 0, sizeof(data));
    sprintf(data, "MU-OS: %s", m_os.c_str());
    headers.push_back(data);
    
    for (int i = 0; i < headers.size(); i++)
    {
        CCLOG("header: %s", headers[i].c_str());
    }
}