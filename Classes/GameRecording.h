//
//  GameRecording.h
//  tapfly
//
//  Created by Kfir Goldman on 3/11/15.
//
//

#ifndef __tapfly__GameRecording__
#define __tapfly__GameRecording__

#include <stdio.h>
#include <string>
#include <stdlib.h>

class MultiplayerMessage;

// ===========================================================
// GameRecording
// ===========================================================

class GameRecording
{
public:
    
    // ===========================================================
    // Initialization & cleanup
    // ===========================================================
    
    GameRecording(unsigned int seed, int birdType);
    ~GameRecording();
    
    // ===========================================================
    // Public methods
    // ===========================================================
    
    void setScore(int score) { m_iScore = score; }
    void setLength(int length) { m_iLength = length; }
    void setUserId(const char * uid);
    void setUsername(const char * username);
    void setFbId(const char * fbId);
    void setData(const unsigned char * data, unsigned int length) { m_pData = data; m_iSize = length; }
    
    int getBirdType() { return m_iBirdType; }
    const char * getFbId() { return m_fbId; }
    const char * getUsername();
    int getScore() { return m_iScore; }
    float getLength() { return m_iLength; }
    unsigned int getSeed() { return m_iSeed; }
    const unsigned char * getData(int * pSize);
    
    float getNextMessageTime();
    MultiplayerMessage * getNextMessage();
    
    std::string toString();
    
private:
    
    // ===========================================================
    // Fields
    // ===========================================================
    
    unsigned int m_iSeed;
    int m_iBirdType;
    int m_iScore;
    float m_iLength;
    unsigned int m_iSize;
    const unsigned char * m_pData;
    char m_userId[60];
    char m_userName[100];
    char m_fbId[40];
    unsigned int m_iReaderPos;
};

#endif /* defined(__tapfly__GameRecording__) */
