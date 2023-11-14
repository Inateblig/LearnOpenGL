#!/bin/sh

if [ $# -lt 2 ]
then
	echo "Not enough arguments. Requires atleast 2." >&2
	echo "Usage: $0 <source> <output>" >&2
	exit 1
fi

source=$1
output=$2
builddir=$output

mkdir -p "$builddir"

extras="$source/extra_compile_options"
if [ -f $extras ]
then
	echo "Has extras at: $extras"
	. $extras
fi

srcf=$(find "$source" -maxdepth 1 -name "*.c")
clang $srcf $extra_libs glad/src/gl.c -Iglad/include -Ilibs/ $extra_includes -lglfw $extra_links -o "$builddir/main.exe"
