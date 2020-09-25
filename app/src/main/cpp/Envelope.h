//
// Created by Clay Bailey on 2020/09/11.
//

#ifndef SENIOR_PROJECT_ENVELOPE_H
#define SENIOR_PROJECT_ENVELOPE_H

#include <vector>
#include <math.h>
#include <limits>

#include "logging_macros.h"

using namespace std;

class Envelope {
public:
    Envelope(vector<double> values, vector<double> times, double exponent, int32_t sampleRate) {
        if (sampleRate == 0) {
            LOGE(__FILE__": sample rate must be greater than 0.");
        }
        if (values.size() != times.size() + 1) {
            LOGE(__FILE__": values-times vector size mismatch.");
        }

        //eliminate divide-by-zero and floor noise errors
        mValues = values;
        for (int i = 0; i < mValues.size(); i++) {
            if (mValues[i] == 0) mValues[i] = numeric_limits<double>::epsilon();
        }

        //one-sample step for when time is 0.
        mTimes = times;
        for (int i = 0; i < mTimes.size(); i++) {
            if(mTimes[i] == 0) mValues[i] = 1 / sampleRate;
        }

        mExponent = exponent;

        setSampleRate(sampleRate);

    }


    //TODO: KONTROL RATE
    void setSampleRate(int32_t sampleRate){
        mSampleRate = sampleRate;

        //allocate new envelope
        double totalTime = 0;
        for (double time : mTimes) totalTime += time;
        mLength = totalTime * mSampleRate;
        mEnvelope.resize(mLength);

        //fill in envelope sample by sample using exponential interpolation
        //TODO: change interpolator to use incoming exponent argument
        //http://www.pmean.com/10/ExponentialInterpolation.html
        int32_t curFrame = 0;
        for (int i = 0; i < mValues.size() - 1; i++) {
            double startVal = mValues[i];
            double endVal = mValues[i + 1];

            double startTime = i == 0 ? 0 : (mTimes[i-1] * mSampleRate);
            double endTime = 0;
            for (int j = 0; j < i + 1; j++) endTime += mTimes[j] * mSampleRate;

            while (curFrame < endTime && curFrame < mLength) {
                if (startVal == 0 || endTime == startTime) {
                    mEnvelope[curFrame] = 0;
                    LOGE(__FILE__": Divide by Zero");
                }
                else {
                    mEnvelope[curFrame] = startVal *
                                          pow((endVal / startVal),
                                              ((curFrame - startTime) / (endTime - startTime)));
                }
                curFrame++;
            }
        }
    }

    void reset() {
        mPosition = 0;
    }

    void play() {
        mIsPlaying = true;
    }

    void stop() {
        mIsPlaying = false;
    }

    //read current value and increment envelope
    float get() {
        float current = mEnvelope[mPosition];
        if (mIsPlaying && mPosition < mLength - 1)
            mPosition++;
        return current;
    }

private:
    vector<double> mValues;
    vector<double> mTimes;
    double mExponent;
    int32_t mSampleRate;

    vector<float> mEnvelope;
    int mPosition = 0;
    int mLength;

    bool mIsPlaying {false};
};

#endif //SENIOR_PROJECT_ENVELOPE_H
