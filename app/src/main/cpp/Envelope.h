//
// Created by Clay Bailey on 2020/09/11.
//

#ifndef SENIOR_PROJECT_ENVELOPE_H
#define SENIOR_PROJECT_ENVELOPE_H

#include <vector>
#include <math.h>
using namespace std;

class Envelope {
public:
    Envelope(vector<double> values, vector<double> times, double exponent, int32_t sampleRate) {
        if (values.size() == times.size() + 1) {
            mValues = values;
            mTimes = times;
            mExponent = exponent;
            setSampleRate(sampleRate);
        }
    }

    ~Envelope(){
        if (mEnvelope != nullptr){
            delete mEnvelope;
        }
    }

    //TODO: KONTROL RATE
    void setSampleRate(int32_t sampleRate){
        mSampleRate = sampleRate;
        //clear previous envelope
        if (mEnvelope != nullptr) delete mEnvelope;

        //allocate new envelope
        double totalTime = 0;
        for (double time : mTimes) totalTime += time;
        mLength = totalTime * mSampleRate;
        mEnvelope = new float(mLength);

        //fill in envelope sample by sample using exponential interpolation
        //TODO: change interpolator to use incoming exponent argument
        //http://www.pmean.com/10/ExponentialInterpolation.html
        int32_t curFrame = 0;
        for (int i = 0; i < mValues.size() - 1; i++) {
            double startVal = mValues[i];
            double endVal = mValues[i+1];

            double startTime = i == 0 ? 0 : (mTimes[i-1] * mSampleRate);
            double endTime = mTimes[i] * mSampleRate;

            while (curFrame < endTime) {
                mEnvelope[curFrame] = startVal *
                        pow((endVal/startVal), ((curFrame-startTime) / (endTime-startTime)));
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
        float current = *(mEnvelope + mPosition);
        if (mIsPlaying && mPosition < mLength)
            mPosition++;
        return current;
    }

private:
    vector<double> mValues;
    vector<double> mTimes;
    double mExponent;
    int32_t mSampleRate;

    float *mEnvelope;
    int mPosition = 0;
    int mLength;

    bool mIsPlaying {false};
};

#endif //SENIOR_PROJECT_ENVELOPE_H
