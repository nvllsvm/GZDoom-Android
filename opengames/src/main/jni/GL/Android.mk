
LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)


LOCAL_MODULE    := nanogl

LOCAL_CFLAGS :=   -O2  

LOCAL_LDLIBS += -lGLESv1_CM -lEGL

LOCAL_LDLIBS += -llog

LOCAL_CFLAGS += -march=armv7-a -mfloat-abi=softfp
LOCAL_LDLIBS += -Wl,--fix-cortex-a8


#LOCAL_C_INCLUDES :=   $(TOP_DIR)  $(TOP_DIR)/prboom/ $(TOP_DIR)/SDL/include



LOCAL_SRC_FILES =  nano_gl.cpp qgl_nano.c gl_extra.c


LOCAL_LDLIBS :=  -ldl -llog

include $(BUILD_STATIC_LIBRARY)








