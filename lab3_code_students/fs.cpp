#include <iostream>
#include "fs.h"
#include <vector>
#include <cstring>

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

    return 0;
}

// #### EGNA ####

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

void
FS::writeContentToDisk(unsigned block, uint8_t* content){

    disk.write(block, content);
}

bool accessCheck(int accessRight, int operation) {
    bool ret = false;

    if(accessRight % 2 == 1 && operation == 1) { // EXECUTE
        ret = true;
    }
    else if(accessRight % 2 == 0 && accessRight != 4 && operation == 2) { // WRITE
        ret = true;
    }
    else if(accessRight >= 4 && operation == 4) { // READ
        ret = true;
    }
    else if(accessRight == 7) { // ALL RIGHTS
        ret = true;
    }
    return ret;
}

int
FS::findFirstFreeFatSlot(int numOfBlocks)
{
    int firstFree;
    for(int i = 2; i < TABLE_SIZE; i++)
    {
        if(fat[i] == FAT_FREE) {
            // check size of file to see if more blocks needed (later)
            //fat[i] = FAT_EOF;
            return firstFree = i;
        }
    }
}

int
FS::createFileInDirectoryTable(std::string filepath, std::string content, uint8_t *tempBlock)
{
    // int numOfBlocks = (content.size() / BLOCK_SIZE) + 1;
    // int previousBlock = 0;
    // int blocksCreated = 0;
    // int firstBlock = 0;

    // //kolla storlek på content - om litet nog allokera 1 block
    // //om större än 1 block allokera x antal block med pekare till varandra

    // int firstFreeFat = findFirstFreeFatSlot();

    // for(int i = 2; i < TABLE_SIZE; i++) {
    //     if(fat[i] == FAT_FREE)
    //         if(blocksCreated == 0)
    //             firstBlock = i;
    //             if(numOfBlocks == 1) // first block to be created
    //                 fat[i] == FAT_EOF;
    //         else if(blocksCreated < numOfBlocks - 1) // check if not last block to be created
    //             fat[previousBlock] = i;
    //         else    // last block to be created
    //             fat[previousBlock] = i;
    // }
    while( something ) 
    {
        int firstFreeFat = findFirstFreeFatSlot();
        int sizeOfContent = content.size();

        for(int i = 0; i < FILESYSTEM_SIZE; ++i)
        {
            if(sizeof(filepath) > 56) {
            std::cout << "FS::create failed, name too long." << std::endl;
            return -1;
            }
            if(content.size() < BLOCK_SIZE || sizeOfContent < BLOCK_SIZE)
            {
                directory_table[i].first_blk = firstFreeFat;
                directory_table[i].size = content.size();
                directory_table[i].type = TYPE_FILE;
                directory_table[i].access_rights = READ;
                fat[firstFreeFat] = FAT_EOF;

                writeFAT();
                writeFAT_directory();
                writeContentToDisk(firstFreeFat, tempBlock);
                break;
            }
            if(content.size() > BLOCK_SIZE && sizeOfContent > BLOCK_SIZE) // how much bigger?? 2?? 3?? 4?? 11??
            {
                directory_table[i].first_blk = firstFreeFat;
                directory_table[i].size = content.size();
                directory_table[i].type = TYPE_FILE;
                directory_table[i].access_rights = READ;

                writeFAT();
                writeFAT_directory();
                writeContentToDisk(firstFreeFat, tempBlock);

            }
        }
    
    }
    

    for(int i = 0; i < FILESYSTEM_SIZE; ++i){
        if(directory_table[i].type == TYPE_FREE){
            directory_table[i].first_blk = firstBlock;
        }
        // check if name is valid
        if(sizeof(filepath) > 56) {
            std::cout << "FS::create failed, name too long." << std::endl;
            return -1;
        }
        strcpy(directory_table[i].file_name, filepath.c_str());

        directory_table[i].size = content.size();
        directory_table[i].type = TYPE_FILE;
        directory_table[i].access_rights = READ;
        break;
    }
    writeFAT();
    writeFAT_directory();
    writeContentToDisk(firstFreeFat, tempBlock);
    
}

