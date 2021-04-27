#include "dir.h"
#include "memory.h"
#include "stdio-kernel.h"
#include "string.h"
#include "debug.h"
#include "fs.h"
#include "file.h"

struct dir root_dir;

void open_root_dir(struct partition* part) {
    root_dir.inode = inode_open(part, part->sb->root_inode_no);
    root_dir.dir_pos = 0;
}

struct dir* dir_open(struct partition* part, uint32_t inode_no) {
    struct dir* pdir = (struct dir*)sys_malloc(sizeof(struct dir));
    pdir->inode = inode_open(part, inode_no);
    pdir->dir_pos = 0;
    return pdir;
}

bool search_dir_entry(struct partition* part, struct dir* pdir, const char* name, struct dir_entry* dir_e) {
    uint32_t block_cnt = 140;
    uint32_t* all_blocks = (uint32_t*)sys_malloc(block_cnt * 4);
    if (all_blocks == NULL) {
        printk("search_dir_entry: sys_malloc for all_blocks failed.\n");
        return false;
    }
    uint32_t block_idx = 0;
    while (block_idx < 12) {
        all_blocks[block_idx] = pdir->inode->i_sectors[block_idx];
        block_idx++;
    }
    block_idx = 0;
    if (pdir->inode->i_sectors[12] != 0) {
        ide_read(part->my_disk, pdir->inode->i_sectors[12], all_blocks + 12, 1);
    }

    uint8_t* buf = (uint8_t*)sys_malloc(SECTOR_SIZE);
    struct dir_entry* p_de = (struct dir_entry*)buf;
    uint32_t dir_entry_size = part->sb->dir_entry_size;
    uint32_t dir_entry_cnt = SECTOR_SIZE / dir_entry_size;

    while (block_idx < block_cnt) {
        if (all_blocks[block_idx] == 0) {
            block_idx++;
            continue;
        }
        ide_read(part->my_disk, all_blocks[block_idx], buf, 1);
        uint32_t dir_entry_idx = 0;
        while (dir_entry_idx < dir_entry_cnt) {
            if (!strcmp(p_de->filename, name)) {
                memcpy(dir_e, p_de, dir_entry_size);
                sys_free(buf);
                sys_free(all_blocks);
                return true;
            }
            dir_entry_idx++;
            p_de++;
        }
        block_idx++;
        p_de = (struct dir_entry*)buf;
        memset(buf, 0, SECTOR_SIZE);
    }
    sys_free(buf);
    sys_free(all_blocks);
    return false;
}

void dir_close(struct dir* dir) {
    if (dir == &root_dir) {
        return;
    }
    inode_close(dir->inode);
    sys_free(dir);
}

void create_dir_entry(char *filename, uint32_t inode_no, uint8_t file_type, struct dir_entry* p_de) {
    ASSERT(strlen(filename) <= MAX_FILE_NAME_LEN);
    memcpy(p_de->filename, filename, strlen(filename));
    p_de->i_no = inode_no;
    p_de->f_type = file_type;
}

