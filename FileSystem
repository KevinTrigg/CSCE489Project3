/* File System Structure */

typedef struct FileSystem FileSystem;
struct FileSystem {
    Disk        *disk;                          /* Disk file system is mounted on */
    bool        *free_blocks;                   /* Free block bitmap */
    SuperBlock   meta_data;                     /* File system meta data */
};

/* File System Functions */

void    fs_debug(Disk *disk);
bool    fs_format(Disk *disk);

bool    fs_mount(FileSystem *fs, Disk *disk);
void    fs_unmount(FileSystem *fs);

ssize_t fs_create(FileSystem *fs);
bool    fs_remove(FileSystem *fs, size_t inode_number);
ssize_t fs_stat(FileSystem *fs, size_t inode_number);

ssize_t fs_read(FileSystem *fs, size_t inode_number, char *data, size_t length, size_t offset);
ssize_t fs_write(FileSystem *fs, size_t inode_number, char *data, size_t length, size_t offset);
