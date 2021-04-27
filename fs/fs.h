#ifndef __FS_FS_H__
#define __FS_FS_H__
#include "dir.h"

#define MAX_FILES_PER_PART 4096
#define BITS_PER_SECTOR 4096
#define SECTOR_SIZE 512
#define BLOCK_SIZE SECTOR_SIZE
#define MAX_PATH_LEN 512

extern struct partition* cur_part;

enum file_types {
    FT_UNKNOWN,
    FT_REGULAR,
    FT_DIRECTORY
};
enum oflags {
    O_RDONLY,
    O_WRONLY,
    O_RDWR,
    O_CREAT = 4
};
enum whence {
    SEEK_SET = 1,
    SEEK_CUR,
    SEEK_END
};
struct path_search_record {
    char searched_path[MAX_PATH_LEN];
    struct dir* parent_dir;
    enum file_types file_type;
};
struct stat {
    uint32_t st_ino;
    uint32_t st_size;
    enum file_types st_file_type;
};
void filesys_init();
int32_t sys_open(const char* pathname, uint8_t flags);
int32_t sys_close(int32_t fd);
int32_t sys_write(int32_t fd, const void* buf, uint32_t count);
int32_t sys_read(int32_t fd, void* buf, uint32_t count);
int32_t sys_lseek(int32_t fd, int32_t offset, uint8_t whence);
int32_t sys_unlink(const char* pathname);
int32_t sys_mkdir(const char* pathname);
int32_t sys_closedir(struct dir* dir);
struct dir* sys_opendir(const char* name);
struct dir_entry* sys_readdir(struct dir* dir);
void sys_rewinddir(struct dir* dir);
int32_t sys_rmdir(const char* pathname);
char* sys_getcwd(char* buf, uint32_t size);
int32_t sys_ch_dir(const char* path);
int32_t sys_stat(const char* path, struct stat* buf);
#endif