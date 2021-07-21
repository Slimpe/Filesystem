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

<<<<<<< Updated upstream
void
FS::writeContentToDisk(unsigned block, std::string content){

    disk.write(block, (uint8_t*) &content);
=======
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
>>>>>>> Stashed changes
}

// dunno if this function is nessecary
int
FS::findFirstFreeFatSlot()
{
    int firstFree;
    for(int i = 2; i < TABLE_SIZE; i++)
    {
        if(fat[i] == FAT_FREE) {
            // check size of file to see if more blocks needed (later)
            fat[i] = FAT_EOF;
            return firstFree = i;
        }
    }
}

<<<<<<< Updated upstream
=======
int
FS::writeContentToDisk(std::string filepath, std::string content, uint8_t *tempBlock)
{
    int numOfBlocks = (content.size() / BLOCK_SIZE) + 1;
    int previousBlock = 0;
    int blocksCreated = 0;
    int firstBlock = 0;

    for(int i = 2; i < TABLE_SIZE; i++) {
        if(fat[i] == FAT_FREE)
            if(blocksCreated == 0)
                firstBlock = i;
                if(numOfBlocks == 1) { // first block to be created
                    fat[i] == FAT_EOF;
                    break;
                }
            else if(blocksCreated < numOfBlocks - 1) // check if not last block to be created
                fat[previousBlock] = i;
            else    // last block to be created
                fat[previousBlock] = i;
                fat[i] = FAT_EOF;
                break;

            previousBlock = i;
            blocksCreated++;
    }

    // SIMONS VERSION
    // while( something ) 
    // {
    //     int firstFreeFat = findFirstFreeFatSlot();
    //     int sizeOfContent = content.size();

    //     for(int i = 0; i < FILESYSTEM_SIZE; ++i)
    //     {
    //         if(sizeof(filepath) > 56) {
    //         std::cout << "FS::create failed, name too long." << std::endl;
    //         return -1;
    //         }
    //         if(content.size() < BLOCK_SIZE || sizeOfContent < BLOCK_SIZE)
    //         {
    //             directory_table[i].first_blk = firstFreeFat;
    //             directory_table[i].size = content.size();
    //             directory_table[i].type = TYPE_FILE;
    //             directory_table[i].access_rights = READ;
    //             fat[firstFreeFat] = FAT_EOF;

    //             writeFAT();
    //             writeFAT_directory();
    //             writeContentToDisk(firstFreeFat, tempBlock);
    //             break;
    //         }
    //         if(content.size() > BLOCK_SIZE && sizeOfContent > BLOCK_SIZE) // how much bigger?? 2?? 3?? 4?? 11??
    //         {
    //             directory_table[i].first_blk = firstFreeFat;
    //             directory_table[i].size = content.size();
    //             directory_table[i].type = TYPE_FILE;
    //             directory_table[i].access_rights = READ;

    //             writeFAT();
    //             writeFAT_directory();
    //             writeContentToDisk(firstFreeFat, tempBlock);
    //             break;
    //             //i donno anymore... :d
    //         }
    //     }
    
    // }
    
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
    disk.write(firstBlock, tempBlock);

    return 0;
}

std::string
FS::readContentFromDisk(int block, uint8_t* emptyBlock) {

    std::string res;

    while(block != FAT_EOF) {

        disk.read(block, emptyBlock);
        block = fat[block];
    }

    // vetifan
    for(int i = 0; i < sizeof(emptyBlock); i++) {
        res[i] = emptyBlock[i];
    }
    return res;
}

>>>>>>> Stashed changes
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
        else
            return -1;
    }
}

<<<<<<< Updated upstream
// #########
=======
// dir_entry* FS::readFromDirectory(int block = 0)
// {
//     uint8_t* dir = new uint8_t[BLOCK_SIZE];
//     disk.read(block, dir);
//     return (dir_entry*) dir;
// }

// ####### TASK 1 ########
>>>>>>> Stashed changes

