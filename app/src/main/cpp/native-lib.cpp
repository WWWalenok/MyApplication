#include <jni.h>
#include <string>
#include "Include/FindA4.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_fittin_fa4Lib_Finder_GetVersion(
        JNIEnv* env,
jobject /* this */) {

std::string hello = fa4::GetVersion();
return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_fittin_fa4Lib_Finder_FindA4(
        JNIEnv* env,
        jobject /* this */,
        jint maxthreadcount,
        jboolean **img,
        jint maxX,
        jint maxY,
        jint imnum
        )
{
    std::string hello = fa4::GetA4FromImg((int32_t)(maxthreadcount), (uint8_t**)(img), int32_t(maxX), int32_t(maxY), int32_t(imnum), -1);
    return env->NewStringUTF(hello.c_str());
}

