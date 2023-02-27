set -ex

function compile {
	./brainasm.elf -t avr-t85-avra -o $2 $1
	avra -fI $2
}

make

mkdir -p out

compile test/hello.bf out/hello.asm
compile test/triangle.bf out/triangle.asm
compile test/yapi.bf out/yapi.asm
compile test/cellsize.bf out/cellsize.asm
compile test/echo.bf out/echo.asm

micronucleus --run out/triangle.hex
