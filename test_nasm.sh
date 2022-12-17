set -ex

function compile {
	./brainasm.elf x86_64-linux-nasm $1 $2
	nasm $2 -o $3.o -f elf64
	gcc $3.o -o $3 -no-pie -static
}

make

mkdir -p out

compile test/hello.bf out/hello.asm out/hello.elf
compile test/yapi.bf out/yapi.asm out/yapi.elf
compile test/cellsize.bf out/cellsize.asm out/cellsize.elf
compile test/triangle.bf out/triangle.asm out/triangle.elf