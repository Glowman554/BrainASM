set -ex

function compile {
	./brainasm.elf -t riscv-esp32-gas -o $2 $1
}

make

mkdir -p out

compile test/hello.bf out/hello.S
compile test/triangle.bf out/triangle.S
compile test/yapi.bf out/yapi.S
compile test/cellsize.bf out/cellsize.S
compile test/echo.bf out/echo.S
cp out/hello.S ./esp32/main/main.s