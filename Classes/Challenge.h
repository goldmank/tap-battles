//
//  Challenge.h
//  tapfly
//
//  Created by Kfir Goldman on 3/14/15.
//
//

#ifndef __tapfly__Challenge__
#define __tapfly__Challenge__

#include <stdio.h>
#include <string>
#include <vector>
#include "Opponent.h"

// ===========================================================
// GameRecording
// ===========================================================

class Challenge
{
public:
    
    // ===========================================================
    // Initialization & cleanup
    // ===========================================================
    
    Challenge(std::string gameId);
    ~Challenge();
    
    // ===========================================================
    // Public methods
    // ===========================================================
    
    std::string getGameId() { return m_gameId; }
    Opponent * getOpponent(int index);
    int getOpponentCount();
    void addOpponent(Opponent o);
    
private:
    
    // ===========================================================
    // Fields
    // ===========================================================
    
    std::string m_gameId;
    std::vector<Opponent> m_opponents;
};

#endif /* defined(__tapfly__Challenge__) */
