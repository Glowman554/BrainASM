set -ex

function compile {
	./bfasm.elf $1 $2
	nasm $2 -o $3.o -f elf64
	gcc $3.o -o $3 -no-pie
}

make

compile hello.bf hello.asm hello.elf
compile yapi.bf yapi.asm yapi.elf
compile cellsize.bf cellsize.asm cellsize.elf
compile triangle.bf triangle.asm triangle.elf