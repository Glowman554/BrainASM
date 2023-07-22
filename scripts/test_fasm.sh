set -ex

function compile {
	./brainasm.elf -t x86-microos-fasm -o $2 $1
	fasm $2
}

make

mkdir -p out

compile test/hello.bf out/hello.asm
compile test/yapi.bf out/yapi.asm
compile test/cellsize.bf out/cellsize.asm 
compile test/triangle.bf out/triangle.asm 
compile test/echo.bf out/echo.asm 