set -ex

function compile {
	./brainasm.elf -t phoenixv2-none-as -o $2 $1
	phoenixv2-as $2 $3
}

make

mkdir -p out

compile test/hello.bf out/hello.asm out/hello.bin
compile test/yapi.bf out/yapi.asm out/yapi.bin
compile test/cellsize.bf out/cellsize.asm out/cellsize.bin
compile test/triangle.bf out/triangle.asm out/triangle.bin
compile test/test.bf out/test.asm out/test.bin