//
// Created by Clay Bailey on 2020/09/11.
//

#ifndef SENIOR_PROJECT_ENVELOPE_H
#define SENIOR_PROJECT_ENVELOPE_H

#include <vector>
using namespace std;

class Envelope {
public:
    Envelope(vector<double> values, vector<double> times, double exponent, int32_t sampleRate)
    : mValues(values), mTimes(times), mExponent(exponent) {
        //what will we do with invalid input? i.e. size of values and times dont match up
        setSampleRate(sampleRate);
    }

    ~Envelope(){
        if (mEnvelope != nullptr){
            delete mEnvelope;
        }
    }

    void setSampleRate(int32_t sampleRate){
        mSampleRate = sampleRate;

        if (mEnvelope != nullptr) delete mEnvelope;
        double totalTime = 0;
        for (double time : mTimes) totalTime += time;
        mEnvelope = new float(totalTime * mSampleRate);
        //TODO: interpolate values
        //http://www.pmean.com/10/ExponentialInterpolation.html
        //y = c * (d/c)^((x-a) / (b-a))
    }

private:
    vector<double> mValues;
    vector<double> mTimes;
    double mExponent;
    int32_t mSampleRate;
    float *mEnvelope;//how will we be able to tell where the envelope ends?
    float *mCurrent;
};

#endif //SENIOR_PROJECT_ENVELOPE_H
