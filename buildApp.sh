#!/bin/sh
# ______________________________________________________________________________
#
#  Compile raylib project for Android
# ______________________________________________________________________________

# stop on error and display each command as it gets executed. Optional step but helpful in catching where errors happen if they do.
set -xe

# NOTE: If you excluded any ABIs in the previous steps, remove them from this list too
ABIS="arm64-v8a armeabi-v7a x86 x86_64"

BUILD_TOOLS=android/sdk/build-tools/29.0.3
TOOLCHAIN=android/ndk/toolchains/llvm/prebuilt/linux-x86_64
NATIVE_APP_GLUE=android/ndk/sources/android/native_app_glue

FLAGS="-ffunction-sections -funwind-tables -fstack-protector-strong -fPIC -Wall \
	-Wformat -Werror=format-security -no-canonical-prefixes \
	-DANDROID -DPLATFORM_ANDROID -D__ANDROID_API__=29"

INCLUDES="-I. -Iinclude -I../include -I$NATIVE_APP_GLUE -I$TOOLCHAIN/sysroot/usr/include"

# Copy icons
cp assets/icon_ldpi.png android/build/res/drawable-ldpi/icon.png
cp assets/icon_mdpi.png android/build/res/drawable-mdpi/icon.png
cp assets/icon_hdpi.png android/build/res/drawable-hdpi/icon.png
cp assets/icon_xhdpi.png android/build/res/drawable-xhdpi/icon.png

# Copy other assets
cp -r assets/* android/build/assets

# ______________________________________________________________________________
#
#  Compile
# ______________________________________________________________________________
#
for ABI in $ABIS; do
	case "$ABI" in
		"armeabi-v7a")
			CCTYPE="armv7a-linux-androideabi"
			ARCH="arm"
			LIBPATH="arm-linux-androideabi"
			ABI_FLAGS="-std=c99 -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16"
			;;

		"arm64-v8a")
			CCTYPE="aarch64-linux-android"
			ARCH="aarch64"
			LIBPATH="aarch64-linux-android"
			ABI_FLAGS="-std=c99 -target aarch64 -mfix-cortex-a53-835769"
			;;

		"x86")
			CCTYPE="i686-linux-android"
			ARCH="i386"
			LIBPATH="i686-linux-android"
			ABI_FLAGS=""
			;;

		"x86_64")
			CCTYPE="x86_64-linux-android"
			ARCH="x86_64"
			LIBPATH="x86_64-linux-android"
			ABI_FLAGS=""
			;;
	esac
	CC="$TOOLCHAIN/bin/${CCTYPE}29-clang"

	# Compile native app glue
	# .c -> .o
	$CC -c $NATIVE_APP_GLUE/android_native_app_glue.c -o $NATIVE_APP_GLUE/native_app_glue.o \
		$INCLUDES -I$TOOLCHAIN/sysroot/usr/include/$CCTYPE $FLAGS $ABI_FLAGS

	# .o -> .a
	$TOOLCHAIN/bin/llvm-ar rcs lib/$ABI/libnative_app_glue.a $NATIVE_APP_GLUE/native_app_glue.o

	# Compile project
	for file in src/*.c; do
		$CC -DPHONE -c $file -o "$file".o \
			$INCLUDES -I$TOOLCHAIN/sysroot/usr/include/$CCTYPE $FLAGS $ABI_FLAGS
	done

	for file in src/**/*.c; do
		$CC -DPHONE -c $file -o "$file".o \
			$INCLUDES -I$TOOLCHAIN/sysroot/usr/include/$CCTYPE $FLAGS $ABI_FLAGS
	done
        # Link the project with toolchain specific linker to avoid relocations issue
	$TOOLCHAIN/bin/ld.lld src/*.o src/**/*.o -o android/build/lib/$ABI/libmain.so -shared \
		--exclude-libs libatomic.a --build-id \
		-z noexecstack -z relro -z now \
		--warn-shared-textrel --fatal-warnings -u ANativeActivity_onCreate \
		-L$TOOLCHAIN/sysroot/usr/lib/$LIBPATH/29 \
		-L$TOOLCHAIN/lib/clang/17/lib/linux/$ARCH \
		-L. -Landroid/build/obj -Llib/$ABI \
    -L/usr/lib/ \
		-lraylib -lnative_app_glue -llog -landroid -lEGL -lGLESv2 -lOpenSLES -lc -lm -ldl
done

# ______________________________________________________________________________
#
#  Build APK
# ______________________________________________________________________________
#
$BUILD_TOOLS/aapt package -f -m \
	-S android/build/res -J android/build/src -M android/build/AndroidManifest.xml \
	-I android/sdk/platforms/android-29/android.jar

# Compile NativeLoader.java
javac -verbose -source 1.8 -target 1.8 -d android/build/obj \
	-bootclasspath jre/lib/rt.jar \
	-classpath android/sdk/platforms/android-29/android.jar:android/build/obj \
	-sourcepath src android/build/src/com/raylib/game/R.java \
	android/build/src/com/raylib/game/NativeLoader.java

$BUILD_TOOLS/dx --verbose --dex --output=android/build/dex/classes.dex android/build/obj

# Add resources and assets to APK

echo "000" | cat - > ./assets/savedata/highScore.sv

$BUILD_TOOLS/aapt package -f \
	-M android/build/AndroidManifest.xml -S android/build/res -A assets \
	-I android/sdk/platforms/android-29/android.jar -F game.apk android/build/dex

# Add libraries to APK
cd android/build
for ABI in $ABIS; do
	../../$BUILD_TOOLS/aapt add ../../game.apk lib/$ABI/libmain.so
done
cd ../..

# Zipalign APK and sign
# NOTE: If you changed the storepass and keypass in the setup process, change them here too
$BUILD_TOOLS/zipalign -f 4 game.apk game.final.apk
mv -f game.final.apk game.apk

# Install apksigner with `sudo apt install apksigner`
android/sdk/build-tools/29.0.3/apksigner sign  --ks android/raylib.keystore --out my-app-release.apk --ks-pass pass:raylib game.apk
mv my-app-release.apk game.apk

rm src/*.o src/**/*.o

# Install to device or emulator
# android/sdk/platform-tools/adb install -r game.apk
