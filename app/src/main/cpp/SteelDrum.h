//
// Created by Clay Bailey on 2020/09/25.
//

#ifndef SENIOR_PROJECT_STEELDRUM_H
#define SENIOR_PROJECT_STEELDRUM_H

#include <vector>
#include <memory>

#include "IRenderableAudio.h"
#include "logging_macros.h"

using namespace std;

const vector<vector<float>> kPartials = {{1, 1}, {2, .2}, {2.6, .1}, {3.2, .1}, {5.6, .1},
                                              {8.2, .1}, {2.9, .1}, {3, .1}, {4.2, .1}, {6.6, .1}};

class SteelDrum : public IRenderableAudio {
public:
    SteelDrum(double frequency, float amplitude, int32_t sampleRate):
    mSampleRate(sampleRate),
    //mAmplitude(amplitude),
    mMixer(amplitude),
    mEnvelope(
            {0,1,0},
            //hack: just make the Envelope longer :P
            {static_cast<float>(0.008 * kPartials.size()), static_cast<float>(0.2 * kPartials.size())},
            1,
            mSampleRate
            )
    {
        setFrequency(frequency);
    }

    void setFrequency(double frequency){
        mFrequency = frequency;
        if(mOscs.size() == 0) {

            LOGD(__FILE__": initializing oscs.");

            for(int i = 0; i < kPartials.size(); i++) {
                double partialFrequency = mFrequency * kPartials[i][0];
                auto partialOsc = new SinOsc(partialFrequency, mSampleRate);
                LOGD(__FILE__": emplacing osc.");
                mOscs.emplace_back(partialOsc);
            }

            float sumPartialAmplitudes = 0;
            for (auto partial : kPartials) {
                sumPartialAmplitudes += partial[1];
            }

            for (int i = 0; i < mOscs.size(); i++) {
                float partialAmplitude = kPartials[i][1];
                float normalizedPartialAmplitude = sumPartialAmplitudes != 0 ?
                                                   partialAmplitude / sumPartialAmplitudes : 0;
                mMixer.addTrack(mOscs[i], &mEnvelope, normalizedPartialAmplitude);
                mOscs[i]->setWaveOn(true);
            }
        }
        else {
            for (int i = 0; i < mOscs.size(); i++) {
                double partialFrequency = mFrequency * kPartials[i][0];
                mOscs[i]->setFrequency(partialFrequency);
            }
        }
    }

    ~SteelDrum(){
        for (auto osc : mOscs) delete osc;
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
    double mFrequency;
    vector<SinOsc *> mOscs{};
};

#endif //SENIOR_PROJECT_STEELDRUM_H
