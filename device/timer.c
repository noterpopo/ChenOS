#include "timer.h"
#include "io.h"
#include "print.h"
#include "thread.h"
#include "interrupt.h"
#include "debug.h"

#define IRQ0_FREQUENCY 100
#define INPUT_FREQUENCY 1193180
#define COUNTER0_VALUE INPUT_FREQUENCY / IRQ0_FREQUENCY
#define COUNTER0_PORT 0x40
#define COUNTER0_NO 0
#define COUNTER_MODE 2
#define READER_WEITE_LATCH 3
#define PIT_CONTROL_PORt 0x43

uint32_t ticks;

static void frequency_set(uint8_t counter_port, uint8_t counter_no, uint8_t rwl, uint8_t counter_mode, uint16_t counter_value) {
    outb(PIT_CONTROL_PORt, (uint8_t)(counter_no << 16 | rwl << 4 | counter_mode << 1));
    outb(counter_port, (uint8_t)counter_value);
    outb(counter_port, (uint8_t)(counter_value >> 8));
}

static void intr_timer_handler(void) {
    struct task_struct* cur_thread = running_thread();
    ASSERT(cur_thread->stack_magic == 0x19980711);
    cur_thread->elapsed_ticks++;
    ticks++;
    if (cur_thread->ticks == 0) {
        schedule();
    } else {
        cur_thread->ticks--;
    }
}

void timer_init() {
    put_str("timer_init start\n");
    frequency_set(COUNTER0_PORT, COUNTER0_NO, READER_WEITE_LATCH, COUNTER_MODE, COUNTER0_VALUE);
    register_handler(0x20, intr_timer_handler);
    put_str("timer_init done\n");
}