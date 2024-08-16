/*************************************************************************************
 * shellfuncts.c - code definitions for your functions
 *
 *void commandLoop(void) loop through taking commands called by main
 * void takeInput(void)  scan input from user in console and parse result to commands
 * void create (char fileName[]) make a new file in the shells directory with the input name
 * void update(char fileName[], int writeLineNumTimes, char text[]) append a block of text to the end of the referenced file a *                                                                   specified amount of times
 *void list(char fileName[])  print the contents of the selected file to the console
 *void dir(void) print out the contents of the current directory
 *void newProcess(void) fork a new child, print their pid, and wait for childs completion if in foreground
 *************************************************************************************/

#include <stdio.h>    //whole big block of inclusions
#include <string.h>
#include "shellfuncts.h"
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/file.h>
#include <errno.h>

pid_t pid;  //global for pid storage
char ampersand= '\0'; //global for & check assigned in commandLoop but used in newProcess(), 
                      
                      
//***********************************************************************************
//commandLoop: loop through takeInput and keep the prompt the user for that input
//inputs: none
//return: none
//***********************************************************************************
void commandLoop(){
      pid = getpid(); //get and print the pid of the main process
      printf("\n\nparent loop pid:%d\n",pid);
        while(1){ //keep getting new input until the process is killed with the halt command
        takeInput();
        }
}


//***********************************************************************************
//takeInput: scan input from user in console and parse result to commands
//inputs: none
//return: none
//***********************************************************************************
void takeInput(){
	char commandInput[511];	  //large buffer for command input
        ampersand='\0';
	printf("shell command:"); //prompt the user for input
	scanf("%s",commandInput); //scan the first word in 
        //if block for command parsing. check the first word for what command was input then split from there
        if (strcmp(commandInput,"create")==0){  //check for new file creation
          char fileName[255]="\0"; //hold the name of the file
          //get the rest of the command (only the name and background check)
          fgets(commandInput,sizeof(commandInput),stdin); 
          sscanf(commandInput,"%s %c[^\n]",fileName,&ampersand);
          
          newProcess(); //spawn a new process and check for background
          if(pid==0){ //function call for child
            create(fileName);
          }
          


        }else if(strcmp(commandInput,"update")==0){ //write to a file
          char fileName[255]="\0";  //create the 3 variables needed for the command
          int writeLineNumTimes=0;
          char text[100]="\0";
          //get the rest of the command (name,repetition, text, and background check)
          fgets(commandInput,sizeof(commandInput),stdin);
          sscanf(commandInput,"%s %d \"%100[^\"]\" %c",fileName, &writeLineNumTimes, text,&ampersand);
          
          newProcess();//spawn a new process and check for background
          if(pid==0){ //function call for child
            update(fileName,writeLineNumTimes,text);
          }
                
                
                
        }else if(strcmp(commandInput,"list")==0){ //write the file out
          char fileName[255]="\0";  //create the needed variable
          //get the rest of the command (name and background check)
          fgets(commandInput,sizeof(commandInput),stdin);
          sscanf(commandInput,"%s ",fileName);
          
          newProcess();//spawn a new process and check for background
          if(pid==0){ //function call for child
            list(fileName);
          }
                     
            
              
        }else if(strcmp(commandInput,"dir")==0){//write the directory out
          newProcess();//spawn a new process and check for background
          if(pid==0){ //function call for child
            dir();
          }
          sleep(.2);//quick and dirty fix for execl() issue warping the child process and breaking console print
        
        
        
        }else if(strcmp(commandInput,"halt")==0){//kill the whole process and all children, complete through parent
          printf("\n halt called\n"); 
          kill(0, SIGTERM); //ensure all children are dead (brutal)
          //kill command found from link, and man pages on kill/signal
          // https://stackoverflow.com/questions/18433585/kill-all-child-processes-of-a-parent-but-leave-the-parent-alive 
          return; //ensure exit of the loop (just in case)
          
          
          
        }else{         //if the user inputs an improper command tell them
          printf("\nunknown command %s, please use a legal command.\n check for typos!\n",commandInput);
        }
}


