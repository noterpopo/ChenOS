BUILD_DIR = ./build
ENTRY_POINT = 0xc0001500
AS = nasm
CC = gcc
LD = ld
LIB = -I lib/ -I lib/kernel/ -I lib/user/ -I kernel/ -I device/ -I thread/
ASFLAG = -f elf
CFLAGS = -m32 -ffreestanding -nostdlib -mno-red-zone -Wall $(LIB) -c -W -Wstrict-prototypes -Wmissing-prototypes
LDFLAGS = -m elf_i386 -Ttext $(ENTRY_POINT) -e main -Map $(BUILD_DIR)/kernel.map
OBJS =  $(BUILD_DIR)/main.o \
		$(BUILD_DIR)/init.o \
		$(BUILD_DIR)/interrupt.o \
		$(BUILD_DIR)/timer.o \
		$(BUILD_DIR)/kernel.o \
		$(BUILD_DIR)/switch.o \
		$(BUILD_DIR)/print.o \
		$(BUILD_DIR)/debug.o \
		$(BUILD_DIR)/debug.o \
		$(BUILD_DIR)/string.o \
		$(BUILD_DIR)/memory.o \
		$(BUILD_DIR)/bitmap.o \
		$(BUILD_DIR)/thread.o \
		$(BUILD_DIR)/list.o


$(BUILD_DIR)/main.o: kernel/main.c lib/kernel/print.h \
				lib/stdint.h kernel/init.h thread/thread.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/init.o: kernel/init.c kernel/init.h lib/kernel/print.h \
				lib/stdint.h kernel/interrupt.h device/timer.h kernel/memory.h thread/thread.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/interrupt.o: kernel/interrupt.c kernel/interrupt.h lib/kernel/print.h \
				lib/stdint.h kernel/global.h lib/kernel/io.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/timer.o: device/timer.c device/timer.h lib/kernel/print.h \
				lib/stdint.h lib/kernel/io.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/debug.o: kernel/debug.c kernel/debug.h lib/kernel/print.h \
				lib/stdint.h kernel/interrupt.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/string.o: lib/string.c lib/string.h kernel/debug.h \
				lib/stdint.h kernel/global.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/bitmap.o: lib/kernel/bitmap.c lib/kernel/bitmap.h lib/string.h kernel/debug.h lib/kernel/print.h \
				lib/stdint.h kernel/interrupt.h kernel/global.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/memory.o: kernel/memory.c kernel/memory.h lib/kernel/bitmap.h lib/kernel/print.h \
				lib/string.h lib/stdint.h kernel/debug.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/thread.o: thread/thread.c  thread/thread.h lib/kernel/list.h lib/kernel/bitmap.c lib/kernel/bitmap.h lib/string.h kernel/debug.h lib/kernel/print.h \
				lib/stdint.h kernel/interrupt.h kernel/global.h kernel/memory.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/list.o: lib/kernel/list.c  lib/kernel/list.h lib/kernel/bitmap.c lib/kernel/bitmap.h lib/string.h kernel/debug.h lib/kernel/print.h \
				lib/stdint.h kernel/interrupt.h kernel/global.h kernel/memory.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/kernel.o: kernel/kernel.S
				$(AS) $(ASFLAG) $< -o $@

$(BUILD_DIR)/print.o: lib/kernel/print.S
				$(AS) $(ASFLAG) $< -o $@

$(BUILD_DIR)/switch.o: thread/switch.S
				$(AS) $(ASFLAG) $< -o $@

$(BUILD_DIR)/kernel.bin: $(OBJS)
				$(LD) $(LDFLAGS) $^ -o $@

.PHONY: mkdir clean all

mkdir: 
	if [[ ! -d $(BUILD_DIR) ]];then mkdir $(BUILD_DIR);fi

clean:
	cd $(BUILD_DIR) && rm -f ./*

build: $(BUILD_DIR)/kernel.bin

all: mkdir build


