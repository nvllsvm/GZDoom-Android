#!/bin/sh
diff -BZcr doom/src/main/jni/gzdoom/src/"$1" doom/src/main/jni/gzdoom_android/src/"$1" > android_gzdoom/patches/src/"$1"
