LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := SDL_net
LOCAL_SRC_FILES := lib/libSDL_net.a

include $(PREBUILT_STATIC_LIBRARY)
