//
//  GameRecording.cpp
//  tapfly
//
//  Created by Kfir Goldman on 3/11/15.
//
//

#include "GameRecording.h"
#include <cocos2d.h>
#include "Pref.h"
#include "GameRecorder.h"

// ===========================================================
// Initialization & cleanup
// ===========================================================

GameRecording::GameRecording(unsigned int seed, int birdType)
{
    m_iSeed = seed;
    m_iBirdType = birdType;
    m_iScore = 0;
    m_iLength = 0;
    m_iSize = 0;
    m_pData = NULL;
    m_iReaderPos = 0;
    memset(m_userId, 0, sizeof(m_userId));
    memset(m_userName, 0, sizeof(m_userName));
    memset(m_fbId, 0, sizeof(m_fbId));
}

GameRecording::~GameRecording()
{
    if (NULL != m_pData)
    {
        delete[] m_pData;
        m_pData = NULL;
    }
}

// ===========================================================
// Public methods
// ===========================================================

void GameRecording::setUserId(const char * uid)
{
    strcpy(m_userId, uid);
}

void GameRecording::setUsername(const char * username)
{
    if (NULL != username)
    {
        strcpy(m_userName, username);
    }
}

void GameRecording::setFbId(const char * fbid)
{
    if (NULL != fbid)
    {
        strcpy(m_fbId, fbid);
    }
}

const char * GameRecording::getUsername()
{
    if ('\0' == m_userName[0])
    {
        return "GUEST";
    }
    return m_userName;
}

const unsigned char * GameRecording::getData(int * pSize)
{
    *pSize = m_iSize;
    return m_pData;
}

float GameRecording::getNextMessageTime()
{
    int index = m_iReaderPos;
    MultiplayerMessage * pMsg = (MultiplayerMessage *)&m_pData[index];
    switch (pMsg->msgType)
    {
        case MSG_TYPE_BIRD : index += sizeof(MutliplayerPos); break;
        case MSG_TYPE_BOUNS : index += sizeof(MutliplayerBouns); break;
        case MSG_TYPE_PUSHER : index += sizeof(MutliplayerPos); break;
    }
    if (index >= m_iSize)
    {
        return MAXFLOAT;
    }
    
    pMsg = (MultiplayerMessage *)&m_pData[index];
    return pMsg->t;
}

MultiplayerMessage * GameRecording::getNextMessage()
{
    if (m_iReaderPos >= m_iSize)
    {
        return NULL;
    }
    
    MultiplayerMessage * pRetVal = (MultiplayerMessage *)&m_pData[m_iReaderPos];
    switch (pRetVal->msgType)
    {
        case MSG_TYPE_BIRD : m_iReaderPos += sizeof(MutliplayerPos); break;
        case MSG_TYPE_BOUNS : m_iReaderPos += sizeof(MutliplayerBouns); break;
        case MSG_TYPE_PUSHER : m_iReaderPos += sizeof(MutliplayerPos); break;
    }
    return pRetVal;
}

std::string GameRecording::toString()
{
    char retVal[50];
    sprintf(retVal, "%d;%d;%d;%.2f", m_iSeed, m_iBirdType, m_iScore, m_iLength);
    return retVal;
}