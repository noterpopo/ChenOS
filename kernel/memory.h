#ifndef __KERNEL_MEMORY_H
#define __KERNEL_MEMORY_H
#include "stdint.h"
#include "bitmap.h"
// 虚拟内存地址池结构
struct virtual_addr {
    struct bitmap vaddr_bitmap;
    uint32_t vaddr_start; //虚拟地址起始位置
};
extern struct pool kernel_pool, user_pool;
void mem_init(void);
#endif