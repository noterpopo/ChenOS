#ifndef __DEVICE_KEYBOARD_H
#define __DEVICE_KEYBOARD_H
#include "print.h"
#include "interrupt.h"
#include "io.h"
#include "global.h"
#include "ioqueue.h"

void keyboard_init();
extern struct ioqueue kbd_buf;
#endif