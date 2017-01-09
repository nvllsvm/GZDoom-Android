
LOCAL_PATH := $(call my-dir)/dumb


include $(CLEAR_VARS)


LOCAL_MODULE    := dumb_dev

LOCAL_CFLAGS :=   -O2  -DNEED_ITOA

LOCAL_LDLIBS += -llog

LOCAL_CFLAGS += -march=armv7-a -mfloat-abi=softfp
LOCAL_LDLIBS += -Wl,--fix-cortex-a8

LOCAL_C_INCLUDES :=   $(LOCAL_PATH)/include/



LOCAL_SRC_FILES =  	\
	  src/core/atexit.c \
    src/core/duhlen.c \
    src/core/duhtag.c \
    src/core/dumbfile.c \
    src/core/loadduh.c \
    src/core/makeduh.c \
    src/core/rawsig.c \
    src/core/readduh.c \
    src/core/register.c \
    src/core/rendduh.c \
    src/core/rendsig.c \
    src/core/unload.c \
    src/helpers/barray.c \
    src/helpers/blip_buf.c \
    src/helpers/clickrem.c \
    src/helpers/memfile.c \
    src/helpers/resample.c \
    src/helpers/riff.c \
    src/helpers/sampbuf.c \
    src/helpers/silence.c \
    src/helpers/stdfile.c \
    src/it/filter.cpp \
    src/it/itload.c \
    src/it/itload2.c \
    src/it/itmisc.c \
    src/it/itorder.c \
    src/it/itread.c \
    src/it/itread2.c \
    src/it/itrender.c \
    src/it/itunload.c \
    src/it/load669.c \
    src/it/load6692.c \
    src/it/loadasy.c \
    src/it/loadasy2.c \
    src/it/loadmod.c \
    src/it/loadmod2.c \
    src/it/loadmtm.c \
    src/it/loadmtm2.c \
    src/it/loadokt.c \
    src/it/loadokt2.c \
    src/it/loadoldpsm.c \
    src/it/loadoldpsm2.c \
    src/it/loadpsm.c \
    src/it/loadpsm2.c \
    src/it/loadptm.c \
    src/it/loadptm2.c \
    src/it/loadriff.c \
    src/it/loadriff2.c \
    src/it/loads3m.c \
    src/it/loads3m2.c \
    src/it/loadstm.c \
    src/it/loadstm2.c \
    src/it/loadxm.c \
    src/it/loadxm2.c \
    src/it/ptmeffect.c \
    src/it/read669.c \
    src/it/read6692.c \
    src/it/readam.c \
    src/it/readasy.c \
    src/it/readdsmf.c \
    src/it/readmod.c \
    src/it/readmod2.c \
    src/it/readmtm.c \
    src/it/readokt.c \
    src/it/readokt2.c \
    src/it/readoldpsm.c \
    src/it/readpsm.c \
    src/it/readptm.c \
    src/it/readriff.c \
    src/it/reads3m.c \
    src/it/reads3m2.c \
    src/it/readstm.c \
    src/it/readstm2.c \
    src/it/readxm.c \
    src/it/readxm2.c \
    src/it/xmeffect.c \

LOCAL_LDLIBS :=  -ldl -llog

include $(BUILD_STATIC_LIBRARY)








