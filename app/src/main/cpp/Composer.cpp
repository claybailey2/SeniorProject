//
// Created by Clay Bailey on 2020/11/01.
//

#include "Composer.h"
#include <pthread.h>

using namespace std;

#define NANOS_PER_SEC 1000000000L
#define LOAD_TIME 2

void *readRoutine(void *composerP) {
    Composer *composer = reinterpret_cast<Composer *>(composerP);
    timespec curTime;
    LOGI("Messages: %i", composer->mMessages.size());
    while (composer->mMessages.size() > 0) {
        clock_gettime(CLOCK_MONOTONIC, &curTime);
        if (   composer->mMessages[0]._timestamp.tv_sec  <= curTime.tv_sec
            && composer->mMessages[0]._timestamp.tv_nsec <= curTime.tv_nsec
                ) {
            //LOGI("Current Seconds: %ld, Timestamp Seconds: %ld",
            //        curTime.tv_sec,messages[0]._timestamp.tv_sec);
            //LOGI("Sending message...");
            AMidiInputPort_send(composer->mInputPort, composer->mMessages[0]._msgBuff, 3);

            composer->mMessages.erase(composer->mMessages.begin());
            //LOGI("Messages: %i", messages.size());
        }
    }
    return nullptr;
}

uint8_t *Composer::format(int noteNum) {
    uint8_t *buffer = new uint8_t [3];
    int channel = 0; // MIDI channels 1-16 are encoded as 0-15.
    buffer[0] = (uint8_t)(0x90 + (channel - 1)); // note on
    buffer[1] = (uint8_t)noteNum; // pitch
    buffer[2] = (uint8_t)127; //velocity
    return buffer;
}

void Composer::setInputPort(AMidiInputPort *inputPort) {
    mInputPort = inputPort;
}

//Send the member song data on a loop for numNotes note events
void Composer::autoPlay(int numNotes) {
    timespec startTime;
    clock_gettime(CLOCK_MONOTONIC,&startTime);
    LOGI("Start S: %ld  Start NS: %ld",startTime.tv_sec,startTime.tv_nsec);
    int songIndex = 0;
    for (int i = 0; i < numNotes; i++) {
        //loop song
        if (songIndex == mSong.size()) songIndex = 0;

        //create byte arrays
        uint8_t *msgBuff = format(mSong[songIndex]);
        songIndex++;

        //create timestamps
        timespec timestamp;
        timestamp.tv_sec = startTime.tv_sec + i;
        timestamp.tv_nsec = startTime.tv_nsec;

        //load mMessages
        TimedMsg msg(msgBuff, timestamp);
        mMessages.emplace_back(msg);
    }

    //read mMessages
    int pthread_result =
            pthread_create(&readThread, NULL, readRoutine, this);
    if (pthread_result != 0) LOGE("Error making read thread!");
}



