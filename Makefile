CC = clang
CFLAGS = -Isrc/include -g

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

CSRC = $(call rwildcard,./src,*.c)
OBJS = $(patsubst %.c, %.o, $(CSRC))

BFSRC = $(call rwildcard,./test,*.bf)
BFOBJS = $(patsubst %.bf, %.elf, $(BFSRC))

all: bfasm.elf $(BFOBJS)

bfasm.elf: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.elf: %.bf
	./bfasm.elf x86_64-linux-nasm $< /tmp/bfasm.asm
	nasm /tmp/bfasm.asm -o /tmp/bfasm.o -f elf64
	gcc /tmp/bfasm.o -no-pie -o $@

# %.elf: %.bf
# 	./bfasm.elf x86_64-linux-gas $< /tmp/bfasm.S
# 	gcc /tmp/bfasm.S -c -o /tmp/bfasm.o
# 	gcc /tmp/bfasm.o -no-pie -o $@

install: bfasm.elf
	sudo cp $< /usr/bin/bfasm

clean:
	rm $(OBJS) $(BFOBJS) bfasm.elf