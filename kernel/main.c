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

int main(void) {
    put_str("I am kernel.\n");
    init_all();
    intr_enable();
    ide_init();
    filesys_init();
    printf("/dir/subdir1 create %s\n", sys_mkdir("/dir/subdir1") == 0 ? "done": "fail");
    printf("/dir create %s\n", sys_mkdir("/dir") == 0 ? "done": "fail");
    printf("/dir1/subdir1 create %s\n", sys_mkdir("/dir/subdir1") == 0 ? "done": "fail");
    int fd = sys_open("/dir/subdir1/file2", O_CREAT|O_RDWR);
    if (fd != -1){
        printf("/dir/dubdir1/file2 created\n");
        sys_write(fd, "Catch me if you can\n", 20);
        sys_lseek(fd, 0, SEEK_SET);
        char buf[32] = {0};
        sys_read(fd, buf, 20);
        printf("%s\n", buf);
        sys_close(fd);
    }
    while(1);
    return 0;
}