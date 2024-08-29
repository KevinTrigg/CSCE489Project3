#ifndef MEMORY_H
#define MEMORY_H
#include <memory.h>



class Memory{
    public:

        Memory();
        Memory(char[]);
        ~Memory();


        void createFile(char []);
        void write( char[], char[]);
        char* findWriteLocation(char[],char[]);
        char findEmptyBlock();
        void dir();
        void list(char []);
    private: 
        char *mem;               //array of chars used as memory
        int numberOfBlocks;     //how many blocks are in the memory
        int fileNameLength;    //how long a files name can be
        int maxFileCapacity;   //max number of files in the memory
        int bitMapOffset;      //number of byte offset to make space in mem for the bit map of the occupied blocks
        int fileHeaderLength; //size of each file header at start of mem behind the bitmap
        int blockSize;        //size of the blocks in bytes  
        int blocksInAFile;      //max number of blocks occupied by a file
        int blockIDLength;      //related to number of blocks, which specific block is indexed
        int fileTypeSize;    //size of the definition of the file type in the header
        int fileSizeLength;     //# of bytes to represent file size in bytes 2^11 =2048 extra bit used for spacing ease

};
#endif