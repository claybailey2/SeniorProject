//
// Created by Clay Bailey on 2020/09/25.
//

#ifndef SENIOR_PROJECT_STEELDRUM_H
#define SENIOR_PROJECT_STEELDRUM_H

#include <vector>

#include "IRenderableAudio.h"
#include "logging_macros.h"

using namespace std;

const vector<vector<double>> kPartials = {{1, 1}, {2, .2}, {2.6, .1}, {3.2, .1}, {5.6, .1},
                                              {8.2, .1}, {2.9, .1}, {3, .1}, {4.2, .1}, {6.6, .1}};

class SteelDrum : public IRenderableAudio {
public:
    SteelDrum(double frequency, float amplitude, int32_t sampleRate):
    mSampleRate(sampleRate),
    //mAmplitude(amplitude),
    mMixer(amplitude),
    mEnvelope(
            {0,1,0},
            {0.008,0.8},
            3,
            mSampleRate
            )
    {
        float sumPartialAmplitudes = 0;
        for (auto partial : kPartials) {
            sumPartialAmplitudes += partial[1];
        }
        mSumPartialAmplitudes = sumPartialAmplitudes;
        setFrequency(frequency);
    }

    void setFrequency(double frequency){
        mFrequency = frequency;
        if(mOscs.size() == 0) {
            LOGD(__FILE__": initializing oscs.");
            for(int i = 0; i < kPartials.size(); i++) {
                double partialFrequency = mFrequency * kPartials[i][0];
                mOscs.emplace_back(SinOsc(partialFrequency, mSampleRate));
            }
        }
        else {
            for (auto osc : mOscs) {
                osc.setFrequency(frequency);
            }
        }
        for (int i = 0; i < mOscs.size(); i++) {
            float partialAmplitude = kPartials[i][1];
            float normalizedPartialAmplitude = partialAmplitude / mSumPartialAmplitudes;
            mMixer.addTrack(&mOscs[i], &mEnvelope, normalizedPartialAmplitude);
        }
    }

    void renderAudio(float *audioData, int32_t numFrames) override {
        mMixer.renderAudio(audioData, numFrames);
    }

    void tap() {
        mEnvelope.reset();
        mEnvelope.play();
    }

private:
    int32_t mSampleRate;
    //double mAmplitude;
    Mixer mMixer;
    Envelope mEnvelope;
    float mSumPartialAmplitudes;
    double mFrequency;
    vector<SinOsc> mOscs;
};

#endif //SENIOR_PROJECT_STEELDRUM_H
