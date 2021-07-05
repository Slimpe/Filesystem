#include <iostream>
#include "fs.h"
#include <vector>

FS::FS()
{
    std::cout << "FS::FS()... Creating file system\n";
    disk.read(ROOT_BLOCK, (uint8_t*) directory_table);
    disk.read(FAT_BLOCK, (uint8_t*) fat);
}

FS::~FS()
{

}

// formats the disk, i.e., creates an empty file system
int
FS::format()
{  
    std::cout << "FS::format()\n";

    // FAT_EOF = -1
    fat[0] = FAT_EOF;
    fat[1] = FAT_EOF;

    directory_table[0].first_blk = ROOT_BLOCK;
    directory_table[0].type = TYPE_FREE;

    // set all indexes except 0 and 1 as free
    for(unsigned int i = 2; i < TABLE_SIZE; i++){
        fat[i] = FAT_FREE;
    };
    // set all indexes except 0 as free
    for(unsigned int i = 1; i < FILESYSTEM_SIZE; i++){
        directory_table[i].type = TYPE_FREE;
    };

    writeFAT();
    writeFAT_directory();

    // std::cout << "\nFAT : " << std::endl;
    // for(unsigned int i = 0; i < 10; i++){
    //     std::cout << fat[i] << std::endl;
    // };
    // std::cout << "\n" << std::endl;
    // std::cout << "DIRECTORY : " << std::endl;
    // std::cout << directory_table[0].first_blk << std::endl;
    // std::cout << unsigned(directory_table[0].type) << std::endl;

    // for(int i = 0; i < 10; i++){
    //    std::cout << unsigned(directory_table[i].type) << std::endl;
    // };

    return 0;
}

void
FS::writeFAT()
{
    disk.write(FAT_BLOCK, (uint8_t*) fat);
}
void
FS::writeFAT_directory()
{
    disk.write(ROOT_BLOCK, (uint8_t*) directory_table);
}

// create <filepath> creates a new file on the disk, the data content is
// written on the following rows (ended with an empty row)
int
FS::create(std::string filepath)
{
    std::cout << "FS::create(" << filepath << ")\n";

    // input reads up to, but not including, the first whitespace!!!!
    std::string input; // hello world
    std::string empty_line;

    // std::cin >> input;
    //empty_line = "\n";
    std::vector<std::string> vec;

    bool flag = true;
    while(flag){
        
        //search for a free slot in FAT
        int firstFreeFat;
        for(int i = 2; i < TABLE_SIZE; i++) {
            if(fat[i] == FAT_FREE) {
                // check size of file to see if more blocks needed (later)
                fat[i] = FAT_EOF;
                return firstFreeFat = i;
            }
        }

        for(int j = 0; j < FILESYSTEM_SIZE; j++) {
            if(directory_table[j].type == TYPE_FREE) {
                directory_table[j].first_blk = firstFreeFat;
                directory_table[j].size = sizeof(input);
                directory_table[j].type = TYPE_FILE;
                directory_table[j].access_rights = READ;
            }
            break;
        }
        
        // read content of the file
        std::cin >> input;

        while(input != "") {
            vec.push_back(input);
        }
        flag = false;
    }

    writeFAT();
    writeFAT_directory();

    return 0;
}

// cat <filepath> reads the content of a file and prints it on the screen
int
FS::cat(std::string filepath)
{
    std::cout << "FS::cat(" << filepath << ")\n";

    for(int i = 0; i < FILESYSTEM_SIZE; i++) {
        
        if(directory_table[i].file_name == filepath) {
            // open file
            // read lines
            // std::out to terminal
            
        }
    }

    return 0;
}

// ls lists the content in the currect directory (files and sub-directories)
int
FS::ls()
{
    std::cout << "FS::ls()\n";

    int i;
    for(i = 0; i < TABLE_SIZE; i++)
    {
        std::cout << directory_table[i].file_name << std::endl;
        std::cout << "\n" << std::endl;
    }

    return 0;
}

// cp <sourcefilepath> <destfilepath> makes an exact copy of the file
// <sourcefilepath> to a new file <destfilepath>
int
FS::cp(std::string sourcefilepath, std::string destfilepath)
{
    std::cout << "FS::cp(" << sourcefilepath << "," << destfilepath << ")\n";
    return 0;
}

// mv <sourcepath> <destpath> renames the file <sourcepath> to the name <destpath>,
// or moves the file <sourcepath> to the directory <destpath> (if dest is a directory)
int
FS::mv(std::string sourcepath, std::string destpath)
{
    std::cout << "FS::mv(" << sourcepath << "," << destpath << ")\n";
    return 0;
}

// rm <filepath> removes / deletes the file <filepath>
int
FS::rm(std::string filepath)
{
    std::cout << "FS::rm(" << filepath << ")\n";
    return 0;
}

// append <filepath1> <filepath2> appends the contents of file <filepath1> to
// the end of file <filepath2>. The file <filepath1> is unchanged.
int
FS::append(std::string filepath1, std::string filepath2)
{
    std::cout << "FS::append(" << filepath1 << "," << filepath2 << ")\n";
    return 0;
}

// mkdir <dirpath> creates a new sub-directory with the name <dirpath>
// in the current directory
int
FS::mkdir(std::string dirpath)
{
    std::cout << "FS::mkdir(" << dirpath << ")\n";
    return 0;
}

// cd <dirpath> changes the current (working) directory to the directory named <dirpath>
int
FS::cd(std::string dirpath)
{
    std::cout << "FS::cd(" << dirpath << ")\n";
    return 0;
}

// pwd prints the full path, i.e., from the root directory, to the current
// directory, including the currect directory name
int
FS::pwd()
{
    std::cout << "FS::pwd()\n";
    return 0;
}

// chmod <accessrights> <filepath> changes the access rights for the
// file <filepath> to <accessrights>.
int
FS::chmod(std::string accessrights, std::string filepath)
{
    std::cout << "FS::chmod(" << accessrights << "," << filepath << ")\n";
    return 0;
}