bool delete_dir_entry(struct partition* part, struct dir* pdir, uint32_t inode_no, void* io_buf) {
   struct inode* dir_inode = pdir->inode;
   uint32_t block_idx = 0, all_blocks[140] = {0};
   /* 收集目录全部块地址 */
   while (block_idx < 12) {
      all_blocks[block_idx] = dir_inode->i_sectors[block_idx];
      block_idx++;
   }
   if (dir_inode->i_sectors[12]) {
      ide_read(part->my_disk, dir_inode->i_sectors[12], all_blocks + 12, 1);
   }

   /* 目录项在存储时保证不会跨扇区 */
   uint32_t dir_entry_size = part->sb->dir_entry_size;
   uint32_t dir_entrys_per_sec = (SECTOR_SIZE / dir_entry_size);       // 每扇区最大的目录项数目
   struct dir_entry* dir_e = (struct dir_entry*)io_buf;   
   struct dir_entry* dir_entry_found = NULL;
   uint8_t dir_entry_idx, dir_entry_cnt;
   bool is_dir_first_block = false;     // 目录的第1个块 

   /* 遍历所有块,寻找目录项 */
   block_idx = 0;
   while (block_idx < 140) {
      is_dir_first_block = false;
      if (all_blocks[block_idx] == 0) {
	 block_idx++;
	 continue;
      }
      dir_entry_idx = dir_entry_cnt = 0;
      memset(io_buf, 0, SECTOR_SIZE);
      /* 读取扇区,获得目录项 */
      ide_read(part->my_disk, all_blocks[block_idx], io_buf, 1);

      /* 遍历所有的目录项,统计该扇区的目录项数量及是否有待删除的目录项 */
      while (dir_entry_idx < dir_entrys_per_sec) {
	 if ((dir_e + dir_entry_idx)->f_type != FT_UNKNOWN) {
	    if (!strcmp((dir_e + dir_entry_idx)->filename, ".")) { 
	       is_dir_first_block = true;
	    } else if (strcmp((dir_e + dir_entry_idx)->filename, ".") && 
	       strcmp((dir_e + dir_entry_idx)->filename, "..")) {
	       dir_entry_cnt++;     // 统计此扇区内的目录项个数,用来判断删除目录项后是否回收该扇区
	       if ((dir_e + dir_entry_idx)->i_no == inode_no) {	  // 如果找到此i结点,就将其记录在dir_entry_found
		  ASSERT(dir_entry_found == NULL);  // 确保目录中只有一个编号为inode_no的inode,找到一次后dir_entry_found就不再是NULL
		  dir_entry_found = dir_e + dir_entry_idx;
		  /* 找到后也继续遍历,统计总共的目录项数 */
	       }
	    }
	 }
	 dir_entry_idx++;
      } 

      /* 若此扇区未找到该目录项,继续在下个扇区中找 */
      if (dir_entry_found == NULL) {
	 block_idx++;
	 continue;
      }

   /* 在此扇区中找到目录项后,清除该目录项并判断是否回收扇区,随后退出循环直接返回 */
      ASSERT(dir_entry_cnt >= 1);
    /* 除目录第1个扇区外,若该扇区上只有该目录项自己,则将整个扇区回收 */
      if (dir_entry_cnt == 1 && !is_dir_first_block) {
	 /* a 在块位图中回收该块 */
	 uint32_t block_bitmap_idx = all_blocks[block_idx] - part->sb->data_start_lba;
	 bitmap_set(&part->block_bitmap, block_bitmap_idx, 0);
	 bitmap_sync(cur_part, block_bitmap_idx, BLOCK_BITMAP);

	 /* b 将块地址从数组i_sectors或索引表中去掉 */
	 if (block_idx < 12) {
	    dir_inode->i_sectors[block_idx] = 0;
	 } else {    // 在一级间接索引表中擦除该间接块地址
	    /*先判断一级间接索引表中间接块的数量,如果仅有这1个间接块,连同间接索引表所在的块一同回收 */
	    uint32_t indirect_blocks = 0;
	    uint32_t indirect_block_idx = 12;
	    while (indirect_block_idx < 140) {
	       if (all_blocks[indirect_block_idx] != 0) {
		  indirect_blocks++;
	       }
	    }
	    ASSERT(indirect_blocks >= 1);  // 包括当前间接块

	    if (indirect_blocks > 1) {	  // 间接索引表中还包括其它间接块,仅在索引表中擦除当前这个间接块地址
	       all_blocks[block_idx] = 0; 
	       ide_write(part->my_disk, dir_inode->i_sectors[12], all_blocks + 12, 1); 
	    } else {	// 间接索引表中就当前这1个间接块,直接把间接索引表所在的块回收,然后擦除间接索引表块地址
	       /* 回收间接索引表所在的块 */
	       block_bitmap_idx = dir_inode->i_sectors[12] - part->sb->data_start_lba;
	       bitmap_set(&part->block_bitmap, block_bitmap_idx, 0);
	       bitmap_sync(cur_part, block_bitmap_idx, BLOCK_BITMAP);
	       
	       /* 将间接索引表地址清0 */
	       dir_inode->i_sectors[12] = 0;
	    }
	 }
      } else { // 仅将该目录项清空
	 memset(dir_entry_found, 0, dir_entry_size);
	 ide_write(part->my_disk, all_blocks[block_idx], io_buf, 1);
      }

   /* 更新i结点信息并同步到硬盘 */
      ASSERT(dir_inode->i_size >= dir_entry_size);
      dir_inode->i_size -= dir_entry_size;
      memset(io_buf, 0, SECTOR_SIZE * 2);
      inode_sync(part, dir_inode, io_buf);

      return true;
   }
   /* 所有块中未找到则返回false,若出现这种情况应该是serarch_file出错了 */
   return false;
}

