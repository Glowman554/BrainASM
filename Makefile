CC = clang
CFLAGS = -Isrc/include -g

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

CSRC = $(call rwildcard,./src,*.c)
OBJS = $(patsubst %.c, %.o, $(CSRC))

OUTPUT = brainasm.elf
INSTALL = /usr/bin/brainasm

all: $(OUTPUT) $(BFOBJS)

$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: $(OUTPUT)
	sudo cp $< $(INSTALL)

clean:
	rm $(OBJS) $(OUTPUT)
