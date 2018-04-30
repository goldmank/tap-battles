//
//  GameRecorder.h
//  tapfly
//
//  Created by Kfir Goldman on 3/12/15.
//
//

#ifndef __tapfly__GameRecorder__
#define __tapfly__GameRecorder__

#include <stdio.h>
#include <string>
#include <stdlib.h>

class GameRecording;

// ===========================================================
// Constants
// ===========================================================

#define MSG_TYPE_BIRD  0
#define MSG_TYPE_BOUNS 1
#define MSG_TYPE_PUSHER 2

#define PLAYER_ANIM_STATE_IFALL 1
#define PLAYER_ANIM_STATE_FALL  2
#define PLAYER_ANIM_STATE_FLY   3
#define PLAYER_ANIM_STATE_HIT   4

#define BOUNS_STATE_CREATE      1
#define BOUNS_STATE_DESTROY     2
#define BOUNS_STATE_COLLECT     3

#define TAG_GROUND (0)
#define TAG_ENEMY  (1)
#define TAG_PLAYER (2)
#define TAG_BOUNS  (3)
#define TAG_BOUNS_2  (4)
#define TAG_BOUNS_3  (5)
#define TAG_BOUNS_4  (6)
#define TAG_CLOSE    (100)
#define TAG_UNLOCK   (101)
#define TAG_ADD      (102)

// ===========================================================
// Messages
// ===========================================================

struct MultiplayerMessage
{
    int msgType;
    float t;
};

struct MutliplayerPos : MultiplayerMessage
{
    int state;
    float y;
};

struct MutliplayerBouns : MutliplayerPos
{
    int bounsType;
};

// ===========================================================
// GameRecording
// ===========================================================

class GameRecorder
{
public:
    
    // ===========================================================
    // Initialization & cleanup
    // ===========================================================
    
    GameRecorder(unsigned int seed, int birdType);
    ~GameRecorder();
    
    // ===========================================================
    // Public methods
    // ===========================================================
    
    void push(MultiplayerMessage * pMessage, unsigned int size);
    
    void setScore(int score) { m_iScore = score; }
    void setLength(int length) { m_iLength = length; }
    
    GameRecording * getData();
    
private:
    
    // ===========================================================
    // Fields
    // ===========================================================
    
    long m_iSeed;
    int m_iBirdType;
    int m_iScore;
    float m_iLength;
    unsigned int m_iSize;
    FILE * m_pFile;
    std::string m_fileName;
};

#endif /* defined(__tapfly__GameRecorder__) */
