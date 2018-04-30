//
//  Opponent.cpp
//  tapfly
//
//  Created by Kfir Goldman on 3/21/15.
//
//

#include "Opponent.h"

// ===========================================================
// Initialization & cleanup
// ===========================================================

Opponent::Opponent(std::string cId, std::string opponentName, std::string opponentImageURL, int level)
{
    m_uId = cId;
    m_name = opponentName;
    m_imageURL = opponentImageURL;
    m_iLevel = level;
}

Opponent::~Opponent()
{
}