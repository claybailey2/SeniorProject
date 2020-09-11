//
// Created by Clay Bailey on 2020/08/31.
//

#ifndef SENIOR_PROJECT_AUDIOENGINE_H
#define SENIOR_PROJECT_AUDIOENGINE_H

// AudioEngine.h

#include <oboe/Oboe.h>

#include "SquareOsc.h"
#include "NoiseOsc.h"
#include "Mixer.h"
#include "Envelope.h"

#include "KickDrum.h"

using namespace oboe;
//AudioEngine inherits from AudioStreamCallback
class AudioEngine : public AudioStreamCallback {
public:
    void start();

    DataCallbackResult
    onAudioReady(AudioStream *oboeStream, void *audioData, int32_t numFrames) override;

    int32_t getBufferSize();

    void tap(bool isDown);

private:
    AudioStream *mStream;
    KickDrum *mKick;

};

#endif //SENIOR_PROJECT_AUDIOENGINE_H
