#include "syscall.h"

#define _syscall0(NUMBER) ({ \
    int retVal; \
    asm volatile ("int $0x80" : "=a" (retVal) \
    : "a" (NUMBER) \
    : "memory"); \
    retVal; \
})

#define _syscall1(NUMBER, ARG1) ({ \
    int retVal; \
    asm volatile ("int $0x80" : "=a" (retVal) \
    : "a" (NUMBER), "b" (ARG1) \
    : "memory"); \
    retVal; \
})

#define _syscall2(NUMBER, ARG1, ARG2) ({ \
    int retVal; \
    asm volatile ("int $0x80" : "=a" (retVal) \
    : "a" (NUMBER), "b" (ARG1), "c" (ARG2) \
    : "memory"); \
    retVal; \
})

#define _syscall3(NUMBER, ARG1, ARG2, ARG3) ({ \
    int retVal; \
    asm volatile ("int $0x80" : "=a" (retVal) \
    : "a" (NUMBER), "b" (ARG1), "c" (ARG2), "d" (ARG3) \
    : "memory"); \
    retVal; \
})

uint32_t getpid() {
    return _syscall0(SYS_GETPID);
}

void* malloc(uint32_t size) {
    return (void*)_syscall1(SYS_MALLOC, size);
}

void free(void* ptr) {
    _syscall1(SYS_FREE, ptr);
}

uint32_t write(int32_t fd, const void* buf, uint32_t count) {
    return _syscall3(SYS_WRITE, fd, buf, count);
}

pid_t fork(void) {
    return _syscall0(SYS_FORK);
}

int32_t read(int32_t fd, void* buf, uint32_t count) {
    return _syscall3(SYS_READ, fd, buf, count);
}

void putchar(char char_ascii) {
    _syscall1(SYS_PUTCHAR, char_ascii);
}

void clear(void) {
    _syscall0(SYS_CLEAR);
}