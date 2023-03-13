set -ex

function compile {
	./brainasm.elf -t any-any-firestorm -o $2 $1
}

make

mkdir -p out

compile test/hello.bf out/hello.fl
compile test/yapi.bf out/yapi.fl
compile test/cellsize.bf out/cellsize.fl
compile test/triangle.bf out/triangle.fl