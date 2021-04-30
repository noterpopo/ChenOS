#include "print.h"
#include "init.h"
#include "debug.h"
#include "thread.h"
#include "console.h"
#include "process.h"
#include "syscall_init.h"
#include "syscall.h"
#include "stdio.h"
#include "memory.h"
#include "ide.h"
#include "fs.h"
void init(void);
int main(void) {
    put_str("I am kernel.\n");
    init_all();
    cls_screen();
    console_put_str("[noterpopo@localhost /]$ ");
    while(1);
    return 0;
}

void init(void) {
    uint32_t ret_pid = fork();
    if (ret_pid) {
        printf("i am father . child pid is %d\n", ret_pid);
    } else {
        printf("i am child .  ret pid is %d\n", ret_pid);
    }
    while(1);
}