bool sync_dir_entry(struct dir* parent_dir, struct dir_entry* p_de, void* io_buf) {
    struct inode* dir_inode = parent_dir->inode;
    uint32_t dir_size = dir_inode->i_size;
    uint32_t dir_entry_size = cur_part->sb->dir_entry_size;

    ASSERT(dir_size % dir_entry_size == 0);

    uint32_t dir_entrys_per_sec = (SECTOR_SIZE / dir_entry_size);
    int32_t block_lba = -1;
    uint8_t block_idx = 0;
    uint32_t all_blocks[140] = {0};
    while (block_idx < 12) {
        all_blocks[block_idx] = dir_inode->i_sectors[block_idx];
        block_idx++;
    }
    struct dir_entry* dir_e = (struct dir_entry*)io_buf;
    int32_t block_bitmap_idx = -1;
    block_idx = 0;
    while (block_idx < 140) {
        block_bitmap_idx = -1;
        if (all_blocks[block_idx] == 0) {
            block_lba = block_bitmap_alloc(cur_part);
            if (block_lba == -1) {
                printk("alloc block bitmap for sync_dir_entry failed\n");
                return false;
            }
            block_bitmap_idx = block_lba - cur_part->sb->data_start_lba;
            ASSERT(block_bitmap_idx != -1);
            bitmap_sync(cur_part, block_bitmap_idx, BLOCK_BITMAP);

            block_bitmap_idx = -1;
            if (block_idx < 12) {
                dir_inode->i_sectors[block_idx] = all_blocks[block_idx] = block_lba;
            } else if (block_idx == 12) {
                dir_inode->i_sectors[12] = block_lba;
                block_lba = -1;
                block_lba = block_bitmap_alloc(cur_part);
                if (block_lba == -1) {
                    block_bitmap_idx = dir_inode->i_sectors[12] - cur_part->sb->data_start_lba;
                    bitmap_set(&cur_part->block_bitmap, block_bitmap_idx, 0);
                    dir_inode->i_sectors[12] = 0;
                    printk("alloc block bitmap for sync_dir_entry faailed\n");
                    return false;
                }
                block_bitmap_idx = block_lba - cur_part->sb->data_start_lba;
                ASSERT(block_bitmap_idx != -1);
                bitmap_sync(cur_part, block_bitmap_idx, BLOCK_BITMAP);
                all_blocks[12] = block_lba;
                ide_write(cur_part->my_disk, dir_inode->i_sectors[12], all_blocks + 12, 1);
            } else {
                all_blocks[block_idx] = block_lba;
                ide_write(cur_part->my_disk, dir_inode->i_sectors[12], all_blocks + 12, 1);
            }
            memset(io_buf, 0, 512);
            memcpy(io_buf, p_de, dir_entry_size);
            ide_write(cur_part->my_disk, all_blocks[block_idx], io_buf, 1);
            return true;
        }
        ide_read(cur_part->my_disk, all_blocks[block_idx], io_buf, 1);
        uint8_t dir_entry_idx = 0;
        while (dir_entry_idx < dir_entrys_per_sec) {
            if ((dir_e + dir_entry_idx)->f_type == FT_UNKNOWN) {
                memcpy(dir_e + dir_entry_idx, p_de, dir_entry_size);
                ide_write(cur_part->my_disk, all_blocks[block_idx], io_buf, 1);
                dir_inode->i_size += dir_entry_size;
                return true;
            }
            dir_entry_idx++;
        }
        block_idx++;
    }
    printk("directory is full\n");
    return false;
}