// create <filepath> creates a new file on the disk, the data content is
// written on the following rows (ended with an empty row)
int
FS::create(std::string filepath)
{
    std::cout << "FS::create(" << filepath << ")\n";

    // input reads up to, but not including, the first whitespace!!!!
    std::string content;
    content.clear();
    std::string input;
<<<<<<< Updated upstream
    //char content[BLOCK_SIZE];
    content.clear();
=======
    uint8_t tempBlock[BLOCK_SIZE];
>>>>>>> Stashed changes

    while(std::getline(std::cin, input)) {

        if(input == "") {
            content += '\0'; // null character at the end of the string
            break;
        }
        content += input;
        content += '\n';
    }
    
    //search for a free slot in FAT
    int firstFreeFat = findFirstFreeFatSlot();

    // search for a free slot in directory_table
    for(int j = 0; j < FILESYSTEM_SIZE; j++) {

<<<<<<< Updated upstream
        if(directory_table[j].type == TYPE_FREE) 
        {
            directory_table[j].first_blk = firstFreeFat;

            // check if name is valid
            if(sizeof(filepath) > 56) {
                std::cout << "FS::create failed, name too long." << std::endl;
                return -1;
            }
            
            strcpy(directory_table[j].file_name, filepath.c_str());
            
            directory_table[j].size = content.size();
            directory_table[j].type = TYPE_FILE;
            directory_table[j].access_rights = READ;
            break;
        }
    }
    writeFAT();
    writeFAT_directory();
    std::cout << firstFreeFat << std::endl;
    writeContentToDisk(firstFreeFat, content);
=======
    writeContentToDisk(filepath, content, tempBlock);
>>>>>>> Stashed changes

    return 0;
}

// cat <filepath> reads the content of a file and prints it on the screen
int
FS::cat(std::string filepath)
{
    std::cout << "FS::cat(" << filepath << ")\n";

    // måste även titta om filen sträcker sig över flera block
    std::string content;
    content.clear();
    std::string name = retrieveFilename(filepath);
    std::cout << name << std::endl; // axel
    uint16_t block = findBlock(name);
    std::cout << block << std::endl; // 2

    char emptyBlock[BLOCK_SIZE];

    uint8_t* temp = reinterpret_cast<uint8_t*>(&emptyBlock);

    // måste även titta om access rights är READ
    // read from disk
    //disk.read(block, (uint8_t*)&content);
    disk.read(block, temp);

    // uint32_t test;
    // for(int i = 0; i < FILESYSTEM_SIZE; i++) {
    //     if(directory_table[i].file_name == filepath) {
    //         test = directory_table[i].size;
    //     }
    // }

    int j = 0;
    while(j < BLOCK_SIZE) {
        content += emptyBlock[j];
        j++;
    }

    std::cout << content;

    // if(temp.empty()) {
    //     return -1;
    // }
    
    return 0;
}

// ls lists the content in the currect directory (files and sub-directories)
int
FS::ls()
{
    int i;
    for(i = 0; i < FILESYSTEM_SIZE; i++)
    {
        if(directory_table[i].type != TYPE_FREE) {
            std::cout << directory_table[i].file_name << std::endl;
            //std::cout << "\n" << std::endl;
        }
    }

    std::cout << "FS::ls()\n";

    return 0;
}

// ########################

// ######## TASK 2 ########

// cp <sourcefilepath> <destfilepath> makes an exact copy of the file
// <sourcefilepath> to a new file <destfilepath>
int
FS::cp(std::string sourcefilepath, std::string destfilepath)
{
    std::cout << "FS::cp(" << sourcefilepath << "," << destfilepath << ")\n";
<<<<<<< Updated upstream
=======

    std::string content;
    content.clear();
    std::string oldFileName = retrieveFilename(sourcefilepath);
    std::string newFileName = retrieveFilename(destfilepath);
    int block = findBlock(oldFileName);
    int destBlock = findBlock(newFileName);

    if(block != 0) {
        std::cout << "File " << sourcefilepath << " does not exist." << std::endl;
        return -1;
    }
    if(destBlock != -1) {
        std::cout << "File " << destfilepath << " already exist." << std::endl;
        return -1;
    }

    uint8_t emptyBlock[BLOCK_SIZE];

    disk.read(block, emptyBlock);

    for(int i = 0; i < sizeof(emptyBlock); i++)
    {
        content += emptyBlock[i];
    }

    writeContentToDisk(destfilepath, content, emptyBlock);

>>>>>>> Stashed changes
    return 0;
}

