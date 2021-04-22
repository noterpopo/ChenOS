#ifndef __FS_INODE_H__
#define __FS_INODE_H__
#include "stdint.h"
#include "list.h"

struct inode {
    uint32_t i_no;
    uint32_t i_size;
    uint32_t i_open_cnts;
    bool write_deny;
    uint32_t i_sectors[13];
    struct list_elem inode_tag;
};
#endif