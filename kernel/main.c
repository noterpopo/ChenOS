#include "print.h"
#include "init.h"
#include "debug.h"
#include "thread.h"
#include "console.h"
#include "process.h"

void k_thread_a(void* arg);
void k_thread_b(void* arg);
void u_prog_a(void* arg);
void u_prog_b(void* arg);
int test_var_a = 0, test_var_b = 0;
int main(void) {
    put_str("I am kernel.\n");
    init_all();
    thread_start("kthread_a", 31, k_thread_a, "argA ");
    thread_start("kthread_b", 31, k_thread_b, "argB ");
    process_execute(u_prog_a, "u_a");
    process_execute(u_prog_b, "u_b");
    intr_enable();
    while(1);
    return 0;
}

void k_thread_a(void* arg) {
    char* para = arg;
    while(1) {
        console_put_str("va: 0x");
        console_put_int(test_var_a);
    }
}

void k_thread_b(void* arg) {
    char* para = arg;
    while(1) {
        console_put_str(" vb: 0x");
        console_put_int(test_var_b);
    }
}

void u_prog_a(void* arg) {
    while(1) {
        test_var_a++;
    }
}

void u_prog_b(void* arg) {
    while(1) {
        test_var_b++;
    }
}