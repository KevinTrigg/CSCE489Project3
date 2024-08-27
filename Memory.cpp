#include <stdio.h> 
#include "Memory.h"

char mem[]; 
int fileNameLength =32;
int maxFileCapacity=24;
int bitMapOffset=32;

Memory::Memory(){
mem= new char[256*256]// block size of 256, 256 blocks 
  for(int i=0;i<mem.length();i++){ //go through the full memory object
    if(i<32){
      mem[i]=0xFF; //reserve first 256 bits for bitmap (always seen as used
    }else{
      mem[i]=0x00;//ensure they are created, but 0 (unnecessary?)
    }
  }
  
  
}

Memory::Memory(char[] mem){ //if the user already has a memory allow them to copy it to this
  this->mem=mem;
}

Memory::~Memory(){
delete [] mem;
}



void createFile(char name[]){
//check basic creation requirements
  if (name.length()>=fileNameLength){
    printf("name exceeds limits ensure name is less than %d\n",fileNameLength);
    return; //exit early, no allocation
  } else if(name[0] ==0x00){
    printf("first character can't be null.\n"); //easy check for if file name space is already occupied or free
    return;
  }
  //check for nuanced file creation needs
  for(int i=0;i<maxFileCapacity;i++){
    if(mem[j+(i*128+ bitMapOffset)]!=0x00){ //check if this file name location is taken
      bool alreadyCreated=true;
    
      for(int j=0;j<fileNameLength;j++){
        if(name[j]!=mem[j+(i*128+ bitMapOffset)]){  //test the provided name against the existing file names
          lreadyCreated=false;
        } 
      }
      
      if(alreadyCreated){
        printf("this file name already exists, name must be unique\n");
        return;
      } 
    }
  }
}
  
char[] write( char location[], char data[]){
  
}
