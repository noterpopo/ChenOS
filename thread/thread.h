#ifndef __THREAD_THREAD_H
#define __THREAD_THREAD_H
#include "stdint.h"
#include "list.h"
#include "memory.h"
#define MAX_FILES_OPEN_PER_PROC 8
typedef void thread_func(void*);
typedef int16_t pid_t;

enum task_status {
    TASK_RUNNING,
    TASK_READY,
    TASK_BLOCKED,
    TASK_WAITTING,
    TASK_HANGING,
    TASK_DIED
};
// 中断栈，在页最顶端
struct intr_stack {
    uint32_t vec_no; //压入中断号
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp_dummy;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;
    uint32_t err_code;
    void (*eip) (void);
    uint32_t cs;
    uint32_t eflags;
    void* esp;
    uint32_t ss;
};
// 线程栈,switch_to时保持环境
struct thread_stack {
    uint32_t ebp;
    uint32_t ebx;
    uint32_t edi;
    uint32_t esi;
    // 第一次执行指向kernel_thread,后面就是switch_to的返回地址
    void (*eip) (thread_func* func, void* func_arg);
    // 实际上无用的返回地址
    void (*unused_ret_addr);
    thread_func* function;
    void* func_arg;
};
// pcb
struct task_struct {
    uint32_t* self_kstack;
    pid_t pid;
    enum task_status status;
    char name[16];
    uint8_t priority;
    uint8_t ticks;
    uint32_t elapsed_ticks;
    int32_t fd_table[MAX_FILES_OPEN_PER_PROC];
    struct list_elem general_tag;
    struct list_elem all_list_tag;
    uint32_t* pgdir;
    struct virtual_addr userproc_vaddr;
    struct mem_block_desc u_block_desc[DESC_CNT];
    uint32_t cwd_inode_nr;
    uint32_t stack_magic;
};
extern struct list thread_ready_list;
extern struct list thread_all_list;
struct task_struct* thread_start(char* name, int prio, thread_func function, void* func_arg);
void schedule();
void thread_init();
void thread_block(enum task_status stat);
void thread_unblock(struct task_struct* pthread);
void thread_create(struct task_struct* pthread, thread_func function, void* func_arg);
void init_thread(struct task_struct* pthread, char* name, int prio);
void thread_yield(void);
#endif