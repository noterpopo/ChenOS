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
#include "shell.h"
void init(void);
int main(void) {
    put_str("I am kernel.\n");
    init_all();
    // uint32_t file_size = 14448;
    // uint32_t sec_cnt = DIV_ROUND_UP(file_size, 512);
    // struct disk* sda  = &channels[0].devices[0];
    // void* prog_buf = sys_malloc(file_size);
    // ide_read(sda, 300, prog_buf, sec_cnt);
    // int32_t fd = sys_open("/prog_arg", O_CREAT|O_RDWR);
    // if (fd != -1) {
    //     if (sys_write(fd, prog_buf, file_size) == -1) {
    //         printk("file_write error\n");
    //         while(1);
    //     }
    // }
    cls_screen();
    console_put_str("[noterpopo@localhost /]$ ");
    while(1);
    return 0;
}

void init(void) {
    uint32_t ret_pid = fork();
    if (ret_pid) {
        while(1);
    } else {
        my_shell();
    }
    PAINC("init: should not be here");
}