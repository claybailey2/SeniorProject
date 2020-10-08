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
    Envelope(vector<float> values, vector<float> times, float exponent, int32_t sampleRate) {
        if (sampleRate == 0) {
            LOGE(__FILE__": sample rate must be greater than 0.");
        }
        if (values.size() != times.size() + 1) {
            LOGE(__FILE__": values-times vector size mismatch.");
        }

        mValues = values;

        //one-sample step for when time is 0.
        mTimes = times;
        for (int i = 0; i < mTimes.size(); i++) {
            if(mTimes[i] == 0) mValues[i] = 1 / sampleRate;
        }

        mExponent = exponent;

        //allocate new envelope
        double totalTime = 0;
        for (double time : mTimes) totalTime += time;
        mLength = totalTime * kControlRate;
        mEnvelope.resize(mLength);

        //fill in envelope sample by sample by translating 0,0->1,1 of polynomials
        int32_t curFrame = 0;
        for (int i = 0; i < mValues.size() - 1; i++) {
            double startVal = mValues[i];
            double endVal = mValues[i + 1];

            double startTime = i == 0 ? 0 : (mTimes[i-1] * kControlRate);
            double endTime = 0;
            for (int j = 0; j < i + 1; j++) endTime += mTimes[j] * kControlRate;

            while (curFrame < endTime && curFrame < mLength) {
                mEnvelope[curFrame] = (endVal - startVal)
                                      * pow(((curFrame - startTime) / (endTime - startTime )),
                                              mExponent)
                                      + startVal;
                curFrame++;
            }

            //manually set the last frame to be the last value
            mEnvelope[mLength - 1] = *mValues.end();
        }

        setSampleRate(sampleRate);
    }

    Envelope(const Envelope &src) {
        mValues = src.mValues;
        mTimes = src.mTimes;
        mExponent = src.mExponent;

        mSampleRate = src.mSampleRate;
        mMaxControlCount = src.mMaxControlCount;

        mEnvelope = src.mEnvelope;
        mLength = src.mLength;
    }


    void setSampleRate(int32_t sampleRate){
        mSampleRate = sampleRate;
        mMaxControlCount = mSampleRate / kControlRate; //todo: consider effects of truncation
    }

    void reset() {
        mPosition = 0;
    }

    void play() {
        //LOGD(__FILE__": Playing Envelope.");
        mIsPlaying = true;
    }

    void stop() {
        mIsPlaying = false;
    }

    //read current value and increment envelope
    float get() {
        float current = mEnvelope[mPosition];

        //if control < maxCtl
        if (mCurrentControlCount < mMaxControlCount) {
            //increment control
            mCurrentControlCount++;
        } else {
            //reset control and
            mCurrentControlCount = 0;
            //if playing AND in range
            if (mIsPlaying && mPosition < mLength - 1) {
                //increment pos
                mPosition++;
            }
        }

        return current;
    }

private:
    vector<float> mValues;
    vector<float> mTimes;
    float mExponent;

    int32_t mSampleRate;
    int32_t mMaxControlCount;
    int32_t mCurrentControlCount = 0;

    vector<float> mEnvelope;
    int mPosition = 0;
    int mLength;

    bool mIsPlaying {false};
};

#endif //SENIOR_PROJECT_ENVELOPE_H
