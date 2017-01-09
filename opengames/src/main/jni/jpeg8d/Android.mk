LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libjpeg
LOCAL_SRC_FILES := lib/libjpeg.a

include $(PREBUILT_STATIC_LIBRARY)
