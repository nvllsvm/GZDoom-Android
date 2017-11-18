#!/bin/bash

if [[ $1 != 'noclean' ]]; then
    rm -rf doom/.externalNativeBuild
    rm -rf doom/build
    rm -rf doom/src/main/obj
    rm -rf doom/src/main/jni/gzdoom_android
    cp -r doom/src/main/jni/gzdoom doom/src/main/jni/gzdoom_android
    cp -r android_gzdoom/new/*mk doom/src/main/jni/gzdoom_android
    cp -r android_gzdoom/new/src/android doom/src/main/jni/gzdoom_android/src
    cp -r android_gzdoom/new/src/sound/fmodsound_studio.* doom/src/main/jni/gzdoom_android/src/sound
    cp -r android_gzdoom/new/src/gl/api doom/src/main/jni/gzdoom_android/src/gl
fi

patches=$(find android_gzdoom/patches -type f)

temp_file=/tmp/gzdoom_android_source
while read -r line; do
    patch_file=$line
    relative_name=$(echo $line | cut -c24-)
    target_file="doom/src/main/jni/gzdoom_android/$relative_name"
    source_file="doom/src/main/jni/gzdoom/$relative_name"

    cp $source_file $temp_file

    patch -N -p1 -r - $temp_file < $patch_file > /dev/null

    new_hash=$(sha256sum $temp_file | awk '{print $1}')
    old_hash=$(sha256sum $target_file | awk '{print $1}')
    if [[ $new_hash != $old_hash ]]; then
        echo Patching $relative_name
        mv $temp_file $target_file
    else
        rm $temp_file
    fi
done <<< $patches


NDK_PROJECT_PATH=doom/src/main ndk-build $@
