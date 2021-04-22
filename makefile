BUILD_DIR = ./build
ENTRY_POINT = 0xc0001500
AS = nasm
CC = gcc
LD = ld
LIB = -I lib/ -I lib/kernel/ -I lib/user/ -I kernel/ -I device/ -I thread/ -I userproc/ -I fs/
ASFLAG = -f elf
CFLAGS = -m32 -ffreestanding -nostdlib -mno-red-zone -Wall $(LIB) -c -W -Wstrict-prototypes -Wmissing-prototypes -fno-stack-protector
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
		$(BUILD_DIR)/sync.o \
		$(BUILD_DIR)/thread.o \
		$(BUILD_DIR)/list.o \
		$(BUILD_DIR)/console.o \
		$(BUILD_DIR)/keyboard.o \
		$(BUILD_DIR)/ioqueue.o \
		$(BUILD_DIR)/tss.o \
		$(BUILD_DIR)/process.o \
		$(BUILD_DIR)/syscall.o \
		$(BUILD_DIR)/syscall-init.o \
		$(BUILD_DIR)/stdio.o \
		$(BUILD_DIR)/stdio-kernel.o \
		$(BUILD_DIR)/ide.o \
		$(BUILD_DIR)/fs.o


$(BUILD_DIR)/main.o: kernel/main.c lib/kernel/print.h lib/user/syscall.h device/ide.h\
				lib/stdint.h kernel/init.h thread/thread.h device/console.h userproc/process.h userproc/syscall_init.h lib/stdio.h kernel/memory.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/init.o: kernel/init.c kernel/init.h lib/kernel/print.h userproc/tss.h device/ide.h\
				lib/stdint.h kernel/interrupt.h device/timer.h kernel/memory.h thread/thread.h device/console.h device/keyboard.h userproc/syscall_init.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/interrupt.o: kernel/interrupt.c kernel/interrupt.h lib/kernel/print.h \
				lib/stdint.h kernel/global.h lib/kernel/io.h kernel/debug.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/timer.o: device/timer.c device/timer.h lib/kernel/print.h \
				lib/stdint.h lib/kernel/io.h lib/stdint.h
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
				lib/string.h lib/stdint.h kernel/debug.h lib/kernel/list.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/thread.o: thread/thread.c  thread/thread.h lib/kernel/list.h lib/kernel/bitmap.c lib/kernel/bitmap.h lib/string.h kernel/debug.h lib/kernel/print.h \
				lib/stdint.h kernel/interrupt.h kernel/global.h kernel/memory.h userproc/process.h thread/sync.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/list.o: lib/kernel/list.c lib/kernel/list.h lib/kernel/bitmap.c lib/kernel/bitmap.h lib/string.h kernel/debug.h lib/kernel/print.h \
				lib/stdint.h kernel/interrupt.h kernel/global.h kernel/memory.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/sync.o: thread/sync.c thread/sync.h lib/kernel/list.h thread/thread.h lib/kernel/list.h lib/string.h kernel/debug.h lib/kernel/print.h \
				lib/stdint.h kernel/interrupt.h kernel/global.h kernel/memory.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/console.o: device/console.c device/console.h thread/sync.h thread/thread.h lib/kernel/print.h \
				lib/stdint.h kernel/interrupt.h kernel/global.h kernel/memory.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/keyboard.o: device/keyboard.c device/keyboard.h thread/sync.h thread/thread.h lib/kernel/print.h \
				lib/stdint.h kernel/interrupt.h kernel/global.h lib/kernel/io.h device/ioqueue.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/ioqueue.o: device/ioqueue.c device/ioqueue.h thread/sync.h thread/thread.h lib/kernel/print.h \
				lib/stdint.h kernel/interrupt.h kernel/global.h lib/kernel/io.h kernel/debug.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/tss.o: userproc/tss.c userproc/tss.h thread/thread.h lib/kernel/print.h \
				lib/stdint.h kernel/interrupt.h kernel/global.h kernel/debug.h lib/string.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/process.o: userproc/process.c userproc/process.h userproc/userprog.h thread/thread.h lib/kernel/print.h lib/kernel/bitmap.h \
				lib/stdint.h kernel/interrupt.h kernel/global.h kernel/debug.h kernel/memory.h userproc/tss.h device/console.h lib/kernel/list.h lib/string.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/syscall.o: lib/user/syscall.c lib/user/syscall.h userproc/userprog.h thread/thread.h lib/kernel/print.h kernel/debug.h \
				lib/stdint.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/syscall-init.o: userproc/syscall_init.c userproc/syscall_init.h lib/user/syscall.h thread/thread.h lib/kernel/print.h \
				lib/stdint.h kernel/interrupt.h kernel/global.h kernel/debug.h device/console.h lib/string.h kernel/memory.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/stdio.o: lib/stdio.c lib/stdio.h lib/user/syscall.h thread/thread.h lib/kernel/print.h \
				lib/stdint.h kernel/interrupt.h kernel/global.h kernel/debug.h device/console.h lib/string.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/stdio-kernel.o: lib/kernel/stdio-kernel.c lib/kernel/stdio-kernel.h lib/stdio.h lib/kernel/print.h \
				lib/stdint.h kernel/interrupt.h kernel/global.h kernel/debug.h device/console.h lib/string.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/ide.o: device/ide.c device/ide.h thread/sync.h lib/kernel/list.h thread/sync.h lib/kernel/bitmap.h kernel/interrupt.h thread/thread.h\
				lib/stdint.h lib/kernel/io.h kernel/global.h kernel/debug.h kernel/memory.h lib/kernel/stdio-kernel.h device/console.h lib/kernel/list.h lib/string.h
				$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/fs.o: fs/fs.c fs/fs.h fs/dir.h fs/inode.h fs/super_block.h device/ide.c device/ide.h thread/sync.h lib/kernel/list.h\
				thread/sync.h lib/kernel/bitmap.h kernel/interrupt.h thread/thread.h\
				lib/stdint.h lib/kernel/io.h kernel/global.h kernel/debug.h kernel/memory.h lib/kernel/stdio-kernel.h lib/kernel/list.h lib/string.h
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


