LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := SDL_image
LOCAL_SRC_FILES := lib/libSDL_image.so

include $(PREBUILT_SHARED_LIBRARY)
