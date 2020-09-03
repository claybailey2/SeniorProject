#include <jni.h>
#include <string>
#include "AudioEngine.h"

/*
 * JNI CODE LIVES HERE
 */


/*
 * Example JNI function loaded in with native C++ app template
 */
extern "C" JNIEXPORT jstring JNICALL
Java_blog_claybailey_seniorproject_MainActivity_stringFromJNI(//notice naming convention
        JNIEnv* env,//pointer to pointer to function table
        jobject /* this */) {
    std::string hello = "Hello from C++";
    //Constructs a new java.lang.String object from an array of characters in modified UTF-8 encoding.
    return env->NewStringUTF(hello.c_str());
}

//Start AudioEngine
extern "C"
JNIEXPORT jlong JNICALL
Java_blog_claybailey_seniorproject_MainActivity_startEngine(JNIEnv *env, jobject thiz) {
    AudioEngine *engine = new AudioEngine();
    return reinterpret_cast<jlong>(engine);
}

//Stop AudioEngine
extern "C"
JNIEXPORT void JNICALL
Java_blog_claybailey_seniorproject_MainActivity_stopEngine(JNIEnv *env, jobject thiz,
                                                           jlong engine_handle) {
    AudioEngine *engine = reinterpret_cast<AudioEngine *>(engine_handle);
    delete engine;
}

//Handle tap event
extern "C"
JNIEXPORT void JNICALL
Java_blog_claybailey_seniorproject_MainActivity_tap(JNIEnv *env, jobject thiz,
                                                    jlong engineHandle, jboolean b) {
    auto engine = reinterpret_cast<AudioEngine *>(engineHandle);
    engine->tap(b);
}