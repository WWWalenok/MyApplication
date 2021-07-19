#include <jni.h>
#include <string>
#include "Include/FindA4.hpp"

extern "C"
{

#define JNI_EXTERN(func) Java_ru_fittin_scanleg_utils_pas_Finder_ ## func

#ifndef UNUSED
#define UNUSED(x) (void)x
#endif

	unsigned char** convert_image(JNIEnv* jenv, jbyteArray jimg, int maxX, int maxY) {
		auto bytes = jenv->GetByteArrayElements(jimg, nullptr);
		auto** img = new unsigned char* [maxY];
		for (int i = 0; i < maxY; i++) { //640
			img[i] = new unsigned char[maxX];
			for (int j = 0; j < maxX; j++) { //480
				img[i][j] = (unsigned char)bytes[j + maxX * i];
			}
		}
		return img;
	}

	void release_image(unsigned char** img, int maxY) {
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



	JNIEXPORT jstring JNICALL
		JNI_EXTERN(GetVersion)(JNIEnv* env, jobject /* this */) {

		std::string hello = fa4::GetVersion();
		return env->NewStringUTF(hello.c_str());
	}

	JNIEXPORT jlong JNICALL
		JNI_EXTERN(InitNewFinder)(JNIEnv* env, jobject /* this */)
	{
		return (jlong)fa4::InitNewFinder();
	}

	JNIEXPORT jint JNICALL
		JNI_EXTERN(InitImage)(JNIEnv* env, jobject /* this */, jlong id, jbyteArray _arr, jint maxX, jint maxY, jint Kadr)
	{
		unsigned int maxthread = hardware_concurrency();
		auto** img = convert_image(jenv, _arr, maxX, maxY);

		for (int32_t i = 0, k = 0; i < maxX; i++)
		{
			img[i] = new unsigned char[maxY];
			for (int32_t j = 0; j < maxY; j++)
				img[i][j] = arr[j + i * maxY];
		}

		fa4::Prop p[2]{ fa4::Prop("Kadr", Kadr) };
		fa4::Init(id, p, 1);

		fa4::Prop p[2]{ fa4::Prop("ThreadCount", hardware_concurrency()) };
		fa4::Init(id, p, 1);

		fa4::InitImg(id, (uint8_t**)(img), int32_t(maxX), int32_t(maxY));
		return 0;
	}

	JNIEXPORT jint JNICALL
		JNI_EXTERN(Remove)(JNIEnv* env, jobject /* this */, jlong id)
	{
		fa4::Remove(id, 1);
		return 0;
	}



	JNIEXPORT jstring JNICALL
		JNI_EXTERN(GetCorner)(JNIEnv* env, jobject /* this */, jlong id, jfloatArray _list)
	{
		float* list = env->GetFloatArrayElements(_list, NULL);

		std::string hello = fa4::GetCorner(id, list);

		if (NULL != list) {
			env->ReleaseFloatArrayElements(_list, list, 0);
		}

		return env->NewStringUTF(hello.c_str());
	}

	JNIEXPORT jstring JNICALL
		JNI_EXTERN(GetCams)(JNIEnv* env, jobject /* this */, jlong id, jfloatArray _list, jfloatArray _cams)
	{
		float* list = env->GetFloatArrayElements(_list, NULL);
		float* tcams = env->GetFloatArrayElements(_list, NULL);

		float* cams[8];
		for (int i = 0; i < 8; i++)
			cams[i] = &tcams[i * 4];

		std::string hello = fa4::GetCams(id, list, cams);

		if (NULL != list) {
			env->ReleaseFloatArrayElements(_cams, tcams, 0);
		}

		return env->NewStringUTF(hello.c_str());
	}

	JNIEXPORT jstring JNICALL
		JNI_EXTERN(GetKrit)(JNIEnv* env, jobject /* this */, jlong id, jfloatArray _list, jfloatArray _krit)
	{
		float* list = env->GetFloatArrayElements(_list, NULL);
		float* krit = env->GetFloatArrayElements(_krit, NULL);

		std::string hello = fa4::GetKrit(id, list, krit);

		if (NULL != list) {
			env->ReleaseFloatArrayElements(_krit, krit, 0);
		}

		return env->NewStringUTF(hello.c_str());
	}

	JNIEXPORT jstring JNICALL
		JNI_EXTERN(GetTips)(JNIEnv* env, jobject /* this */, jlong id, jfloatArray _list, jfloatArray _cams, jbooleanArray _tip)
	{
		float* list = env->GetFloatArrayElements(_list, NULL);
		float* tcams = env->GetFloatArrayElements(_list, NULL);
		uint8_t* tip = env->GetBooleanArrayElements(_tip, NULL);

		float* cams[8];
		for (int i = 0; i < 8; i++)
			cams[i] = &tcams[i * 4];

		std::string hello = fa4::GetTips(id, list, cams, tip);

		if (NULL != list) {
			env->ReleaseBooleanArrayElements(_tip, tip, 0);
		}

		return env->NewStringUTF(hello.c_str());
	}

	JNIEXPORT jstring JNICALL
		JNI_EXTERN(UpdateOrientedCams)(JNIEnv* env, jobject /* this */, jlong id, jfloatArray _list, jfloatArray _cams)
	{
		float* list = env->GetFloatArrayElements(_list, NULL);
		float* tcams = env->GetFloatArrayElements(_list, NULL);

		float* cams[8];
		for (int i = 0; i < 8; i++)
			cams[i] = &tcams[i * 4];

		std::string hello = fa4::UpdateOrientedCams(id, list, cams);

		if (NULL != list) {
			env->ReleaseFloatArrayElements(_cams, tcams, 0);
		}

		return env->NewStringUTF(hello.c_str());
	}

	JNIEXPORT jstring JNICALL
		JNI_EXTERN(FootCollor)(JNIEnv* env, jobject /* this */, jlong id, jfloatArray _list)
	{
		float* list = env->GetFloatArrayElements(_list, NULL);


		std::string hello = fa4::FootCollor(id, list);


		return env->NewStringUTF(hello.c_str());
	}


	JNIEXPORT jstring JNICALL
		JNI_EXTERN(StartAll)(JNIEnv* env, jobject /* this */, jlong id)
	{
		std::string hello = fa4::StartAll(id);


		return env->NewStringUTF(hello.c_str());
	}
}