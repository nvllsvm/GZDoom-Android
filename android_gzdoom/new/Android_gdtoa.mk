
LOCAL_PATH := $(call my-dir)/gdtoa


include $(CLEAR_VARS)


LOCAL_MODULE    := gdtoa_dev

LOCAL_CFLAGS :=   -O2  

LOCAL_LDLIBS += -llog

LOCAL_CFLAGS += -march=armv7-a -mfloat-abi=softfp -DINFNAN_CHECK -DMULTIPLE_THREADS
LOCAL_LDLIBS += -Wl,--fix-cortex-a8

LOCAL_C_INCLUDES :=   . $(GZDOOM_TOP_PATH)/src/android/extrafiles 



LOCAL_SRC_FILES =  	\
	dmisc.c \
	dtoa.c \
	g_Qfmt.c \
	g__fmt.c \
	g_ddfmt.c \
	g_dfmt.c \
	g_ffmt.c \
	g_xLfmt.c \
	g_xfmt.c \
	gdtoa.c \
	gethex.c \
	gmisc.c \
	hd_init.c \
	hexnan.c \
	misc.c \
	smisc.c \
	strtoIQ.c \
	strtoId.c \
	strtoIdd.c \
	strtoIf.c \
	strtoIg.c \
	strtoIx.c \
	strtoIxL.c \
	strtod.c \
	strtodI.c \
	strtodg.c \
	strtopQ.c \
	strtopd.c \
	strtopdd.c \
	strtopf.c \
	strtopx.c \
	strtopxL.c \
	strtorQ.c \
	strtord.c \
	strtordd.c \
	strtorf.c \
	strtorx.c \
	strtorxL.c \
	sum.c \
	ulp.c \

#strtof.c \

LOCAL_LDLIBS :=  -ldl -llog

include $(BUILD_STATIC_LIBRARY)








