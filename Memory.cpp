#include <stdio.h> 
#include "Memory.h"

const int numberOfBlocks=256;   //how many blocks are in the memory
const int fileNameLength =32;   //how long a files name can be
const int maxFileCapacity=16;   //max number of files in the memory
const int bitMapOffset=32;      //number of byte offset to make space in mem for the bit map of the occupied blocks
const int fileHeaderLength=64; //size of each file header at start of mem behind the bitmap
const int blockSize=256;        //size of the blocks in bytes  
const int blocksInAFile=8;      //max number of blocks occupied by a file
const int blockIDLength=1;      //related to number of blocks, which specific block is indexed roundup(log(blockSize)/8)
const int fileTypeSize=1;       //size of the definition of the file type in the header
const int fileSizeLength=3;     //# of bytes to represent file size in bytes 2^11 =2048 extra bit used for spacing ease
char *mem;                      //array of chars used as memory


Memory::Memory(){
  this->mem = new char[256*blockSize]; // block size of 256, 256 blocks 
  for(int i=0;i<sizeof(mem);i++){ //go through the full memory object
    if(i<1){
      mem[i]=0xF0; //reserve first 4 blocks for the bitmap and file headers
    }else{
      mem[i]=0x00;  //ensure they are created, but 0 
    }
  }
}

Memory::Memory(char mem[]){ //if the user already has a memory allow them to copy it to this
  this->mem=mem;
}

Memory::~Memory(){  //destructor of memory
  delete this->mem;
}



void Memory::createFile(char name[]){
//check basic creation requirements
  if (sizeof(name)>=fileNameLength){
    printf("name exceeds limits ensure name is less than %d\n",fileNameLength);
    return; //exit early, no allocation exit early, no allocation exit early, no allocation exit early, no allocation exit early, no allocation exit early, no allocation
  } else if(name[0] ==0x00){
    printf("first character can't be null.\n"); //easy check for if file name space is already occupied or free
    return; //exit early, no allocation exit early, no allocation exit early, no allocation exit early, no allocation exit early, no allocation exit early, no allocation
  }
  //check for nuanced file creation needs
  bool openFileSlot =false; //check for if there's an open slot to put a file
  for(int i=0;i<maxFileCapacity;i++){
    if(mem[(bitMapOffset + i*fileHeaderLength)]!=0x00){ //check if this file name location is taken
      bool alreadyCreated=true;
    
      for(int j=0;j<fileNameLength;j++){
        if(name[j]!=mem[j+(bitMapOffset + i*fileHeaderLength)]){  //test the provided name against the existing file name
          alreadyCreated=false; //if there is a difference then it's unique and can be used
        } 
      }
      
      if(alreadyCreated){ //if the name ISN'T unique, it can't be used
        printf("this file name already exists, name must be unique\n");
        return; //exit early, no allocation exit early, no allocation exit early, no allocation exit early, no allocation exit early, no allocation exit early, no allocation
      } 
    } else {
      openFileSlot=true;
    }
  }
  
  if(!openFileSlot){  //if no open slots were detected during the unique name check then exit
    printf("no open slot for a file, file cannot be created\n");
    return; //exit early, no allocation exit early, no allocation exit early, no allocation exit early, no allocation exit early, no allocation exit early, no allocation
  }
  //if the file creation passes all errors, proceed to creating it
  for(int i=0;i<maxFileCapacity;i++){
    if(mem[(bitMapOffset + i*fileHeaderLength)]==0x00){ //check if this file name location is free
      for(int j=0;j<fileNameLength;j++){  //write the file name
        mem[j+(bitMapOffset + i*fileHeaderLength)] =name[j]; 
      }
      for(int j=0; j<fileHeaderLength-fileNameLength;j++){
        mem[fileNameLength+(bitMapOffset + i*fileHeaderLength) +j ] =0x00;  //clear any junk data for the header infomation 
      }
      mem[(bitMapOffset + i*fileHeaderLength) + fileNameLength ] =0x00;  //set file type to .txt defaul 0x00
      //all other blocks of file header are default null anyways so we don't need to adjust them


      break;  //if the space was open we only need to fill one slot, stop traversing the file name spaces
    }
  }
}
  
void Memory::write( char name[], char data[]){
  char* writeLocation = new char(2);
  writeLocation[0]=0x00;//block location
  writeLocation[1]=0x00;//block offset
  writeLocation = findWriteLocation(name, writeLocation);
  if(writeLocation[0] == 0x00 && writeLocation[1] == 0x00){ //null, values == errored out, don't write
  printf("error writing\n");
    return; //breaks condition, it would overwrite the bitmap
  }

  for(int write =0; write< sizeof(data); write++){
    mem[writeLocation[0]*256+writeLocation[1]]=data[write];
    writeLocation[1]= char((int(writeLocation[1])+1)%blockSize);  //increment the block size and if it's at the end of the block w
    if(writeLocation[1]==0x00){
      writeLocation = findWriteLocation(name, writeLocation);
      if(writeLocation[0] == 0x00 && writeLocation[1] == 0x00){ //null, values == errored out, don't write
          return; //breaks condition, it would overwrite the bitmap
      }
    }  
  }
  //TODO: given write location, write to memory at that location then update file for occupied blocks, block offset, and file size 


  return;
}


