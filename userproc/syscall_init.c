#include "syscall.h"
#include "syscall_init.h"
#include "thread.h"
#include "print.h"
#include "console.h"
#include "string.h"
#define syscall_nr 32
typedef void* syscall;
syscall syscall_table[syscall_nr];

uint32_t sys_getpid(void) {
    return ((struct task_struct*)running_thread())->pid;
}

uint32_t sys_write(char* str) {
    console_put_str(str);
    return strlen(str);
}

void syscall_init(void) {
    put_str("syscall_init start\n");
    syscall_table[SYS_GETPID] = sys_getpid;
    syscall_table[SYS_WRITE] = sys_write;
    put_str("syscall_init done\n");
}
