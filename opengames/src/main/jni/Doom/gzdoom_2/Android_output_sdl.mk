LOCAL_PATH := $(call my-dir)/output_sdl


include $(CLEAR_VARS)


LOCAL_MODULE    := output_sdl


LOCAL_CPPFLAGS :=  

LOCAL_LDLIBS += -llog


	
LOCAL_C_INCLUDES := \
$(TOP_DIR)/ \
$(TOP_DIR)/SDL/include  \
$(TOP_DIR)/GL \
 $(GZDOOM_TOP_PATH)/src/android  \
 $(GZDOOM_TOP_PATH)/game-music-emu/ \
 $(GZDOOM_TOP_PATH)/gdtoa \
 $(GZDOOM_TOP_PATH)/lzma/C \
 $(GZDOOM_TOP_PATH)/bzip2 \
 $(GZDOOM_TOP_PATH)/src/sound \
 $(GZDOOM_TOP_PATH)/src/textures \
 $(GZDOOM_TOP_PATH)/src/thingdef \
 $(GZDOOM_TOP_PATH)/src/sdl \
 $(GZDOOM_TOP_PATH)/src/g_doom \
 $(GZDOOM_TOP_PATH)/src/g_heretic \
 $(GZDOOM_TOP_PATH)/src/g_hexen \
 $(GZDOOM_TOP_PATH)/src/g_raven \
 $(GZDOOM_TOP_PATH)/src/g_strife \
 $(GZDOOM_TOP_PATH)/src/g_shared \
$(TOP_DIR)/openal/include \
$(TOP_DIR)/FMOD_studio/api/lowlevel/inc \
$(TOP_DIR)/prboom/MUSIC/include/fluidsynth/ \
$(TOP_DIR)/libpng \
$(TOP_DIR)/jpeg8d \
$(TOP_DIR)/TouchControls  \
$(TOP_DIR)/TinyXML  



LOCAL_SRC_FILES = output_sdl.c
	
	# -lGLESv1_CM 
LOCAL_LDLIBS :=  -llog -lz
LOCAL_STATIC_LIBRARIES := 
LOCAL_SHARED_LIBRARIES := SDL SDL_mixer 

include $(BUILD_SHARED_LIBRARY)








