//
// Created by Clay Bailey on 2020/10/31.
//
#include <amidi/Amidi.h>
#include <vector>
#include <time.h>
#include <atomic>
#include "logging_macros.h"

using namespace std;

class TimedMsg {
public:
    uint8_t *_msgBuff; //3 byte message
    timespec _timestamp;
    TimedMsg(uint8_t *msgBuff, timespec timestamp) : _msgBuff(msgBuff), _timestamp(timestamp){}
};

class Composer {
public:
    vector<int> mSong = {60,60,67,67,69,69,67,55};
    atomic<AMidiInputPort *>mInputPort;
    vector<TimedMsg> mMessages;
    pthread_t readThread;

    uint8_t *format(int noteNum);
    void setInputPort(AMidiInputPort *inputPort);
    //Send the member song data on a loop for numNotes note events
    void autoPlay(int numNotes);
};