// mv <sourcepath> <destpath> renames the file <sourcepath> to the name <destpath>,
// or moves the file <sourcepath> to the directory <destpath> (if dest is a directory)
int
FS::mv(std::string sourcepath, std::string destpath)
{
    std::cout << "FS::mv(" << sourcepath << "," << destpath << ")\n";
<<<<<<< Updated upstream
=======

    // vetifan va för skit jag har skrivit här :8

    for(int i = 0; i < FILESYSTEM_SIZE; i++) {
        if(directory_table[i].file_name == sourcepath) {
            for(int j = 0; j < FILESYSTEM_SIZE; j++) {
                if(directory_table[j].file_name == destpath)
                    if(directory_table[j].type == TYPE_FILE) // if it is a file, just rename it
                        strcpy(directory_table[i].file_name, destpath.c_str());
                    else if(directory_table[j].type == TYPE_DIR) // if it is a directory, move the file to the directory
                        // to be continued
                    return 0;
                // else
            }
        }
        else
            std::cout << "File " << sourcepath << " does not exist.\n"; 
    }

>>>>>>> Stashed changes
    return 0;
}

// rm <filepath> removes / deletes the file <filepath> --- kan göras
int
FS::rm(std::string filepath)
{
<<<<<<< Updated upstream
    // 
=======
    std::cout << "FS::rm(" << filepath << ")\n";

    // man kanske måste göra en titt så accessrights stämmer
>>>>>>> Stashed changes

    for(int j = 0; j < FILESYSTEM_SIZE; j++) {
        if(directory_table[j].file_name == filepath) {

        }
    }

    std::cout << "FS::rm(" << filepath << ")\n";
    return 0;
}

// append <filepath1> <filepath2> appends the contents of file <filepath1> to
// the end of file <filepath2>. The file <filepath1> is unchanged.
int
FS::append(std::string filepath1, std::string filepath2)
{
    std::cout << "FS::append(" << filepath1 << "," << filepath2 << ")\n";

    int block1 = findBlock(filepath1);
    int block2 = findBlock(filepath2);
    uint8_t filepath1Content[BLOCK_SIZE];
    uint8_t filepath2Content[BLOCK_SIZE];
    uint8_t res[BLOCK_SIZE];

    if(block1 != 0) {
        std::cout << "File " << filepath1 << " does not exist." << std::endl;
        return -1;
    }
    if(block2 != 0) {
        std::cout << "File " << filepath2 << " does not exist." << std::endl;
        return -1;
    }

    std::string content1 = readContentFromDisk(block1, filepath1Content);
    std::string content2 = readContentFromDisk(block2, filepath2Content);
    content2 += content1;

    for(int j = 0; j < content2.size(); j++) {
        res[j] = content2[j];
    }

    // här kanske man måste radera 'filepath2' genom rm(filepath2) och allokera den på nytt istället ????????

    disk.write(block2, res);

    return 0;
}

// ########################

// ######## TASK 3 ########

// mkdir <dirpath> creates a new sub-directory with the name <dirpath>
// in the current directory ---- kan göras
int
FS::mkdir(std::string dirpath)
{
    std::cout << "FS::mkdir(" << dirpath << ")\n";
    return 0;
}

// cd <dirpath> changes the current (working) directory to the directory named <dirpath> -- kan göras
int
FS::cd(std::string dirpath)
{
    std::cout << "FS::cd(" << dirpath << ")\n";
    return 0;
}

// pwd prints the full path, i.e., from the root directory, to the current
// directory, including the currect directory name --- kan göras
int
FS::pwd()
{
    std::cout << "FS::pwd()\n";
    return 0;
}

// ########################

// ######## TASK 5 ########

// chmod <accessrights> <filepath> changes the access rights for the
// file <filepath> to <accessrights>. -- kan göras
int
FS::chmod(std::string accessrights, std::string filepath)
{
    std::cout << "FS::chmod(" << accessrights << "," << filepath << ")\n";
    return 0;
}

// ########################