//return concatenated array of block number to write and position in block
char* Memory::findWriteLocation(char name[],char returnable[] ){
  bool nameMatch=true;
  char tempBlockID= 0x00; //create a temp ID to find the last non 0 blockID in the file
  char blockOffset= 0x00; //what byte within the block to write to next
  // returnable: location is block position, offset in block
  for(int i=0;i<maxFileCapacity;i++){
    if(mem[(bitMapOffset + i*fileHeaderLength)]==0x00){ //check if this file name location is occupied
      for(int j=0;j<fileNameLength;j++){  //check the file name
        if(mem[j+(bitMapOffset + i*fileHeaderLength)]!=name[j]){ // if the name doesn't match set match to false
          nameMatch=false;  
        } 
      }

      if(nameMatch){
        blockOffset= mem[bitMapOffset + (i*fileHeaderLength) + fileNameLength + fileTypeSize + blocksInAFile*blockIDLength +  fileSizeLength ]; ;
        //load offset from  the file in question past the filename,type specifier,blocks,file size to get nextByte
        for(int blockCheck= blocksInAFile-1; blockCheck<=0; blockCheck--){  //find the last block used by the file
          //check through the files block used space in reverse order
          //find the first non null value, if the nextByte is not equal to block size then use that block
          //if the nextByte IS equal to block size, get a new block
                   
            tempBlockID = mem[bitMapOffset + (i*fileHeaderLength) + fileNameLength +   fileTypeSize        +     blockCheck*blockIDLength]; 
            //          offset by map             name of file    how long the name is  what type of file byte    what block is loaded     byte of that blockID
            // load the next byte to the temp 

          if(tempBlockID != 0x00){ //the first non null should be used
            if(blockOffset==0x00){
              if(blockCheck==blocksInAFile-1){
                printf("max size reached, can't write more\n");
                return returnable;  
                //if the last block (8) has an offset of 0 that means it needs a new block.since 8 is the max it means it can't be writen, error
              }
              //if the block offset is 0 then a new block needs to be allocated (done on the start of new block not end of last block)
              tempBlockID = findEmptyBlock(); //
              mem[bitMapOffset + (i*fileHeaderLength) + fileNameLength + fileTypeSize + (blockCheck+1)*blockIDLength]= tempBlockID;  
                                                                                  //write the NEXT block to be the new block
              }
            }
          }
          
        }
        mem[fileNameLength+(bitMapOffset + i*fileHeaderLength) ] =0x00;  //set file type to .txt defaul 0x00
        //all other blocks of file header are default null anyways so we don't need to adjust them
        break;  //if the space was open we only need to fill one slot, stop traversing the file name spaces
    }
  }
  returnable[0] =tempBlockID;
  returnable[1]=blockOffset;
  return returnable;
}

char findEmptyBlock(){
  //need a new block
  char bitFromByte=0x00;
  char tempBlockID=0x00;
  for(int nextAvailableBlock= 0; nextAvailableBlock<bitMapOffset; nextAvailableBlock++ ){ //traverse the bitmap to find an available block
    if(mem[nextAvailableBlock] < char(0xFF)){  //if the byte isn't FF that means there's a 0 (unused block) that can be used
      bitFromByte = mem[nextAvailableBlock];
      int bit=0;
      while (bit < 8) {         //https://stackoverflow.com/questions/26411131/iterate-through-bits-in-c 
        if (bitFromByte & 0x80) { //if the next bit (check from left) is 1 do nothing 
        }  else { //if it isn't one, we have an empty block
        //we have the bit that isn't 1, now we need to convert its location to a block index, apply that to the next block index of this file and pass that and 
          tempBlockID= char(nextAvailableBlock*8+bit);
          //set the temp block ID to  
          break;
        }

        bit++;
        bitFromByte = bitFromByte << 1;
      }
   // mem[nextAvailableBlock]
    }

  }
  return tempBlockID;
}

void Memory::dir(){
  //list the files in the memory space. IF directories were implemented they would add in order and depth dependent on directory location 
  for(int i=0;i<maxFileCapacity;i++){
    if(mem[bitMapOffset + i*fileHeaderLength]!=0x00){ //check if this file name location is taken
   
      for(int j=0;j<fileNameLength;j++){//print out the name of the file 
        printf("%c" ,mem[(bitMapOffset + i*fileHeaderLength)+ j ]);  
                //offset byte by the map, what file is being read, and the byte within the file name
      }
      printf("\n"); //new line for the next file to be printed
      
    } 
  }
}

void Memory::list(char fileName[]){
  //print out the contents of the file 
  //traverse through the blocks of the file (those that are occupied) and print out their contents
  int whichFile = 0;  //find what file needs to be listed out based on the file name
  bool nameMatches=true; 
  
  for(int i=0;i<maxFileCapacity;i++){ //go through each file in the file explorer
      if(mem[(bitMapOffset + i*fileHeaderLength)]==0x00){ //check if this file name location is occupied
        nameMatches=true; //set the expectation that this it the file 

        for(int j=0;j<fileNameLength;j++){  //check the file name
          if(mem[j+(bitMapOffset + i*fileHeaderLength)]!=fileName[j]){ // if the name doesn't match set match to false
            nameMatches=false;  
         } 
        }
        if (nameMatches){//if the file is successfully found
          // find the location of the first block from the file then go through it.
          for(int itterateThroughBlocks = 0; itterateThroughBlocks < blocksInAFile; itterateThroughBlocks++){
            char blockLocation = mem[bitMapOffset + (i*fileHeaderLength) + fileNameLength + fileTypeSize + itterateThroughBlocks*blockIDLength];
            //once we have the location we need to test if it's valid then read memory from that block
            if (blockLocation==0x00){
              printf("reached the end of the file\n");
              break;  //the block is null so we don't need to try the rest of the blocks
            }
            for(int blockByte=0; blockByte < blockSize; blockByte++){ //print out the contents of the block byte by byte
              printf("%c",mem[int(blockLocation) +blockByte]);
            }
          }
        }
      }
  }
}
