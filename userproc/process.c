#include "process.h"
#include "debug.h"
#include "tss.h"
#include "memory.h"
#include "console.h"
#include "bitmap.h"
#include "list.h"
#include "string.h"
extern void intr_exit(void);

void strat_process(void* filename_) {
    void* function = filename_;
    struct task_struct* cur = running_thread();
    cur->self_kstack += sizeof(struct thread_stack);
    struct intr_stack* proc_stack = (struct intr_stack*)cur->self_kstack;
    proc_stack->edi = proc_stack->esi = proc_stack->ebp = proc_stack->esp_dummy = 0;
    proc_stack->ebx = proc_stack->edx = proc_stack->ecx = proc_stack ->eax = 0;
    proc_stack->gs = 0;
    proc_stack->ds = proc_stack->es = proc_stack->fs = SELECTOR_U_DATA;
    proc_stack->eip = function;
    proc_stack->cs = SELECTOR_U_CODE;
    proc_stack->eflags = (EFLAGS_IOPL_0 | EFLAGS_MBS | EFLAGS_IF_1);
    proc_stack->esp = (void*)((uint32_t)get_a_page(PF_USER, USER_STACK3_VADDR) + PG_SIZE);
    proc_stack->ss = SELECTOR_U_DATA;
    asm volatile ("movl %0, %%esp; jmp intr_exit": : "g" (proc_stack): "memory");
}

void page_dir_activate(struct task_struct* pthread) {
    uint32_t pagedir_phy_addr = 0x100000;
    if (pthread->pgdir != NULL) {
        pagedir_phy_addr = addr_v2p((uint32_t)pthread->pgdir);
    }
    asm volatile ("movl %0, %%cr3": : "r" (pagedir_phy_addr): "memory");
}

void process_activite(struct task_struct* pthread) {
    ASSERT(pthread != NULL);
    page_dir_activate(pthread);
    if (pthread->pgdir) {
        update_tss_esp(pthread);
    }
}

uint32_t* create_page_dir(void) {
    uint32_t* page_dir_vaddr = get_kernel_pages(1);
    if (page_dir_vaddr == NULL) {
        console_put_str("create_page_dir: get_kernel_page failed!\n");
        return NULL;
    }
    memcpy((uint32_t*)((uint32_t)page_dir_vaddr + 0x300 * 4), (uint32_t*)(0xfffff000 + 0x300 * 4), 1024);
    uint32_t new_page_dir_phy_addr = addr_v2p((uint32_t)page_dir_vaddr);
    page_dir_vaddr[1023] = new_page_dir_phy_addr | PG_US_U | PG_RW_W | PG_P_1;
    return page_dir_vaddr;
}

void create_user_vaddr_bitmap(struct task_struct* user_prog) {
    user_prog->userproc_vaddr.vaddr_start = USER_VADDR_START;
    uint32_t bitmap_pg_cnt = DIV_ROUND_UP((0xc0000000 - USER_VADDR_START) / PG_SIZE / 8, PG_SIZE);
    user_prog->userproc_vaddr.vaddr_bitmap.bits = get_kernel_pages(bitmap_pg_cnt);
    user_prog->userproc_vaddr.vaddr_bitmap.btmp_bytes_len = (0xc0000000 - USER_VADDR_START) / PG_SIZE / 8;
    bitmap_init(&user_prog->userproc_vaddr.vaddr_bitmap);
}

void process_execute(void* filename, char* name) {
    struct task_struct* thread = get_kernel_pages(1);
    init_thread(thread, name, DEFAULT_PRIO);
    create_user_vaddr_bitmap(thread);
    thread_create(thread, strat_process, filename);
    thread->pgdir = create_page_dir();
    block_desc_init(thread->u_block_desc);

    enum intr_status old_status = intr_disable();
    ASSERT(!elem_find(&thread_ready_list, &thread->general_tag));
    list_append(&thread_ready_list, &thread->general_tag);
    ASSERT(!elem_find(&thread_all_list, &thread->all_list_tag));
    list_append(&thread_all_list, &thread->all_list_tag);
    intr_set_status(old_status);
}
