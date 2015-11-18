/***
Author: Vishnu Dutt Sharma
    12EC35018

Main controller file, run the shell
Brief Function Description:
  mycd: change current working directory
  myls: list all files/directories in the current directory
  mymkdir: create directory
  mymv: move file/directory
  myps: show all process
  mypwd: show present working directory
  myrm: remove file/directory
  mytail: read some last line of a file
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

// Gloabal array for storing arguments in command line input
char **command;

/*
initShell()

Ojective:
  Compiles all function files
*/
void initShell()
{
  // Compile 'mycd'
  system("gcc -o mycd.out mycd.c");
  
  // Compile 'myls'
  system("gcc -o myls.out myls.c");
  
  // Compile 'mymkdir'
  system("gcc -o mymkdir.out mymkdir.c");
  
  // Compile 'mymv'
  system("gcc -o mymv.out mymv.c");
 
 // Compile 'myps'
  system("gcc -o myps.out myps.c");
 
 // Compile 'mypwd'
  system("gcc -o mypwd.out mypwd.c");
 
 // Compile 'myrm'
  system("gcc -o myrm.out myrm.c");
 
 // Compile 'mytail'
  system("gcc -o mytail.out mytail.c");

}


/*
parser(char *, const char *)
Input:
  string1: Input string to be parsed
  delimiter: delemiter for tokenization

Output:
  number of arguments

Objective:
  This function tokenizes the string1 (command line input)
  and makes appropriate changed (removing newline) so that
  we can give a proper input to the 'execlp()' function
*/
int parser(char *string1, const char *delimiter)
{
  // Iterators
  char *itr; // iterator over tokens
  int intItr = 0; // iterator + total number of arguments

  itr = strtok(string1, delimiter);

  // Final output 
  command = (char **)malloc((intItr) * sizeof(char *));
  

  while( itr != NULL)
  {
    // Store tokens in array while iterating
    command[intItr] = itr;
    
    itr = strtok(NULL, delimiter);
    if(itr == NULL)
      break;
    

    intItr++;
    command[intItr] = (char *)malloc( (strlen(itr)) * sizeof(char) );
    
  }

  // Replace last newline with '\0'
  command[intItr][ strlen(command[intItr]) - 1] = '\0';

  // Return total number of arguments
  return intItr+1;
}

/*
runCommand(char *, int)

Input:
  cmd: executable command
  argLen: Total number of arguments

Objective:
  the function runs the command using execlp()
  using arglen it decides number of arguments
  At a time, maximum 4 arguments can be handeled
*/
void runCommand(char* cmd, int argLen)
{

  printf("Total argd: %d", argLen);
  

  switch(argLen)
  {
    case 1:
      execlp (cmd, cmd, NULL);
      break;
    case 2:
      execlp (cmd, cmd, command[1], NULL);
      break;
    case 3:
      execlp (cmd, cmd, command[1], command[2], NULL);
      break;
    case 4:
      execlp (cmd, cmd, command[1], command[2], command[3], NULL);
      break;
    case 5:
      execlp (cmd, cmd, command[1], command[2], command[3], command[4], NULL);
      break;
    case 6:
      execlp (cmd, cmd, command[1], command[2], command[3], command[4], command[5], NULL);
      break;
    case 7:
      execlp (cmd, cmd, command[1], command[2], command[3], command[4], command[5], command[6], NULL);
      break;
    case 8:
      execlp (cmd, cmd, command[1], command[2], command[3], command[4], command[5], command[6], command[7], NULL);
      break;
      

    default:
      printf("Commmand could not be processed. Less than 9 argument needed\n");
  }

  

}


/*
runMyshell()

Objective:
  The function runs an infinite loop (breakable by 'myexit' as input)
  It runs the shell by taking inputs form user and executing command accordingly
  using child process
*/
void runMyshell()  
{

  pid_t pid; // Child process id
  int status; // status of child process
  ssize_t cmd_len; // number of ytes
  size_t buf_len; // length of command line input
  char *buf, *cmd; // variable strings

  
  int argLen;

  char cwd[256]; // For storing the path name
  // Use system call 'getcwd()'
  getcwd(cwd, sizeof( cwd ));
  
  char cmdPath[256];


  int ret; // variable to  store return of 'chdir()'

  

  while(1)  
  {  
    // Prompt
    printf("\nmyshell>> ");  
    
    // Taking input
    cmd_len = getline(&buf,&buf_len,stdin);  
     
    // Store input in 'cmd'
    cmd = (char *)malloc(cmd_len * sizeof( char));
       
    cmd = buf;

    // Tokenize the input string and store in global array 'command'
    argLen = parser(cmd," ");
    
  
    
    // myexit ~ exit ('~' means 'similar to')
    if( strcmp(command[0], "myexit") == 0 )  
      return;
   
    
      // mycd ~ cd
    // mycd is implemented here rather in a file, because
    // when the binary outside the program causes it to exit
    // after changing directory 
    if( strcmp(command[0], "mycd") == 0 ) 
      { 
        // Append path name
        sprintf(cmdPath, "%s/mycd.out", cwd);
        
        ret = chdir(command[1]);

        // If change is successful, ret = 0
        if (ret == 0 )
          printf("Directory changed successfully");
        else
          printf("Please provide target directory");
        
      }
 
    
    else if( (pid = fork ()) == 0) { 
      
      
      // myls ~ ls
      if( strcmp(command[0], "myls") == 0 )  
      { 
        sprintf(cmdPath, "%s/myls.out", cwd);
        runCommand(cmdPath, argLen);
        
      }

      // mymkdir ~ mkdir
      else if( strcmp(command[0], "mymkdir") == 0 )  
      {  
        sprintf(cmdPath, "%s/mymkdir.out", cwd);
        runCommand(cmdPath, argLen);  
      }

      // mymv ~ mv
      else if( strcmp(command[0], "mymv") == 0 )  
      { 
        sprintf(cmdPath, "%s/mymvout.out", cwd);
        runCommand(cmdPath, argLen);  
      } 

      // myps ~ ps
      else if( strcmp(command[0], "myps") == 0 )  
      { 
        sprintf(cmdPath, "%s/myps.out", cwd);
        runCommand(cmdPath, argLen);   
      }

      // mypwd ~ pwd
      else if( strcmp(command[0], "mypwd") == 0 )  
      { 
        sprintf(cmdPath, "%s/mypwd.out", cwd);
        runCommand(cmdPath, argLen); 
        exit(0);
      }

      // myrm ~ rm
      else if( strcmp(command[0], "myrm") == 0 )  
      { 
        sprintf(cmdPath, "%s/myrm.out", cwd);
        runCommand(cmdPath, argLen);   
      }

      // mytail ~ tail
      else if( strcmp(command[0], "mytail")== 0 )  
      { 
        sprintf(cmdPath, "%s/mytail.out", cwd);
        runCommand(cmdPath, argLen);
      }

      // No matching commmand
      else
        printf("%s: No mathcing command found", command[0]);
      
    } 
    else 
      waitpid(pid, &status, NULL);  
  }
}


int main()
{
  initShell();

  runMyshell();

  return 0;
}
