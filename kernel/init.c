#include "init.h"
#include "print.h"
#include "interrupt.h"
#include "../device/timer.h"
#include "memory.h"
#include "thread.h"
#include "console.h"
#include "keyboard.h"
#include "tss.h"
#include "syscall_init.h"

void init_all() {
    put_str("init_all\n");
    idt_init(); // 初始化中断
    mem_init();
    thread_init();
    timer_init(); //初始化PIT
    console_init();
    keyboard_init();
    tss_init();
    syscall_init();
    intr_enable();
    ide_init();
    filesys_init();
}