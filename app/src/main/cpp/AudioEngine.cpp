//
// Created by Clay Bailey on 2020/08/31.
//
#include "AudioEngine.h"
#include "logging_macros.h"

AudioEngine::AudioEngine() {
    mKick = nullptr;
}

AudioEngine::~AudioEngine() {
    mStream->close();
    delete mKick;
}

void AudioEngine::start() {

    AudioStreamBuilder builder;

    builder.setFormat(AudioFormat::Float);
    builder.setChannelCount(1);
    builder.setPerformanceMode(PerformanceMode::LowLatency);
    builder.setSharingMode(
            SharingMode::Exclusive);//Do not mix audio with another app's (bypass mixer = lower latency)
    builder.setCallback(this);

    builder.openStream(&mStream);

    //create oscillator with sample rate from stream
    if (mStream != nullptr) {
        //LOGD("Allocating Kick");
        //mKick = new KickDrum(50.0, 0.5, mStream->getSampleRate());
    }
    else LOGE("mStream is NULL");

    //interrogate audio device for minimum amount of data it will read in one operation (burst)
    //use two bursts to prevent underruns
    mStream->setBufferSizeInFrames(mStream->getFramesPerBurst() * 2);

    mStream->requestStart();
    LOGD("Engine Started");
}

void AudioEngine::pause() {
    mIsInPause = true;
    mStream->pause();
}

void AudioEngine::resume() {
    mIsInPause = false;

    mStream->flush();
    mStream->start();
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
    //LOGD("Audio Ready");
    if (mIsInPause) return  DataCallbackResult::Stop;
    if (mKick == nullptr) {
        return DataCallbackResult::Continue;
    }
    try {
        mKick->renderAudio(static_cast<float *>(audioData), numFrames);
    } catch(const std::overflow_error& e){
        LOGE("%s",e.what());
    }

    return DataCallbackResult::Continue;
}

void AudioEngine::tap() {
    if (mKick == nullptr) mKick = new KickDrum(110.0, 0.5, mStream->getSampleRate());
    mKick->tap();
}


