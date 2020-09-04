#include <jni.h>
#include <string>
#include <sstream>
#include <thread>
#include "AudioEngine.h"

/*
 * JNI CODE LIVES HERE
 */

AudioEngine engine;

/*
 * Example JNI function loaded in with native C++ app template
 */
extern "C" JNIEXPORT jstring JNICALL
Java_blog_claybailey_seniorproject_MainActivity_stringFromJNI(//notice naming convention
        JNIEnv* env,//pointer to pointer to function table
        jobject /* this */) {
    unsigned int nCores = std::thread::hardware_concurrency();
    std::ostringstream helloStream;
    helloStream << "This hardware supports concurrency with "
            << nCores << " cores available.";
    std::string hello = helloStream.str();
    //Constructs a new java.lang.String object from an array of characters in modified UTF-8 encoding.
    return env->NewStringUTF(hello.c_str());
}

//Start AudioEngine
extern "C"
JNIEXPORT void JNICALL
Java_blog_claybailey_seniorproject_MainActivity_startEngine(JNIEnv *env, jobject thiz) {
    engine.start();
}

//Stop AudioEngine
extern "C"
JNIEXPORT void JNICALL
Java_blog_claybailey_seniorproject_MainActivity_stopEngine(JNIEnv *env, jobject thiz) {
    //TODO
}

//Handle tap event
extern "C"
JNIEXPORT void JNICALL
Java_blog_claybailey_seniorproject_MainActivity_tap(JNIEnv *env, jobject thiz,
                                                    jboolean b) {
    engine.tap(b);
}

//update default stream values for hardware
extern "C"
JNIEXPORT void JNICALL
Java_blog_claybailey_seniorproject_MainActivity_native_1setDefaultStreamValues(JNIEnv *env,
                                                                               jclass thiz,
                                                                               jint sample_rate,
                                                                               jint frames_per_burst) {
    oboe::DefaultStreamValues::SampleRate = (int32_t) sample_rate;
    oboe::DefaultStreamValues::FramesPerBurst = (int32_t) frames_per_burst;
}