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
#sudo avrdude -c stk500v1 -p m328p -b 19200 -P /dev/ttyUSB0 -U flash:w:out/hello.hex
#screen /dev/ttyACM0 9600
