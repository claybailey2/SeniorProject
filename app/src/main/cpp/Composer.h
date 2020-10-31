//
// Created by Clay Bailey on 2020/10/31.
//

//#include <jni.h>
#include <amidi/Amidi.h>
#include <vector>
#include <chrono>
#include <time.h>

using namespace std;

#define NANOS_PER_SEC 1000000000L

class Composer {
private:
    vector<int> mSong = {60,60,67,67,69,69,67,55};
    AMidiInputPort *mInputPort;
    uint8_t *format(int noteNum) {
        uint8_t *buffer = new uint8_t [3];
        int channel = 0; // MIDI channels 1-16 are encoded as 0-15.
        buffer[0] = (uint8_t)(0x90 + (channel - 1)); // note on
        buffer[1] = (uint8_t)noteNum; // pitch
        buffer[2] = (uint8_t)127; //velocity
        return buffer;
    }
    void write(uint8_t msgBuff[3], size_t numBytes, int64_t timestamp) {
        AMidiInputPort_sendWithTimestamp(mInputPort,msgBuff,numBytes,timestamp);
    }

public:
    void setInputPort(AMidiInputPort *inputPort) {
        mInputPort = inputPort;
    }
    //Send the member song data on a loop for numNotes note events
    void autoPlay(int numNotes) {
        timespec startTime;
        clock_gettime(CLOCK_MONOTONIC,&startTime);

        int songIndex = 0;
        for (int i = 0; i < numNotes; i++) {
            //loop
            if (songIndex == mSong.size()) songIndex = 0;
            //create byte arrays
            uint8_t *msgBuff = format(mSong[songIndex]);
            songIndex++;
            //create timestamps
            //int64_t timestamp = (i * NANOS_PER_SEC);
            int64_t timestamp = 0;
            //senD messages
            LOGD("Sending MIDI...");
            write(msgBuff,3,timestamp);
            sleep(1);
        }
    }
};
