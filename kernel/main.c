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

void k_thread_a(void* arg);
void k_thread_b(void* arg);
void u_prog_a(void* arg);
void u_prog_b(void* arg);
int test_var_a = 0, test_var_b = 0;
int main(void) {
    put_str("I am kernel.\n");
    init_all();
    process_execute(u_prog_a, "u_a");
    process_execute(u_prog_b, "u_b");
    intr_enable();
    console_put_char('\n');
    thread_start("kthread_a", 31, k_thread_a, "argA ");
    thread_start("kthread_b", 31, k_thread_b, "argB ");
    while(1);
    return 0;
}

void k_thread_a(void* arg) {
    char* para = arg;
    void* addr1;
    void* addr2;
    void* addr3;
    void* addr4;
    void* addr5;
    void* addr6;
    void* addr7;
    console_put_str("thread as start\n");
    int max = 1;
    while (max-- > 0) {
        int size = 128;
        addr1 = sys_malloc(size);
        size *= 2;
        addr2 = sys_malloc(size);
        size *= 2;
        addr3 = sys_malloc(size);
        sys_free(addr1);
        addr4 = sys_malloc(size);
        size *= 32;
        addr5 = sys_malloc(size);
        addr6 = sys_malloc(size);
        sys_free(addr5);
        size *= 2;
        addr7 = sys_malloc(size);
        sys_free(addr6);
        sys_free(addr7);
        sys_free(addr4);
        sys_free(addr3);
        sys_free(addr2);
    }
    console_put_str("thread as end\n");
    while(1);
}

void k_thread_b(void* arg) {
    char* para = arg;
    console_put_str(" b thread pid:0x");
    console_put_int(sys_getpid());
    console_put_char('\n');
    while(1);
}

void u_prog_a(void* arg) {
    char* name = "prog_a";
    void* addr1 = malloc(256);
    void* addr2 = malloc(255);
    void* addr3 = malloc(254);
    printf("I am %s, my pid: %d%c", name, getpid(), '\n');
    printf(" pa malloc addr: 0x%x, 0x%x, 0x%x", (int)addr1, (int)addr2, (int)addr3);
    while(1);
}

void u_prog_b(void* arg) {
    char* name = "prog_b";
    void* addr1 = malloc(256);
    void* addr2 = malloc(255);
    void* addr3 = malloc(254);
    printf("I am %s, my pid: %d%c", name, getpid(), '\n');
    printf(" pb malloc addr: 0x%x, 0x%x, 0x%x", (int)addr1, (int)addr2, (int)addr3);
    while(1);
}