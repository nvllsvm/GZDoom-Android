#!/bin/sh
diff -BZcr doom/src/main/jni/gzdoom/src/"$1" doom/src/main/jni/Doom/gzdoom_2/src/"$1" > android_gzdoom/patches/src/"$1"
