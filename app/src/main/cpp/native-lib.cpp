#include <jni.h>
#include <string>

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
