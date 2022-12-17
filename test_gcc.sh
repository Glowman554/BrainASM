set -ex

function compile {
	./brainasm.elf any-any-gcc $1 $2
	gcc $2 -o $3.o -c -no-pie
	gcc $3.o -o $3 -no-pie -static
}

make

mkdir -p out

compile test/hello.bf out/hello.c out/hello.elf
compile test/yapi.bf out/yapi.c out/yapi.elf
compile test/cellsize.bf out/cellsize.c out/cellsize.elf
compile test/triangle.bf out/triangle.c out/triangle.elf