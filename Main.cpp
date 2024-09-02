#include <stdio.h>
#include "Memory.h"
#include <string>



int main(){

Memory *mem = new Memory();



char fileName1[] = "Mike Smith";
char* pname1 = &fileName1[0];
char fileContent1[] = "Dragic";
char* content1F1 = &fileContent1[0];
char str[] = "Mike Smith";
char* pname = &str[0];
char str2[] = "Dragic";
char* pteam = &str2[0];

mem->createFile(pname);
mem->dir();
mem->write(pname,pteam);
mem->list(pname);
mem->createFile(pname);
mem->dir();
mem->write(pname,pteam);
mem->list(pname);
}
