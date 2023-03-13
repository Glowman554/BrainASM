set -ex

function compile {
	./brainasm.elf -t any-any-firestorm -o $2 $1
	flc $2 -o $3
}

make

mkdir -p out

compile test/hello.bf out/hello.fl out/hello.elf
compile test/yapi.bf out/yapi.fl out/yapi.elf
compile test/cellsize.bf out/cellsize.fl out/cellsize.elf
compile test/triangle.bf out/triangle.fl out/triangle.elf
