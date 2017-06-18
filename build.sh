#!/bin/sh
rm -r doom/src/main/jni/Doom/gzdoom_2
cp -r doom/src/main/jni/gzdoom doom/src/main/jni/Doom/gzdoom_2
cp -r android_gzdoom/new/*mk doom/src/main/jni/Doom/gzdoom_2
cp -r android_gzdoom/new/src/android doom/src/main/jni/Doom/gzdoom_2/src
cp -r android_gzdoom/new/src/sound/fmodsound_studio.* doom/src/main/jni/Doom/gzdoom_2/src/sound
cp -r android_gzdoom/new/src/gl_v1 doom/src/main/jni/Doom/gzdoom_2/src

patches=$(find android_gzdoom/patches -type f)

while read -r line; do
    patch_file=$line
    target_file="doom/src/main/jni/Doom/gzdoom_2/$(echo $line | cut -c24-)"
    patch -p1 $target_file < $patch_file
done <<< $patches

rm -r doom/{.externalNativeBuild/,build}
NDK_PROJECT_PATH=doom/src/main ndk-build
