LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := SDL_mixer
LOCAL_SRC_FILES := lib/libSDL_mixer.so

include $(PREBUILT_SHARED_LIBRARY)