// retrieves the file from a path, "mainDir/subDir/file33" --> returns "file33"
std::string FS::retrieveFilename(std::string path) {
    int pos = path.find_last_of('/');

    if(pos != -1) { // check to see if there are any subfolders
        return path.substr(pos+1, path.size());
    }
    // otherwise the path is just the filename, return path
    return path;
}

// retrieves and returns first_blk in directory_table
uint16_t FS::findBlock(std::string filepath) {

    for(int i = 0; i < FILESYSTEM_SIZE; i++) {
        if(directory_table[i].file_name == filepath) {
            return directory_table[i].first_blk;
        }
    }
}

// dir_entry* FS::readFromDirectory(int block = 0)
// {
//     uint8_t* dir = new uint8_t[BLOCK_SIZE];
//     disk.read(block, dir);
//     return (dir_entry*) dir;
// }

// #########

// create <filepath> creates a new file on the disk, the data content is
// written on the following rows (ended with an empty row)
int
FS::create(std::string filepath)
{
    std::cout << "FS::create(" << filepath << ")\n";

    std::string content;
    std::string input;
    uint8_t tempBlock[BLOCK_SIZE];
    content.clear();

    while(getline(std::cin, input)) {

        if(input == "") {
            content += '\0';
            break;
        }
        content += input;
        content += '\n';
    }

    for(int i = 0; i < content.size(); i++) {
        tempBlock[i] = content[i];
    }

    createFileInDirectoryTable(filepath, content, tempBlock);

    return 0;
}

// cat <filepath> reads the content of a file and prints it on the screen
int
FS::cat(std::string filepath)
{
    std::cout << "FS::cat(" << filepath << ")\n";

    // måste även titta om filen sträcker sig över flera block
    std::string name = retrieveFilename(filepath);
    std::cout << name << std::endl;
    int block = findBlock(name);
    std::cout << block << std::endl;

    uint8_t emptyBlock[BLOCK_SIZE];

    while(block != FAT_EOF) {

        disk.read(block, emptyBlock);
        block = fat[block];
        std::cout << emptyBlock << std::endl;
    }

    return 0;
}

// ls lists the content in the currect directory (files and sub-directories) -- vänta med dennna tills directiory är implementerad
int
FS::ls()
{
    std::cout << "FS::ls()\n";

    int i;
    for(i = 0; i < FILESYSTEM_SIZE; i++)
    {
        if(directory_table[i].type != TYPE_FREE) {
            std::cout << "\n| Filename | Size |" << std::endl;
            std::cout << "    " << directory_table[i].file_name << "  " << directory_table[i].size << "  " << std::endl;
            std::cout << "\n" << std::endl;
        }
    }
    return 0;
}

// ##########

// Vi kanske måste göra en funktion som splittar upp filepathsen för att
// underlätta att hitta rätt beroende hur långa de är.

// bara en fundering

// ##########

// cp <sourcefilepath> <destfilepath> makes an exact copy of the file
// <sourcefilepath> to a new file <destfilepath>
int
FS::cp(std::string sourcefilepath, std::string destfilepath)
{
    std::cout << "FS::cp(" << sourcefilepath << "," << destfilepath << ")\n";
    std::string content;

    std::string oldFileName = retrieveFilename(sourcefilepath);
    int block = findBlock(oldFileName);

    uint8_t emptyBlock[BLOCK_SIZE];

    disk.read(block, emptyBlock);

    for(int i = 0; i < sizeof(emptyBlock); i++)
    {
        content += emptyBlock[i];
    }

    createFileInDirectoryTable(destfilepath, content, emptyBlock);


    return 0;
}

