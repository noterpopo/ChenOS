# ifndef __DEVICES_CONSOLE_H
#define __DEVICES_CONSOLE_H
#include "sync.h"
#include "print.h"
#include "stdint.h"
#include "thread.h"
void console_init();

void console_put_str(char* str);

void console_put_char(uint8_t c);

void console_put_int(uint32_t num);
#endif