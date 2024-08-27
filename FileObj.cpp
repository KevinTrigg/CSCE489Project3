File space:

256 blocks of 256 Bytes 64 kB size
bit map of available blocks  0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000 //each 0 is it's own block 
write to each block 
read from block
find available block
update bitmap of block 
if block is updated to null free the block

first 4 blocks reserved for file/directory headers



Directory information "root" directory of data:
//files in directory (location in control block) 
//create file
//delete file
//list
//create directory
//delete directory

file info storage:
file name 32 bytes 
type 1 byte (only going to use default .txt as 0000)
list of occupied blocks  each file has max size of 8 blocks (attempting to exceed this leads to error for file size)
{control block array each block occupied is 8 bits for the location: 00110010, 10100100, zero block =end of file 00000000}
file size byte 3 bytes 0/1 within max size 000 0000 0000 addressing



position of last byte in last block (for tail writing) 00000000 - 11111111 once 11111111 is reached a new block is needed

withhold 128 bytes for file info storage
