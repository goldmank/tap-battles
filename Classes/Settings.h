//
//  Settings.h
//  tapfly
//
//  Created by Kfir Goldman on 3/3/15.
//
//

#ifndef __tapfly__Settings__
#define __tapfly__Settings__

#include "cocos2d.h"

class Settings
{
public:
    
    // ===========================================================
    // Initialization & cleanup
    // ===========================================================
    
    Settings();
    ~Settings();
    
    // ===========================================================
    // Public methods
    // ===========================================================
    
    bool parse(const char * str);
    
    int getRewardCoins() { return m_iReward; }
    int getCoinValue(int index) { return m_iCoins[index]; }
    int getBirdPrice(int index);
    int getFlySpeed() { return m_iFlySpeed; }
    int getGravity() { return m_iGravity; }
    int getEnemySpeed() { return m_iEnemySpeed; }
    int getEnemySpeedChangeInterval() { return m_iEnemySpeedChangeInterval; }
    float getEnemySpeedChange() { return m_fEnemySpeedChange; }
    bool shouldRecordGame() { return m_iRecordChance > 0 && rand() % m_iRecordChance == 0; }
    
private:
    
    // ===========================================================
    // Fields
    // ===========================================================
    
    int m_iReward;
    int m_iCoins[4];
    int m_birdPrice[100];
    int m_iFlySpeed;
    int m_iGravity;
    int m_iEnemySpeed;
    int m_iEnemySpeedChangeInterval;
    float m_fEnemySpeedChange;
    int m_iRecordChance;
    
    // ===========================================================
    // Private methods
    // ===========================================================
    
    std::vector<std::string> splitLines(const char * str);
};

#endif /* defined(__tapfly__Settings__) */
