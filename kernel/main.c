#include "print.h"
#include "init.h"
#include "debug.h"
#include "thread.h"
#include "console.h"

void k_thread_a(void* arg);
int main(void) {
    put_str("I am kernel.\n");
    init_all();
    intr_enable();
    while(1);
    return 0;
}

void k_thread_a(void* arg) {
    char* para = arg;
    while(1) {
        console_put_str(para);
    }
}