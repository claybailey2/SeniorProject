    #include <jni.h>
    #include "AudioEngine.h"

    /*
     * JNI CODE LIVES HERE
     */

    extern "C" {
    //Start AudioEngine
    JNIEXPORT jlong JNICALL
    Java_blog_claybailey_seniorproject_MainActivity_startEngine(JNIEnv *env, jobject thiz) {
        auto engine = new AudioEngine();
        engine->start();
        return reinterpret_cast<jlong>(engine);
    }

    //Stop AudioEngine
    JNIEXPORT void JNICALL
    Java_blog_claybailey_seniorproject_MainActivity_stopEngine(JNIEnv *env, jobject thiz,
                                                               jlong jEngineHandle) {
        auto engine = reinterpret_cast<AudioEngine *>(jEngineHandle);
        engine->pause();
    }

    //Handle tap event
    JNIEXPORT void JNICALL
    Java_blog_claybailey_seniorproject_MainActivity_tap(JNIEnv *env, jobject thiz,
                                                        jlong jEngineHandle, jboolean b) {
        auto engine = reinterpret_cast<AudioEngine *>(jEngineHandle);
        if (engine) {
            engine->tap(b);
        }
    }

    //update default stream values for hardware
    JNIEXPORT void JNICALL
    Java_blog_claybailey_seniorproject_MainActivity_native_1setDefaultStreamValues(JNIEnv *env,
                                                                                   jclass thiz,
                                                                                   jint sample_rate,
                                                                                   jint frames_per_burst) {
        oboe::DefaultStreamValues::SampleRate = (int32_t) sample_rate;
        oboe::DefaultStreamValues::FramesPerBurst = (int32_t) frames_per_burst;
    }

    JNIEXPORT void JNICALL
    Java_blog_claybailey_seniorproject_MainActivity_resumeEngine(JNIEnv *env, jobject thiz,
                                                                 jlong jEngineHandle) {
        auto engine = reinterpret_cast<AudioEngine *>(jEngineHandle);
        engine->resume();
    }
    }