//
// Created by Clay Bailey on 2020/08/31.
//
#include "AudioEngine.h"

void AudioEngine::start() {
    AudioStreamBuilder builder;

    builder.setFormat(AudioFormat::Float);
    builder.setChannelCount(1);
    builder.setPerformanceMode(PerformanceMode::LowLatency);
    builder.setSharingMode(SharingMode::Exclusive);//Do not mix audio with another app's (bypass mixer = lower latency)
    builder.setCallback(this);

    builder.openStream(&mStream);

    //interrogate audio device for minimum amount of data it will read in one operation (burst)
    //use two bursts to prevent underruns
    mStream->setBufferSizeInFrames(mStream->getFramesPerBurst() * 2);

    //create oscillator with sample rate from stream
    mKick = new KickDrum(80.0, 0.5, mStream->getSampleRate());

    mStream->requestStart();
}

/*
 * Overriding function inherited from AudioStreamCallback.
 * Method called when the audio device needs more data.
 *
 * @AudioStream *oboeStream:
 * The stream who wants more data.
 *
 * @void *audioData:
 * Container array, either 16bit ints or float samples, to which the audio data will be written.
 *
 * @int32_t numFrames:
 * Number of audio frames which need to be sent in the audioData container array.
 */
DataCallbackResult
AudioEngine::onAudioReady(AudioStream *oboeStream, void *audioData, int32_t numFrames) {
    mKick->renderAudio(static_cast<float *>(audioData), numFrames);
    return DataCallbackResult::Continue;
}

int32_t AudioEngine::getBufferSize() {
    if (mStream != nullptr) {
        return mStream->getFramesPerBurst() * 2;
    } else return 512;
}

void AudioEngine::tap(bool isDown) {
    mKick->tap(isDown);
}
