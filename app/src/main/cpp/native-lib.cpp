#include "Include/FindA4.h"
#include <jni.h>
#include <string>
#include <fstream>
#include <thread>

#define JNI_EXTERN(func) Java_ru_fittin_scanleg_utils_jni_A4Find_ ## func

#ifndef UNUSED
#define UNUSED(x) (void)x
#endif

unsigned char **convert_image(JNIEnv *jenv, jbyteArray jimg, int maxX, int maxY) {
    auto bytes = jenv->GetByteArrayElements(jimg, nullptr);
    auto **img = new unsigned char *[maxY];
    for (int i = 0; i < maxY; i++) { //640
        img[i] = new unsigned char[maxX];
        for (int j = 0; j < maxX; j++) { //480
            img[i][j] = (unsigned char) bytes[j + maxX * i];
        }
    }
    return img;
}

void release_image(unsigned char **img, int maxY) {
    for (int i = 0; i < maxY; i++) delete[] img[i];
    delete[] img;
}

unsigned int old_hardware_concurrency() {
    std::ifstream cpuinfo("/proc/cpuinfo");
    return std::count(std::istream_iterator<std::string>(cpuinfo),
                      std::istream_iterator<std::string>(),
                      std::string("processor"));
}

unsigned int hardware_concurrency() {
    unsigned int cores = std::thread::hardware_concurrency();
    return cores ? cores : old_hardware_concurrency();
}

extern "C" JNIEXPORT jstring JNICALL
JNI_EXTERN(getAngel(JNIEnv * jenv, jclass jobj, jbyteArray jimg,
        jint maxX, //480
        jint maxY, //640
        jint cardnum)) {
    UNUSED(jobj);
    unsigned int maxthread = hardware_concurrency();
    auto **img = convert_image(jenv, jimg, maxX, maxY);
    std::string str = fa4::GetA4FromImg(maxthread, img, maxY, maxX, cardnum);
    release_image(img, maxY);
    return jenv->NewStringUTF(str.c_str());
}
//Java_ru_fittin_scanleg_utils_pas_Finder_