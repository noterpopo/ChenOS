#ifndef __FS_DIR_H__
#define __FS_DIR_H__
#include "stdint.h"
#include "inode.h"
#include "fs.h"
#include "ide.h"

#define MAX_FILE_NAME_LEN 16

struct dir {
    struct inode* inode;
    uint32_t dir_pos;
    uint8_t dir_buf[512];
};

struct dir_entry {
    char filename[MAX_FILE_NAME_LEN];
    uint32_t i_no;
    uint8_t f_type;
};

extern struct dir root_dir;
bool search_dir_entry(struct partition* part, struct dir* pdir, const char* name, struct dir_entry* dir_e);
void dir_close(struct dir* dir);
struct dir* dir_open(struct partition* part, uint32_t inode_no);
void create_dir_entry(char *filename, uint32_t inode_no, uint8_t file_type, struct dir_entry* p_de);
bool sync_dir_entry(struct dir* parent_dir, struct dir_entry* p_de, void* io_buf);
int32_t path_depth_cnt(char* pathname);
void open_root_dir(struct partition* part);
char* path_parse(char* pathname, char* name_store);
bool delete_dir_entry(struct partition* part, struct dir* pdir, uint32_t inode_no, void* io_buf);
struct dir_entry* dir_read(struct dir* dir);
bool dir_is_empty(struct dir* dir);
int32_t dir_remove(struct dir* parent_dir, struct dir* child_dir);
#endif