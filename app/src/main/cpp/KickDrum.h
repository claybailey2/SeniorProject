//
// Created by Clay Bailey on 2020/09/10.
//

#ifndef SENIOR_PROJECT_KICKDRUM_H
#define SENIOR_PROJECT_KICKDRUM_H

#include "IRenderableAudio.h"
#include "Mixer.h"


class KickDrum : public IRenderableAudio {
public:
    KickDrum(double frequency, float amplitude, int32_t sampleRate)
    : mFrequency(frequency), mAmplitude(amplitude), mSampleRate(sampleRate), mBody(frequency, sampleRate) {
        mMixer.addTrack(&mBody, 0.5);
        mMixer.addTrack(&mClick, 0.2);
    }

    void tap(bool isDown) {
        mBody.SetWaveOn(isDown);
        mClick.SetWaveOn(isDown);
    }

    void renderAudio(float *audioData, int32_t numFrames) override {
        mMixer.renderAudio(audioData, numFrames);
    }

private:
    int32_t mSampleRate = 48000;
    double mFrequency = 50;
    float mAmplitude = 1.0;
    Mixer mMixer;

    //dynamically allocated
    SquareOsc mBody;
    NoiseOsc mClick;
};

#endif //SENIOR_PROJECT_KICKDRUM_H
