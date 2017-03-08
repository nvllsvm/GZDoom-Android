#include <stdio.h>

#include "android_fopen.h"
#include <errno.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <android/log.h>
#include <jni.h>
#include "AssetFileCache.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,"android_fopen", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "android_fopen", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,"android_fopen", __VA_ARGS__))


#define EXPORT_ME __attribute__ ((visibility("default")))

#define USE_CACHE

extern "C"
{

void EXPORT_ME Java_com_beloko_AssetFileAccess_setAssetManager(JNIEnv *env, jobject obj,
        jobject assetManager)
{
    AAssetManager *mgr = (AAssetManager *)AAssetManager_fromJava(env, assetManager);
    android_fopen_set_asset_manager(mgr);
}

static int android_read(void* cookie, char* buf, int size) {
// LOGI("android_read %p %d",cookie,size);

#ifdef USE_CACHE
    AssetFileCache *cache = (AssetFileCache*)cookie;
    return cache->read( buf, size );
#else
    return AAsset_read((AAsset*)cookie, buf, size);
#endif
}

static int android_write(void* cookie, const char* buf, int size) {
    return EACCES; // can't provide write access to the apk
}

static fpos_t android_seek(void* cookie, fpos_t offset, int whence) {
#ifdef USE_CACHE
    AssetFileCache *cache = (AssetFileCache*)cookie;
    return cache->seek( offset, whence );
#else
    return AAsset_seek((AAsset*)cookie, offset, whence);
#endif
}

static int android_close(void* cookie) {
#ifdef USE_CACHE
    AssetFileCache *cache = (AssetFileCache*)cookie;
    delete cache;
#else
    AAsset_close((AAsset*)cookie);
#endif

    return 0;
}

// must be established by someone else...
AAssetManager* android_asset_manager;
void android_fopen_set_asset_manager(AAssetManager* manager) {
    android_asset_manager = manager;
}

FILE* android_fopen(const char* fname, const char* mode) {

    LOGI("android_fopen - %s",fname);

    if(mode[0] == 'w') return NULL;

    AAsset* asset = AAssetManager_open(android_asset_manager, fname, 0);
    if(!asset)
    {
        LOGE("android_fopen error opening: %s",fname);
        return NULL;
    }

#ifdef USE_CACHE
    AssetFileCache *cache = new AssetFileCache( asset );
    return funopen(cache, android_read, android_write, android_seek, android_close);
#else
    return funopen(asset, android_read, android_write, android_seek, android_close);
#endif
}

}