//
//  GameRecorder.cpp
//  tapfly
//
//  Created by Kfir Goldman on 3/11/15.
//
//

#include "GameRecorder.h"
#include "GameRecording.h"
#include <cocos2d.h>
#include "Pref.h"

// ===========================================================
// Initialization & cleanup
// ===========================================================

GameRecorder::GameRecorder(unsigned int seed, int birdType)
{
    m_iSeed = seed;
    m_iBirdType = birdType;
    m_iScore = 0;
    m_iLength = 0;
    m_iSize = 0;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // In android, every programe has a director under /data/data.
    // The path is /data/data/ + start activity package name.
    // You can save application specific data here.
    m_fileName.append("/data/data/com.flambo.tapfly/GameRecorder.dat");
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // You can save file in anywhere if you have the permision.
    m_fileName.append(cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath());
    m_fileName.append("GameRecorder.dat");
#endif
    
    m_pFile = fopen(m_fileName.c_str(), "w");
}

GameRecorder::~GameRecorder()
{
}

// ===========================================================
// Public methods
// ===========================================================

void GameRecorder::push(MultiplayerMessage * pMessage, unsigned int size)
{
    fwrite(pMessage, 1, size, m_pFile);
    m_iSize += size;
    m_iLength = pMessage->t;
    
    if (MSG_TYPE_BOUNS == pMessage->msgType)
    {
        MutliplayerBouns * msg = (MutliplayerBouns *)pMessage;
        if (BOUNS_STATE_COLLECT == msg->state)
        {
            int coins = 0;
            switch (msg->bounsType)
            {
                case TAG_BOUNS : coins = Pref::getInstance()->getSettings().getCoinValue(0); break;
                case TAG_BOUNS_2 : coins = Pref::getInstance()->getSettings().getCoinValue(1); break;
                case TAG_BOUNS_3 : coins = Pref::getInstance()->getSettings().getCoinValue(2); break;
                case TAG_BOUNS_4 : coins = Pref::getInstance()->getSettings().getCoinValue(3); break;
            }
            
            m_iScore += coins;
        }
    }
}

GameRecording * GameRecorder::getData()
{
    GameRecording * pRetVal = new GameRecording(m_iSeed, m_iBirdType);
    pRetVal->setLength(m_iLength);
    pRetVal->setScore(m_iScore);
    
    fclose(m_pFile);
    
    m_pFile = fopen(m_fileName.c_str(), "r");
    unsigned char * data = new unsigned char[m_iSize];
    fread(data, sizeof(unsigned char), m_iSize, m_pFile);
    fclose(m_pFile);
    
    pRetVal->setData(data, m_iSize);
    
    return pRetVal;
}