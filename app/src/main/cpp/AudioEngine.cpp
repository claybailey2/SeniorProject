//
// Created by Clay Bailey on 2020/08/31.
//
#include "AudioEngine.h"
#include "logging_macros.h"

AudioEngine::AudioEngine()
{
}

AudioEngine::~AudioEngine() {
    mStream->close();
    delete mKick;
    delete mSteelDrum;
}

void AudioEngine::start() {

    AudioStreamBuilder builder;

    builder.setFormat(AudioFormat::Float);
    builder.setChannelCount(1);
    builder.setPerformanceMode(PerformanceMode::LowLatency);
    builder.setSharingMode(
            SharingMode::Shared);//Do not mix audio with another app's (bypass mixer = lower latency)
    builder.setCallback(this);

    builder.openStream(&mStream);

    //create oscillator with sample rate from stream
    if (mStream == nullptr) LOGE("mStream is NULL");

    //interrogate audio device for minimum amount of data it will read in one operation (burst)
    //use two bursts to prevent underruns
    int32_t bufferSize = mStream->getFramesPerBurst() * 2;
    mStream->setBufferSizeInFrames(bufferSize);
    mStream->flush(0);
    //mStream->write(,bufferSize,0);

    mKick = new KickDrum(50.0, 0.7, mStream->getSampleRate());
    mMasterMixer.addTrack(mKick);

    mSteelDrum = new SteelDrum(440.0, 0.3, mStream->getSampleRate());
    mMasterMixer.addTrack(mSteelDrum);


    mStream->requestStart();
    LOGD("Engine Started");
}

void AudioEngine::pause() {
    mIsInPause = true;
    mStream->pause();
}

void AudioEngine::resume() {
    mIsInPause = false;
    if(mStream->getState() == oboe::StreamState::Paused ||
        mStream->getState() == oboe::StreamState::Open ||
        mStream->getState() == oboe::StreamState::Stopped)
    {
        mStream->flush();
    }
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
    if (mIsInPause) return  DataCallbackResult::Stop; //stop audio when app is out of focus

    if (mKick == nullptr || mSteelDrum == nullptr) {//ensure synths are created
        return DataCallbackResult::Continue;
    }

    uint8_t inDataBuffer[SIZE_MIDIBUFFER];
    uint32_t numMessages;
    int32_t opCode;
    int64_t timestamp;
    size_t numBytesReceived;

    // Read MIDI Data
    numMessages = AMidiOutputPort_receive(mOutputPort, &opCode, inDataBuffer,
                                          sizeof(inDataBuffer), &numBytesReceived, &timestamp);
    if (numMessages < 0) {
        LOGW("Failure receiving MIDI data %zd", numMessages);
    }
    if (numMessages > 0) {
        LOGD("Data received!");

        // Parse and respond to MIDI data
        tapSteelDrum(700);
    }

    try {
        mMasterMixer.renderAudio(static_cast<float *>(audioData), numFrames);// render audio
    } catch(const std::overflow_error& e){
        LOGE("%s",e.what());
    }

    return DataCallbackResult::Continue;
}

void AudioEngine::tapKick() {
    mKick->tap();
}

void AudioEngine::tapSteelDrum(double frequency) {
    LOGD("Tap steel drum");
    mSteelDrum->setFrequency(frequency);
    mSteelDrum->tap();
}

void AudioEngine::setOutputPort(AMidiOutputPort *outputPort) {
    mOutputPort = outputPort;
}

