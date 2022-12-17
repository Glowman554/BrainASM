set -ex

function compile {
	./brainasm.elf x86_64-linux-gas $1 $2
	gcc $2 -o $3.o -c -no-pie
	gcc $3.o -o $3 -no-pie -static
}

make

mkdir -p out

compile test/hello.bf out/hello.S out/hello.elf
compile test/yapi.bf out/yapi.S out/yapi.elf
compile test/cellsize.bf out/cellsize.S out/cellsize.elf
compile test/triangle.bf out/triangle.S out/triangle.elf