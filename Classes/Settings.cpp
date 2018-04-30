//
//  Settings.cpp
//  tapfly
//
//  Created by Kfir Goldman on 3/3/15.
//
//

#include "Settings.h"

// ===========================================================
// Initialization & cleanup
// ===========================================================

Settings::Settings()
{
    m_iRecordChance = 0;
}

Settings::~Settings()
{    
}

// ===========================================================
// Public methods
// ===========================================================

bool Settings::parse(const char * str)
{
    std::vector<std::string> lines = splitLines(str);
    if (lines.size() >= 29)
    {
        m_iReward = atoi(lines[0].c_str());
                
        for (int i = 0; i < 4; i++)
        {
            m_iCoins[i] = atoi(lines[1 + i].c_str());
        }
        
        m_iFlySpeed = atoi(lines[5].c_str());
        m_iGravity = atoi(lines[6].c_str());
        m_iEnemySpeed = atoi(lines[7].c_str());
        m_iEnemySpeedChangeInterval = atoi(lines[8].c_str());
        m_fEnemySpeedChange = atof(lines[9].c_str());
        
        for (int i = 0; i < 19; i++)
        {
            std::string birdPrice = lines[10 + i];
            m_birdPrice[i] = atoi(birdPrice.c_str());
        }
        
        if (lines.size() >= 30)
        {
            m_iRecordChance = atoi(lines[29].c_str());
        }
        
        if (lines.size() > 30)
        {
            for (int i = 0; i < 19; i++)
            {
                std::string birdPrice = lines[30 + i];
                m_birdPrice[19 + i] = atoi(birdPrice.c_str());
            }
        }
        
        return true;
    }
    return false;
}

int Settings::getBirdPrice(int index)
{
    if (0 == m_birdPrice[index])
    {
        return 200;
    }
    return m_birdPrice[index];
}

// ===========================================================
// Private methods
// ===========================================================

std::vector<std::string> Settings::splitLines(const char * str)
{
    std::vector<std::string> retVal;
    
    int bufIndex = 0;
    char buffer[100];
    memset(buffer, 0, sizeof(buffer));
    
    int len = (int)strlen(str);
    for (int i = 0; i < len; i++)
    {
        if (';' == str[i])
        {
            retVal.push_back(buffer);
            bufIndex = 0;
            memset(buffer, 0, sizeof(buffer));
        }
        else
        {
            buffer[bufIndex++] = str[i];
        }
    }
    
    if (bufIndex > 0)
    {
        retVal.push_back(buffer);
    }
    
    return retVal;
}
