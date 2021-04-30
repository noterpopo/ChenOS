#ifndef __USERPROC_PROCESS_H
#define __USERPROC_PROCESS_H
#include "stdint.h"
#include "thread.h"
#include "global.h"
#include "userprog.h"
#include "interrupt.h"
#define USER_VADDR_START 0x8048000 //elf文件起始地址
void process_activite(struct task_struct* pthread);
void process_execute(void* filename, char* name);
void page_dir_activate(struct task_struct* pthread);
#endif