char* path_parse(char* pathname, char* name_store) {
    if (pathname[0] == '/') {
        while(*(++pathname) == '/');
    }
    while(*pathname != '/' && *pathname != 0) {
        *name_store++ = *pathname++;
    }
    if(pathname[0] == 0) {
        return NULL;
    }
    return pathname;
}

int32_t path_depth_cnt(char* pathname) {
    ASSERT(pathname != NULL);
    char* p = pathname;
    char name[MAX_FILE_NAME_LEN];
    uint32_t depth = 0;
    p = path_parse(p, name);
    while (name[0]) {
        depth++;
        memset(name, 0, MAX_FILE_NAME_LEN);
        if (p) {
            p = path_parse(p, name);
        }
    }
    return depth;
}

struct dir_entry* dir_read(struct dir* dir) {
    struct dir_entry* dir_e = (struct dir_entry*)dir->dir_buf;
    struct inode* dir_inode = dir->inode;
    uint32_t all_blocks[140] = {0}, block_cnt = 12;
    uint32_t block_idx = 0, dir_entry_idx = 0;
    while (block_idx < 12) {
        all_blocks[block_idx] = dir_inode->i_sectors[block_idx];
        block_idx++;
    }
    if (dir_inode->i_sectors[12] != 0) {
        ide_read(cur_part->my_disk, dir_inode->i_sectors[12], all_blocks + 12, 1);
        block_cnt = 140;
    }
    block_idx = 0;
    uint32_t cur_dir_entry_pos = 0;
    uint32_t dir_entry_size = cur_part->sb->dir_entry_size;
    uint32_t dir_entrys_per_sec = SECTOR_SIZE / dir_entry_size;
    while (dir->dir_pos < dir_inode->i_size) {
        if (dir->dir_pos >= dir_inode->i_size) {
            return NULL;
        }
        if (all_blocks[block_idx] == 0) {
            block_idx++;
            continue;
        }
        memset(dir_e, 0, SECTOR_SIZE);
        ide_read(cur_part->my_disk, all_blocks[block_idx], dir_e, 1);
        dir_entry_idx = 0;
        while (dir_entry_idx < dir_entrys_per_sec) {
            if ((dir_e + dir_entry_idx)->f_type) {
                if (cur_dir_entry_pos < dir->dir_pos) {
                    cur_dir_entry_pos += dir_entry_size;
                    dir_entry_idx++;
                    continue;
                }
                ASSERT(cur_dir_entry_pos == dir->dir_pos);
                dir->dir_pos += dir_entry_size;
                return dir_e + dir_entry_idx;
            }
            dir_entry_idx++;
        }
        block_idx++;
    }
    return NULL;
}

bool dir_is_empty(struct dir* dir) {
    struct inode* dir_inide = dir->inode;
    return (dir->inode->i_size == cur_part->sb->dir_entry_size * 2);
}

int32_t dir_remove(struct dir* parent_dir, struct dir* child_dir) {
    struct inode* child_dir_inode = child_dir->inode;
    int32_t block_idx = 1;
    while (block_idx < 13) {
        ASSERT(child_dir_inode->i_sectors[block_idx] == 0);
        block_idx++;
    }
    void* io_buf = sys_malloc(SECTOR_SIZE * 2);
    if (io_buf == NULL) {
        printk("dir_remove: malloc failed\n");
        return  -1;
    }
    delete_dir_entry(cur_part, parent_dir, child_dir_inode->i_no, io_buf);
    inode_release(cur_part, child_dir_inode->i_no);
    sys_free(io_buf);
    return 0;
}
