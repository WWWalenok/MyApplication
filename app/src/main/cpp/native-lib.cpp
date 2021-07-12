#include <jni.h>
#include <string>
#include "Include/FindA4.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_myapplication_MainActivity_GetFa4(
        JNIEnv* env,
jobject /* this */) {

std::string hello = fa4::GetVersion();
return env->NewStringUTF(hello.c_str());
}