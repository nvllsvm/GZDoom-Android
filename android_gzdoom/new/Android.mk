
GZDOOM_TOP_PATH := $(call my-dir)

include $(GZDOOM_TOP_PATH)/Android_lzma.mk
include $(GZDOOM_TOP_PATH)/Android_gdtoa.mk
include $(GZDOOM_TOP_PATH)/Android_dumb.mk
include $(GZDOOM_TOP_PATH)/Android_gme.mk
include $(GZDOOM_TOP_PATH)/Android_bzip2.mk
#include $(GZDOOM_TOP_PATH)/Android_output_sdl.mk
include $(GZDOOM_TOP_PATH)/Android_src.mk




