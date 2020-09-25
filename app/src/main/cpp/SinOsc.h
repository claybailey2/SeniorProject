//
// Created by Clay Bailey on 2020/09/24.
//

#ifndef SENIOR_PROJECT_SINOSC_H
#define SENIOR_PROJECT_SINOSC_H


#include <cstdint>
#include <math.h>
#include "IRenderableAudio.h"
#include "logging_macros.h"

class SinOsc : public IRenderableAudio {
public:
    //TODO: Change frequency to optional arg
    SinOsc(double frequency, int32_t sampleRate) : mFrequency(frequency), mSampleRate(sampleRate) {
        UpdatePhaseIncrement();
    }

    //from "Low Latency Audio: Because Your Ears Are Worth It (Android Dev Summit '18)
    void renderAudio(float *audioData,int32_t numFrames) override {
        if(mIsWaveOn){
            for (int i = 0; i < numFrames; i++) {
                //sin wave
                audioData[i] = sinf(mPhase) * mAmplitude;
                //LOGD(__FILE__"%f",audioData[i]);
                mPhase += mPhaseIncrement;

                //reset phase
                if(mPhase > kTwoPi) mPhase -= kTwoPi;
            }
        } else {
            memset(audioData, 0, sizeof(float) * numFrames);
        }
    }

    void setFrequency(double frequency) {
        mFrequency = frequency;
    }

    void SetWaveOn(bool isOn) {
        mIsWaveOn = isOn;
    }

private:
    bool mIsWaveOn {false};
    double mFrequency = kDefaultFrequency;
    int32_t mSampleRate = kDefaultSampleRate;
    float mAmplitude = 0.5;
    float mPhase = 0.0;
    float mPhaseIncrement {};

    void UpdatePhaseIncrement() {
        if (mSampleRate == 0) LOGE(__FILE__"Sample rate is zero!");
        mPhaseIncrement = static_cast<float>(kTwoPi * mFrequency / mSampleRate);
    }
};

#endif //SENIOR_PROJECT_SINOSC_H
