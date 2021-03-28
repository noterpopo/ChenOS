#ifndef __LIB_STDIO_H
#define __LIB_STDIO_H
#include "stdint.h"
#include "global.h"
typedef char* va_list;
uint32_t printf(const char* format, ...);
uint32_t sprintf(char* buf, const char* format, ...);
#endif