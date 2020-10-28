//
// Created by Clay Bailey on 2020/08/31.
//

#ifndef SENIOR_PROJECT_AUDIOENGINE_H
#define SENIOR_PROJECT_AUDIOENGINE_H

// AudioEngine.h

#include <oboe/Oboe.h>
#include <amidi/AMidi.h>

#include "Constants.h"
#include "SinOsc.h"
#include "SquareOsc.h"
#include "NoiseOsc.h"
#include "Mixer.h"
#include "Envelope.h"

#include "KickDrum.h"
#include "SteelDrum.h"

constexpr int SIZE_MIDIBUFFER = 1024; //TODO: Figure out how big the midi buffer should be;

using namespace oboe;
//AudioEngine inherits from AudioStreamCallback
class AudioEngine : public AudioStreamCallback {
public:
    AudioEngine();
    ~AudioEngine();
    void start();
    void pause();
    void resume();
    void setOutputPort(AMidiOutputPort *outputPort);

    DataCallbackResult
    onAudioReady(AudioStream *oboeStream, void *audioData, int32_t numFrames) override;

    int32_t getBufferSize();

    void tapKick();
    void tapSteelDrum(double frequency);

private:
    AMidiOutputPort  *mOutputPort;
    Mixer mMasterMixer;
    bool mIsInPause = false;
    AudioStream *mStream = nullptr;
    KickDrum *mKick = nullptr;
    SteelDrum *mSteelDrum = nullptr;
};

#endif //SENIOR_PROJECT_AUDIOENGINE_H
