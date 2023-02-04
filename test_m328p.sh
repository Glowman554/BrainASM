set -ex

function compile {
	./brainasm.elf -t avr-m328p-avra -o $2 $1
	avra -fI $2
}

make

mkdir -p out

compile test/hello.bf out/hello.asm
compile test/triangle.bf out/triangle.asm
compile test/yapi.bf out/yapi.asm
compile test/cellsize.bf out/cellsize.asm
compile test/echo.bf out/echo.asm

# avra -fI out/hello.asm
sudo avrdude -c arduino -p m328p -b 115200 -P /dev/ttyACM0 -U flash:w:out/triangle.hex
screen /dev/ttyACM0 9600
