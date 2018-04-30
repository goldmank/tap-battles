//
//  Challenge.cpp
//  tapfly
//
//  Created by Kfir Goldman on 3/14/15.
//
//

#include "Challenge.h"

// ===========================================================
// Initialization & cleanup
// ===========================================================

Challenge::Challenge(std::string gameId)
{
    m_gameId = gameId;
}

Challenge::~Challenge()
{    
}

// ===========================================================
// Public methods
// ===========================================================

Opponent * Challenge::getOpponent(int index)
{
    return &m_opponents[index];
}

int Challenge::getOpponentCount()
{
    return (int)m_opponents.size();
}

void Challenge::addOpponent(Opponent o)
{
    m_opponents.push_back(o);
}