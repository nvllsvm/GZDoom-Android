#!/bin/sh
diff -cr doom/src/main/jni/gzdoom/"$1" doom/src/main/jni/Doom/gzdoom_2/"$1" > android_gzdoom/patches/"$1"
