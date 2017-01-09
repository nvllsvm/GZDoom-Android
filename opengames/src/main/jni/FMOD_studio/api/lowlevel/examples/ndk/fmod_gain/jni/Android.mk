LOCAL_PATH := $(call my-dir)

#
# Plugin Static Library
# 
include $(CLEAR_VARS)

LOCAL_MODULE            := fmod_gain
LOCAL_SRC_FILES         := \
                           ../../../plugins/fmod_gain.cpp
LOCAL_C_INCLUDES        := $(LOCAL_PATH)/../../../../../lowlevel/inc

include $(BUILD_SHARED_LIBRARY)
