//
// Created by Clay Bailey on 2020/09/03.
//

#ifndef SENIOR_PROJECT_SQUAREOSC_H
#define SENIOR_PROJECT_SQUAREOSC_H

#include <cstdint>
#include <math.h>
#include "IRenderableAudio.h"

//define constants
constexpr double kDefaultFrequency = 440.0;
constexpr int32_t kDefaultSampleRate = 48000;
constexpr double kPi = M_PI;
constexpr double kTwoPi = 2 * kPi;


class SquareOsc : public IRenderableAudio {
public:
    SquareOsc(double frequency, int32_t sampleRate) : mFrequency(frequency), mSampleRate(sampleRate) {
        UpdatePhaseIncrement();
    }

    //from "Low Latency Audio: Because Your Ears Are Worth It (Android Dev Summit '18)
    void renderAudio(float *audioData,int32_t numFrames) override {
        if(mIsWaveOn){
            for (int i = 0; i < numFrames; i++) {
                //square wave
                if (mPhase <= kPi){
                    audioData[i] = -mAmplitude;
                } else {
                    audioData[i] = mAmplitude;
                }

                mPhase += mPhaseIncrement;
                //reset phase
                if(mPhase > kTwoPi) mPhase -= kTwoPi;
            }
        } else {
            memset(audioData, 0, sizeof(float) * numFrames);
        }
    };

    void setFrequency(double frequency) {
        mFrequency = frequency;
    }

    void SetWaveOn(bool isDown) {
        mIsWaveOn = isDown;
    }

private:
    bool mIsWaveOn {false};
    double mFrequency = kDefaultFrequency;
    int32_t mSampleRate = kDefaultSampleRate;
    float mAmplitude = 0.5;
    float mPhase = 0.0;
    float mPhaseIncrement {};

    void UpdatePhaseIncrement() {
        mPhaseIncrement = static_cast<float>(kTwoPi * mFrequency / mSampleRate);
    }
};

#endif //SENIOR_PROJECT_SQUAREOSC_H
