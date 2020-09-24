//
// Created by Clay Bailey on 2020/09/10.
//

#ifndef SENIOR_PROJECT_KICKDRUM_H
#define SENIOR_PROJECT_KICKDRUM_H

#include <vector>

#include "IRenderableAudio.h"
#include "logging_macros.h"

using namespace std;

class KickDrum : public IRenderableAudio {
public:
    KickDrum(double frequency, float amplitude, int32_t sampleRate)
    : mFrequency(frequency), mAmplitude(amplitude), mSampleRate(sampleRate),
    mBody(frequency, sampleRate),
    mEnvelope(
            vector<double> {0, 1, 0},
            vector<double> {0.003, 0.2},
            0, sampleRate)
            {
        LOGD("Kick Constructor");
        mBody.SetWaveOn(true);
        mClick.SetWaveOn(true);

        mMixer.addTrack(&mBody, &mEnvelope, 0.5);
        mMixer.addTrack(&mClick, &mEnvelope,0.2);
    }

    void tap(bool isDown) {
        if (isDown) {
            mEnvelope.reset();
            mEnvelope.play();
        }
    }

    void renderAudio(float *audioData, int32_t numFrames) override {
        mMixer.renderAudio(audioData, numFrames);
    }

private:
    int32_t mSampleRate = 48000;
    double mFrequency = 50;
    float mAmplitude = 1.0;
    Mixer mMixer;

    SquareOsc mBody;
    NoiseOsc mClick;

    Envelope mEnvelope;
    vector<double> mValues = {0, 1, 0};
    vector<double> mTimes = {0.003, 0.2};
};

#endif //SENIOR_PROJECT_KICKDRUM_H
