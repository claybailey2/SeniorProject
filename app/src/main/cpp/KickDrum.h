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
    KickDrum(double frequency, float amplitude, int32_t sampleRate) :

            mAmplitude(amplitude),

            mBody(frequency, sampleRate),

            mClickEnvelope(
                    vector<float> {0, 1, 0},
                    vector<float> {0.007, 0.015},
                    3, sampleRate),

            mBodyEnvelope(
                    vector<float> {0, 1, 0},
                    vector<float> {0.01, 0.3},
                    3, sampleRate),

            mMixer(mAmplitude)

    {
        LOGD("Kick Constructor");
        mBody.setWaveOn(true);
        mClick.setWaveOn(true);

        mMixer.addTrack(&mBody, &mBodyEnvelope, 0.6);
        mMixer.addTrack(&mClick, &mClickEnvelope,0.01);
    }

    void tap() {

        mClickEnvelope.reset();
        mBodyEnvelope.reset();

        mClickEnvelope.play();
        mBodyEnvelope.play();

    }

    void renderAudio(float *audioData, int32_t numFrames) override {
        mMixer.renderAudio(audioData, numFrames);
    }

private:
    float mAmplitude;
    SinOsc mBody;
    Envelope mClickEnvelope;
    Envelope mBodyEnvelope;

    Mixer mMixer;
    NoiseOsc mClick;
};

#endif //SENIOR_PROJECT_KICKDRUM_H
