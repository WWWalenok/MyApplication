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