//***********************************************************************************
//create: make a new file in the shells directory with the input name
//inputs: 
//      fileName: the name of the file to be created
//return: none
//***********************************************************************************
void create(char fileName[]){
  if(!strcmp(fileName,"\0")){ //check for existence of file name
    printf("Invalid name\n"); //if nonexistent, exit without attempting creation and tell the user
    exit(0);
  }
  
  FILE *file; //make the pointer to the file location
  if(fopen(fileName,"r")){  //test to see if the file already exists
    printf("This file already exists!\n");
  } else{ //if it does then open the file to create it
    file= fopen(fileName,"w");
    fclose(file);	//close file immediately afterwards
    if(ampersand!='&'){ //don't print completion message if in background
      printf("file created\n");
    }
  }
  exit(0);
}


//***********************************************************************************
//update: append a block of text to the end of the referenced file a specified amount of times
//inputs: 
//      fileName: the name of the file to be created
//      writeLineNumTimes: how many times the block of text will be appended
//      text: block of text that will be appended to the file
//return: none
//***********************************************************************************
void update(char fileName[], int writeLineNumTimes, char text[]){
  if(!strcmp(fileName,"\0")){ //check that filename was input
    printf("Invalid name\n");
    exit(0);
  }else if (writeLineNumTimes<1){ //check that the line will be written at least once
    printf("must write at least once\n");
    exit(0);
  }else if(!strcmp(text,"\0")){   //check that there is anything to write
    printf("you must write something\n");
    exit(0);
  }
  
  FILE *file; //file pointer
  if(  fopen(fileName,"r") ==NULL){ //check for the existence of the file
    printf("file %s doesn't exist!\n",fileName);
  } else { 
   file =fopen(fileName,"a"); //append to the file
 
    for(int i=0;i<writeLineNumTimes;i++){ //append text to the file a specified amount of times
      fprintf(file,"%s\n",text);    //write the text
      fflush(stdout); //clear the buffer
      sleep(strlen(text)/5);  //sleep so it can be tested
    }
    fclose(file); //close file once complete
    if(ampersand!='&'){ //don't write success message if in background
      printf("file %s successfully updated by process: %d\n",fileName,pid);
    }
  }
  exit(0);  //kill child
}


//***********************************************************************************
//list: print the contents of the selected file to the console
//inputs: 
//      fileName: the name of the file to be created
//return: none
//***********************************************************************************
void list(char fileName[]){
  if(!strcmp(fileName,"\0")){   //check valid file name
    printf("Invalid name\n");
    exit(0);
  }
  FILE *file;
  if(  fopen(fileName,"r") ==NULL){ //check that the file exists
    printf("File \"%s\" doesn't exist!\n",fileName);
  }else{
     file = fopen(fileName,"r"); //read from the file
  printf("\n");
    //file reading loop taken from www.geeksforgeeks.org/c-program-print-contents-file/ 
    char nextCharacter=fgetc(file); //get next character
    while (nextCharacter!=EOF){ //while the character isn't the end of the file
      printf("%c",nextCharacter);//print the character to terminal
      nextCharacter=fgetc(file);//get the next character to read/print
    }
    fclose(file);//close the file
  }
  if(ampersand!='&'){ //don't print completion message if in background
    printf("list completed\n");
  }
  exit(0);
}


//***********************************************************************************
//dir: print out the contents of the current directory
//inputs: 
//      none
//return: none
//***********************************************************************************
void dir(){
execl("/bin/ls","ls",NULL); //provided in assignment, call the system to execute an ls command from console
exit(0);
}


//***********************************************************************************
//newProcess: fork a new child, print their pid, and wait for childs completion if in foreground
//          issue: if list ran in background it disjoints prompt for input due to console print extending to dir if commanded
//                  immediately afterwards, any other command fixes the issue. 
//inputs: none
//return: none
//***********************************************************************************
void newProcess(){
  pid = fork(); //create new child and record its pid
  if(pid!=0) {
     printf("child pid: %d\n",pid); //print the childs pid
     if(ampersand != '&'){  //if it's a background task don't wait for it and move on
       wait(NULL);  //wait for the child to exit before continuing main execution
     }
  }
}
