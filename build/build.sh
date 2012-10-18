#!/bin/bash

ARCH=arm
CROSS_COMPILE=arm-linux-
CC="${CROSS_COMPILE}"gcc

TOP=`pwd`
if [[ -d "$TOP/$dir_busybox" ]]; then
	TOP=$TOP
else
	TOP="$TOP/.."
fi

dir_busybox="$TOP/busybox-1.19.3"
dir_freetype="$TOP/freetype-2.3.9"
dir_jpeg="$TOP/jpeg-6b"
dir_png="$TOP/libpng-1.2.19"
dir_zlib="$TOP/zlib-1.2.3"
dir_alsalib="$TOP/alsa-lib"
dir_alsautils="$TOP/alsa-utils"
dir_dfb="$TOP/DirectFB-1.1.1"
dir_dfbexample="$TOP/DirectFB-examples-0.9.23"
dir_dfbturtorials="$TOP/DFBTutorials-0.5.0"
dir_build="$TOP/build"

TARGET="$TOP/ramdisk"

EDIR="-1000"

all_dirs="$dir_busybox $dir_freetype $dir_jpeg $dir_png $dir_zlib \
	$dir_alsalib $dir_alsautils $dir_dfb $dir_dfbexample $dir_dfbturtorials"

function do_distclean ()
{
	for dir in $all_dirs
	do
		cd $dir
		make distclean
	done
	rm -rf $TARGET
}

case "$1" in 
	distclean )
		do_distclean
		exit
		;;
esac

## check environment
for dir in $all_dirs
do
	echo $dir
	if [[ ! -d "$dir" ]]; then
		echo "error $dir not found"
		exit $EDIR
	fi
done

## function build_busybox
echo "build busybox"
cp "$dir_build/busybox.config" "$dir_busybox/.config"
cd $dir_busybox

# 确认编译成功
make
make install

rm -rf "$TARGET"
mv _install "$TARGET"
cd "$TARGET"
mkdir -p lib dev proc etc etc/init.d mnt usr bin sbin tmp sys

# 复制动态库
tmp=`$CC -print-file-name=libm.so`
path_lib="`dirname $tmp`/../../lib"
cp -a $path_lib/ld*   "$TARGET/lib"
cp -a $path_lib/libm* "$TARGET/lib"
cp -a $path_lib/libc* "$TARGET/lib"

cp "$dir_build/busybox.fstab" "$TARGET/etc/fstab"
cp "$dir_build/busybox.rcS" "$TARGET/etc/rcS"

###############

export PREFIX="$TARGET"
export LDFLAGS="-L$PREFIX/lib -Wl,-rpath,$PREFIX/lib"
export CFLAGS="-g -I$PREFIX/include"
export PKG_CONFIG_PATH="$PREFIX/lib/pkgconfig"

## freetype
cd $dir_freetype
./configure --host=arm-linux --prefix=$PREFIX
make
make install


## jpeg
cd $dir_jpeg
./configure --host=arm-linux --prefix=$PREFIX --enable-shared --enable-static
make CC=$CC
make install-lib


## zlib
cd $dir_zlib
CC=$CC ./configure --prefix=$PREFIX --shared
make
make install

## libpng
cd $dir_png
./configure --host=arm-linux --prefix=$PREFIX
make
make install


## DirectFB
cd $dir_dfb
./configure 	--host=arm-linux \
		--prefix=$PREFIX \
		--with-gfxdrivers=none \
		--with-inputdrivers=linuxinput \
		--enable-png \
		--enable-jpeg \
		--disable-tiff \
		--enable-zlib \
		--enable-sdl=no \
		--enable-gif=no \
		--disable-video4linux \
		--disable-x11 \
		--enable-unique \
		--sysconfdir=/etc

make
make install


## Directfb example
cd $dir_dfbexample
./configure --host=arm-linux --prefix=$PREFIX
make
make install

cd $TOP
exit

