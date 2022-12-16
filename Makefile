CC = clang
CFLAGS = -Isrc/include -g

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

CSRC = $(call rwildcard,./src,*.c)
OBJS = $(patsubst %.c, %.o, $(CSRC))

BFSRC = $(call rwildcard,./test,*.bf)
BFOBJS = $(patsubst %.bf, %.elf, $(BFSRC))

OUTPUT = brainasm.elf
INSTALL = /usr/bin/brainasm

all: $(OUTPUT) $(BFOBJS)

$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# %.elf: %.bf
#	./$(OUTPUT) x86_64-linux-nasm $< /tmp/bfasm.asm
#	nasm /tmp/bfasm.asm -o /tmp/bfasm.o -f elf64
#	gcc /tmp/bfasm.o -no-pie -o $@

# %.elf: %.bf
#	./$(OUTPUT) x86_64-linux-gas $< /tmp/bfasm.S
#	gcc /tmp/bfasm.S -c -o /tmp/bfasm.o
#	gcc /tmp/bfasm.o -no-pie -o $@

# %.elf: %.bf
# 	./$(OUTPUT) aarch64-linux-gas $< /tmp/bfasm.S
# 	gcc -g /tmp/bfasm.S -c -o /tmp/bfasm.o
# 	gcc -g /tmp/bfasm.o -no-pie -o $@

%.elf: %.bf
	./$(OUTPUT) phoenix-none-as $< /tmp/bfasm.S
	cp /tmp/bfasm.S out.S
	phoenix-as /tmp/bfasm.S $@

install: $(OUTPUT)
	sudo cp $< $(INSTALL)

clean:
	rm $(OBJS) $(BFOBJS) $(OUTPUT)
