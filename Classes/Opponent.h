//
//  Opponent.h
//  tapfly
//
//  Created by Kfir Goldman on 3/21/15.
//
//

#ifndef __tapfly__Opponent__
#define __tapfly__Opponent__

#include <stdio.h>

#include <stdio.h>
#include <string>

// ===========================================================
// GameRecording
// ===========================================================

class Opponent
{
public:
    
    // ===========================================================
    // Initialization & cleanup
    // ===========================================================
    
    Opponent(std::string uId, std::string opponentName, std::string opponentImageURL, int level);
    ~Opponent();
    
    // ===========================================================
    // Public methods
    // ===========================================================
    
    std::string getUserId() { return m_uId; }
    std::string getOpponentName() { return m_name; }
    std::string getOpponentImageURL() { return m_imageURL; }
    int getLevel() { return m_iLevel; }
    
private:
    
    // ===========================================================
    // Fields
    // ===========================================================
    
    std::string m_uId;
    std::string m_name;
    std::string m_imageURL;
    int m_iLevel;
};


#endif /* defined(__tapfly__Opponent__) */
