#ifndef __KERNEL_DEBUG_H
#define __KERNEL_DEBUG_H
void painc_spin(char* filename, int line, const char* func, const char* condition);

#define PAINC(...) painc_spin (__FILE__, __LINE__, __func__, __VA_ARGS__)

#ifdef NDEBUG
    #define ASSERT(CONDITION) ((void)0)
#else
    #define ASSERT(CONDITION) \
    if (CONDITION){} else {    \
        PAINC(#CONDITION);     \
    }
#endif

#endif