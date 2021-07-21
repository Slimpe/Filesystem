#include <iostream>
#include <cstdint>
#include "disk.h"

#ifndef __FS_H__
#define __FS_H__

#define ROOT_BLOCK 0
#define FAT_BLOCK 1
#define FAT_FREE 0
#define FAT_EOF -1

#define TYPE_FILE 0
#define TYPE_DIR 1
#define READ 0x04
#define WRITE 0x02
#define EXECUTE 0x01

// EGNA
#define TYPE_FREE 2

// size of fat
#define TABLE_SIZE (BLOCK_SIZE/2)

// size of directory_table
#define FILESYSTEM_SIZE (BLOCK_SIZE/sizeof(dir_entry) + 1)


class FS {
private:
    struct dir_entry {
        char file_name[56]; // name of the file / sub-directory | axel
        uint16_t first_blk; // index in the FAT for the first block of the file | 2
        uint32_t size; // size of the file in bytes | size_of(input)
        uint8_t type; // directory (1) or file (0) or free (2) | 0
        uint8_t access_rights; // read (0x04), write (0x02), execute (0x01) | ?
    };

    Disk disk;
    
    // size of a FAT entry is 2 bytes (?)
    int16_t fat[TABLE_SIZE];
    
    // table of information of files
    dir_entry directory_table[BLOCK_SIZE/sizeof(dir_entry) + 1];

public:
    FS();
    ~FS();
    // formats the disk, i.e., creates an empty file system
    int format();
    // create <filepath> creates a new file on the disk, the data content is
    // written on the following rows (ended with an empty row)
    int create(std::string filepath);
    // cat <filepath> reads the content of a file and prints it on the screen
    int cat(std::string filepath);
    // ls lists the content in the currect directory (files and sub-directories)
    int ls();

    // cp <sourcefilepath> <destfilepath> makes an exact copy of the file
    // <sourcefilepath> to a new file <destfilepath>
    int cp(std::string sourcefilepath, std::string destfilepath);
    // mv <sourcepath> <destpath> renames the file <sourcepath> to the name <destpath>,
    // or moves the file <sourcepath> to the directory <destpath> (if dest is a directory)
    int mv(std::string sourcepath, std::string destpath);
    // rm <filepath> removes / deletes the file <filepath>
    int rm(std::string filepath);
    // append <filepath1> <filepath2> appends the contents of file <filepath1> to
    // the end of file <filepath2>. The file <filepath1> is unchanged.
    int append(std::string filepath1, std::string filepath2);

    // mkdir <dirpath> creates a new sub-directory with the name <dirpath>
    // in the current directory
    int mkdir(std::string dirpath);
    // cd <dirpath> changes the current (working) directory to the directory named <dirpath>
    int cd(std::string dirpath);
    // pwd prints the full path, i.e., from the root directory, to the current
    // directory, including the currect directory name
    int pwd();

    // chmod <accessrights> <filepath> changes the access rights for the
    // file <filepath> to <accessrights>.
    int chmod(std::string accessrights, std::string filepath);

    // Our own helpfunctions
    void writeFAT();
    void writeFAT_directory();
    std::string retrieveFilename(std::string path);
    uint16_t findBlock(std::string filepath);
    void writeContentToDisk(unsigned block, std::string content);
    int findFirstFreeFatSlot();
    

};

#endif // __FS_H__
