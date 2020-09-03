//
// Created by Clay Bailey on 2020/09/03.
//

#ifndef SENIOR_PROJECT_OSCILLATOR_H
#define SENIOR_PROJECT_OSCILLATOR_H

#include <cstdint>
#include <math.h>

//define constants
constexpr double kDefaultFrequency = 440.0;
constexpr int32_t kDefaultSampleRate = 48000;
constexpr double kPi = M_PI;
constexpr double kTwoPi = 2 * kPi;


class Oscillator {
public:
    Oscillator(double frequency, int32_t sampleRate) : mFrequency(frequency), mSampleRate(sampleRate) {
        UpdatePhaseIncrement();
    }



private:
    double mFrequency = kDefaultFrequency;
    int32_t mSampleRate = kDefaultSampleRate;
    float mAmplitude = 0.5;
    float mPhase = 0.0;
    float mPhaseIncrement {};

    void UpdatePhaseIncrement() {
        mPhaseIncrement = static_cast<float>(kTwoPi * mFrequency / mSampleRate);
    }
};

#endif //SENIOR_PROJECT_OSCILLATOR_H
