LOCAL_PATH := $(call my-dir)

#
# FMOD Shared Library
# 
include $(CLEAR_VARS)

LOCAL_MODULE            := fmod
LOCAL_SRC_FILES         := ../../../../../lowlevel/lib/$(TARGET_ARCH_ABI)/libfmodL.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../../../lowlevel/inc

include $(PREBUILT_SHARED_LIBRARY)

#
# FMOD Studio Shared Library
# 
include $(CLEAR_VARS)

LOCAL_MODULE            := fmodstudio
LOCAL_SRC_FILES         := ../../../../../studio/lib/$(TARGET_ARCH_ABI)/libfmodstudioL.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../../../studio/inc

include $(PREBUILT_SHARED_LIBRARY)

#
# Example Library
#
include $(CLEAR_VARS)

LOCAL_MODULE            := example
LOCAL_SRC_FILES         := ../../../common_platform.cpp \
                           ../../../common.cpp \
                           ../../../recording_playback.cpp
LOCAL_C_INCLUDES        := $(LOCAL_PATH)/../../../
LOCAL_SHARED_LIBRARIES  := fmod fmodstudio

include $(BUILD_SHARED_LIBRARY)
