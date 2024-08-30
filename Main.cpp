#include <stdio.h>
#include "Memory.h"
#include <string>



int main(){

Memory *mem = new Memory();


char myword[] = "Hello";
char str[] = "Mike Smith";
char str2[] = "Dragic";

char* pname = &str[0];
//char* pteam = &str2[0];


mem->createFile(pname);

}