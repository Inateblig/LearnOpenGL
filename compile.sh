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

srcf=$(find "$source" -maxdepth 1 -name "*.c")
clang $srcf glad/src/gl.c -Iglad/include -lglfw -lm -o "$builddir/main.exe"
