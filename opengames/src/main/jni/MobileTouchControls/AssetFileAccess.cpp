#include <jni.h>
#include <android/log.h>
extern "C"
{

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,"AssetFileAccess", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "AssetFileAccess", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,"AssetFileAccess", __VA_ARGS__))


#include "AssetFileAccess.h"

JNIEnv * getEnv();

int assetFopen(const char *filename, const char *mode)
{
    LOGI("assetFopen %s",filename);

    JNIEnv * env = getEnv();

	jclass cls =  env->FindClass("com/beloko/AssetFileAccess");
	jmethodID method = env->GetStaticMethodID(cls,  "fopen", "(Ljava/lang/String;Ljava/lang/String;)I");

	jstring filenameStr = env->NewStringUTF(filename);
	jstring modeStr = env->NewStringUTF(mode);


	return env->CallStaticIntMethod(cls, method, filenameStr, modeStr);
}

int assetFlen(int handle)
{
    JNIEnv * env = getEnv();

	jclass cls =  env->FindClass("com/beloko/AssetFileAccess");
	jmethodID method = env->GetStaticMethodID(cls,  "flen", "(I)I");

	return env->CallStaticIntMethod(cls, method, handle);
}

}