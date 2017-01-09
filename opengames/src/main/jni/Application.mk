
ifeq ($(G),Q2)
    APP_ABI      := armeabi-v7a
    NDK_TOOLCHAIN_VERSION=4.9
else ifeq ($(G),HEX2)
    #Hexen 2 needs old compiler, otherwise crash whenloading game the second time, not sure why
else ifeq ($(G),WOLF)
    APP_ABI      := armeabi-v7a
    NDK_TOOLCHAIN_VERSION=4.9
else ifeq ($(G),HL)
    APP_ABI      := armeabi-v7a
    NDK_TOOLCHAIN_VERSION=4.9
else ifeq ($(G),BS)
    APP_ABI      := armeabi-v7a
    NDK_TOOLCHAIN_VERSION=4.9
    APP_CPPFLAGS += -std=c++11
else ifeq ($(G),JK2)
    APP_ABI      := armeabi-v7a  x86
    NDK_TOOLCHAIN_VERSION=4.9
else
    APP_ABI      := armeabi-v7a
    NDK_TOOLCHAIN_VERSION=4.9
endif

APP_STL := gnustl_static
#APP_STL  := stlport_static
#APP_MODULES := libsigc++  
