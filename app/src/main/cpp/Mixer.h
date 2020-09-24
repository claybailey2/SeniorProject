//
// Created by Clay Bailey on 2020/09/10.
//

#ifndef SENIOR_PROJECT_MIXER_H
#define SENIOR_PROJECT_MIXER_H

#include "IRenderableAudio.h"
#include "Envelope.h"
#include <array>

constexpr int32_t kBufferSize = 192*10; //TODO: find a way to ask the stream for a reasonable buffer size
constexpr int32_t kMaxTracks = 100;

class Mixer : public IRenderableAudio {
public:
    void renderAudio(float *audioData, int32_t numFrames) override {
        memset(audioData, 0, sizeof(float) * numFrames);
        for (int i = 0; i < mNextFreeTrack; i++) {
            mTracks[i]->renderAudio(mMixingBuffer,numFrames);
            for (int j = 0; j < numFrames; j++) {
                audioData[j] += mMixingBuffer[j] * mAmplitudes[i] * mEnvelopes[i]->get();
            }
        };
    }

    //optional arguments of amplitudeEnvelope and amplitude
    void addTrack(IRenderableAudio *track, Envelope *envelope = nullptr, float amplitude = 1.0){
        mTracks[mNextFreeTrack] = track;
        mEnvelopes[mNextFreeTrack] = envelope;
        mAmplitudes[mNextFreeTrack] = amplitude;

        if(mNextFreeTrack < kMaxTracks - 1) {
            mNextFreeTrack++;
        }
    }

private:
    int mChannelCount;//TODO: incorporate multichannel functionality
    float mMixingBuffer[kBufferSize];
    IRenderableAudio* mTracks[kMaxTracks];
    Envelope *mEnvelopes[kMaxTracks];
    float mAmplitudes[kMaxTracks];
    uint8_t mNextFreeTrack = 0;
};


#endif //SENIOR_PROJECT_MIXER_H
