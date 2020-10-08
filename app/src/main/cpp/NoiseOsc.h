//
// Created by Clay Bailey on 2020/09/10.
//

#ifndef SENIOR_PROJECT_NOISEOSC_H
#define SENIOR_PROJECT_NOISEOSC_H

#include "IRenderableAudio.h"

#include <stdlib.h>

constexpr int32_t kNumIncrements = 10000;

class NoiseOsc : public  IRenderableAudio {
public:
    void renderAudio(float *audioData, int32_t numFrames) override {
        if(mIsOn){
            for (int i = 0; i < numFrames; i++) {
                //generate a random float value between -1.0 and 1.0
                float noise = ((rand() % (kNumIncrements * 2) + 1) / kNumIncrements) - 1;
                assert(noise <= 1 && noise >= -1);
                audioData[i] = noise;
            }
        } else {
            memset(audioData, 0, sizeof(float) * numFrames);
        }
    }

    void setWaveOn(bool isOn){
        mIsOn = isOn;
    }

private:
    bool mIsOn {false};
};


#endif //SENIOR_PROJECT_NOISEOSC_H