// mv <sourcepath> <destpath> renames the file <sourcepath> to the name <destpath>,
// or moves the file <sourcepath> to the directory <destpath> (if dest is a directory)
int
FS::mv(std::string sourcepath, std::string destpath)
{
    std::cout << "FS::mv(" << sourcepath << "," << destpath << ")\n";

    for(int i = 0; i < FILESYSTEM_SIZE; i++) {
        if(directory_table[i].file_name == sourcepath) {
            for(int j = 0; j < FILESYSTEM_SIZE; j++) {
                if(directory_table[i].file_name == destpath)
                    if(directory_table[j].type == TYPE_FILE) // if it is a file, just rename it
                        strcpy(directory_table[i].file_name, destpath.c_str());
                    else if(directory_table[j].type == TYPE_DIR) // if it is a directory, move the file to the directory
                        // to be continued
                    return 0;
            }
        }
        else
            std::cout << "File " << sourcepath << " does not exist.\n"; 
    }

    return 0;
}

// rm <filepath> removes / deletes the file <filepath>
int
FS::rm(std::string filepath)
{
    std::cout << "FS::rm(" << filepath << ")\n";
    // man kanske måste göra en titt så accessrights stämmer

    for(int j = 0; j < FILESYSTEM_SIZE; j++) {
        if(directory_table[j].file_name == filepath) {
            directory_table[j].type = TYPE_FREE;
            int blk = directory_table[j].first_blk;

            // njaa, kanske
            while(blk != FAT_EOF) {
                fat[blk] = FAT_FREE;
                blk = fat[blk];
            }
        }
    }

    // detta borde egentligen räcka för att markera en fil som "borttagen"! (tror jag)
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

    // std::string parentDirectory = "..";
    // int directoryIndex = 0;
    // int directoryIndexInParent = 0;
    // bool freeSpace = false;

    // if(dirpath.length() > 56) {
    //     std::cout << "The name of the new sub-directory is too long." << std::endl;
    //     return -1;
    // }
    
    // for(int i = 2; i < TABLE_SIZE; i++) {
    //     if(fat[i] == FAT_FREE)
    //         freeSpace = true;
    //         directoryIndex = i;
    //         break;
    // }

    // if(freeSpace == false) {
    //     std::cout << "FAT Table is full." << std::endl;
    //     return -1;
    // }

    // freeSpace= false;
    // for(int j = 0; j < FILESYSTEM_SIZE; j++) {
    //     if(directory_table[j].type == TYPE_FREE)
    //         directoryIndexInParent = j;
    //         freeSpace = true;
    //         break;
    // }

    // for(int k = 1; k < FILESYSTEM_SIZE; k++) {
    //     if(directory_table[k].file_name == dirpath && directory_table[k].type == TYPE_DIR)
    //         std::cout << "Another folder with this name already exists." << std::endl;
    //         disk.read(currentDir, (uint8_t*)directory_table);
    //         return -1;
    // }

    // if(freeSpace == false) {
    //     std::cout << "Directory is full." << std::endl;
    //     return -1;
    // }

    // disk.read(currentDir, (uint8_t*)directory_table);

    // strcpy(directory_table[directoryIndexInParent].file_name, dirpath.c_str());
    // directory_table[directoryIndexInParent].type = TYPE_DIR;
    // directory_table[directoryIndexInParent].first_blk = directoryIndex;
    // disk.write(currentDir, (uint8_t*)directory_table);

    // strcpy(directory_table[0].file_name, parentDirectory.c_str());
    // directory_table[0].type = TYPE_DIR;
    // directory_table[0].first_blk = currentDir;
    // fat[directoryIndex] = FAT_EOF;

    // for(int i = 1; i < FILESYSTEM_SIZE; i++) {
    //     directory_table[i].type = TYPE_FREE;
    // }

    // disk.write(directoryIndex, (uint8_t*)directory_table);
    // disk.write(currentDir, (uint8_t*)directory_table);
    // writeFAT();

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
