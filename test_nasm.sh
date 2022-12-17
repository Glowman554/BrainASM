set -ex

function compile {
	./brainasm.elf x86_64-linux-nasm $1 $2
	nasm $2 -o $3.o -f elf64
	gcc $3.o -o $3 -no-pie -static
}

make

compile test/hello.bf hello.asm hello.elf
compile test/yapi.bf yapi.asm yapi.elf
compile test/cellsize.bf cellsize.asm cellsize.elf
compile test/triangle.bf triangle.asm triangle.elf