#include "print.h"
#include "init.h"
#include "debug.h"
#include "thread.h"
#include "console.h"
#include "process.h"
#include "syscall_init.h"
#include "syscall.h"
#include "stdio.h"

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
    console_put_str(" main_pid:0x");
    console_put_int(sys_getpid());
    console_put_char('\n');
    thread_start("kthread_a", 31, k_thread_a, "argA ");
    thread_start("kthread_b", 31, k_thread_b, "argB ");
    while(1);
    return 0;
}

void k_thread_a(void* arg) {
    char* para = arg;
    console_put_str(" a thread pid:0x");
    console_put_int(sys_getpid());
    console_put_char('\n');
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
    printf("I am %s, my pid: %d%c", name, getpid(), '\n');
    while(1);
}

void u_prog_b(void* arg) {
    char* name = "prog_b";
    printf("I am %s, my pid: %d%c", name, getpid(), '\n');
    while(1);
}