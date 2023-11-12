#!/bin/sh

if [ $# -lt 2 ]
then
	echo "Not enough arguments. Requires atleast 2." >&2
	echo "Usage: $0 <Vmajor> <Vminor>" >&2
	exit 1
fi

Vmajor=$1
Vminor=$2

shift 2

printf '%s\n' "$Vmajor" "$Vminor" |
awk '/^[0-9]+$/ {
	next
} {
	print $0 ": not a valid number"
	exit 1
}' || exit 1

dirs=$(find src/ -maxdepth 1 -type d -name "$Vmajor.$Vminor*")

if [ -z "$dirs" ]
then
	echo "ERROR: No matching results."
	exit 1
fi

case "$(printf '%s\n' "$dirs" | wc -l)" in
(1)
	echo "1 matching result."
;;
(*)
	echo "ERROR: More than 1 matching result."
	echo "Results are:"
	printf '%s\n' "$dirs"
	exit 1
;;
esac

source=$dirs
builddir="build/${dirs#src/}"

echo "Compiling '"$dirs"' to '$builddir' in process..."
./compile.sh "$source" "$builddir" || exit
echo "Done compiling."

shddir=$(find $source -maxdepth 1 -type d -name "shaders")
if [ -n "$shddir" ] && ! [ -d "$builddir/data/shaders" ]
then
	mkdir -p "$builddir/data"
	ln -s "../../../$shddir" "$builddir/data/"
fi

if [ -n "data/textures" ] && ! [ -d "$builddir/data/textures" ]
then
	ln -s "../../../data/textures" "$builddir/data/"
fi

if [ $# -gt 0 ] && [ "$1" = run ]
then
	echo "Running.."
	"./$builddir/main.exe"
fi

echo "Finished."
