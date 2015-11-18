/**
@file myshell.c
@author Vishnu Dutt Sharma (12EC35018)

@brief Main controller file, runs the shell

Description Of Customized Function:
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
#include <fcntl.h>
#include <errno.h>
#include <termios.h>


/** 
@brief This 2-d array stored the input by user as tokens separated by ' '(space)
*/
char **command;


/**
\fn void initShell()

@brief 
  Initializes  the shell by compiling all function files

@return Void
*/
void initShell()
{
  /* Compile 'mycd' */
  system("gcc -o mycd.out mycd.c");
  
  /* Compile 'myls' */
  system("gcc -o myls.out myls.c");
  
  /* Compile 'mymkdir' */
  system("gcc -o mymkdir.out mymkdir.c");
  
  /* Compile 'mymv' */
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


/**
\fn int myParser(char *string1, const char *delimiter)

@param string1: Input string to be parsed
@param delimiter: delemiter for tokenization

@param argLen: number of arguments separated by 'delimiter'

@return intItr: number of arguments

@brief Finds the number of token in a string
  This function tokenizes the string1 (command line input)
  and makes appropriate changed (removing newline) so that
  we can give a proper input to the 'execlp()' function

*/
int myParser(char *string1, const char *delimiter)
{
  //Iterators 
  char *itr; //iterator over tokens 
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
    
    // increase the number of arguments and allocate memory to it
    intItr++;
    command[intItr] = (char *)malloc( (strlen(itr)) * sizeof(char) );
    
  }

  // Replace last newline with '\0'
  if( (command[intItr][ strlen(command[intItr]) - 1] == '\n') || (command[intItr][ strlen(command[intItr]) - 1] == ' '))
    command[intItr][ strlen(command[intItr]) - 1] = '\0';

  // Return total number of arguments
  return intItr+1;
}





/**
\fn void runCommand(char* cmd, int argLen)


@param cmd: executable command
@param argLen: Total number of arguments
@return Void

@brief Runs the system commands/executables
  The function runs the command using execlp()
  using arglen it decides number of arguments
  At a time, maximum 4 arguments can be handeled
*/
void runCommand(char* cmd, int argLen)
{

  
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


/**
\fn void runMyshell()

Objective:
  The function runs an infinite loop (breakable by 'myexit' as input)
  It runs the shell by taking inputs form user and executing command accordingly
  using child process

@return Void

*/
void runMyshell()  
{

  pid_t pid; // Child process id
  int status; // status of child process
  ssize_t cmd_len; // number of ytes
  size_t buf_len; // length of command line input
  char *buf, *cmd; // variable strings

  
  int argLen; // Totsl number of argumets

  char cwd[256]; // For storing the path name
  // Use system call 'getcwd()'
  getcwd(cwd, sizeof( cwd ));
  
  char cmdPath[256];


  int ret; // variable to  store return of 'chdir()'

  //* File descriptor for input and Output
  int i; // iterator
  int op_ind, ip_ind; // indices for helping in reading the filenames
  int ip, op; // input and output file descriptor
  int ipFlag, opFlag; // Flags for input and output
  int stdoBack, stdiBack; // Backup file descriptor for stdout and stdin

  //* Background operator
  int backgrnd; // Flag for background operator
  int countProc = 0; // Cont of background processes
  pid_t lastPid; // PID of last completed process (needed when using background operator)
  char lastCommand[256];
  
  //* Pipes
  char *startP, *endP; // Pointers for the start and end of string (for pipe implementation)
  char *data, dataCopy[256]; // local string variables 
  int len; // length of tokenized string
  int pipefd[2]; // File descriptors for piping
  int buffd;    // Temporary file descriptor


  // Making copies for backup
  stdoBack = dup(1); 
  stdiBack = dup(0);
  while(1)  
  { 

    
    // Prompt
    printf("\nmyshell>> ");  

    
    // Make the input and output flags zero
    ipFlag = 0;
    opFlag = 0;

    // Making background operator flag zero
    backgrnd = 0;

    // Taking input
    cmd_len = getline(&buf,&buf_len,stdin);  

    
    // Making a copy of the data 
    sprintf(dataCopy, "%s", buf);
   
    // Starting point for the iteration
    startP = dataCopy;
    while(1)
    {
      // Create a pipe
      {
        if ( pipe(pipefd) == -1)
          perror("pipe");

      }

      // Searching and pointing at the pipe ('|')
      endP = strchr( startP, '|');

      // If no more pipe operator is there, Look for end of string
      if(endP == NULL){
        len = (strchr( startP, '\0') - startP);
      }
      else{
        len = endP - startP ;
      }

      //  store the token in 'data'
      data = (char *)malloc(len * sizeof( char));
      
      strncpy(data, startP, len);

      // Store input in 'cmd'
      cmd = (char *)malloc(cmd_len * sizeof( char));
         
      cmd = data;


      // Tokenize the input string and store in global array 'command'
      argLen = myParser(cmd," ");
      
      // Finding out if a descriptor is needed
      for (i = 1; i < argLen; i++){
       
        // Case for  '<'
        if( strcmp( command[i], ">") == 0){ 
          opFlag = 1;
          op_ind = i;
          argLen -= 2;
        }

        // Case for '>'
        if( strcmp( command[i], "<") == 0){
          ipFlag = 1;
          ip_ind = i;
          argLen -= 2;
          }

        // Case for '&'
        if( strcmp( command[i], "&") == 0)
          backgrnd = 1;
        
      }
      
      // If '<' was detected
      if ( opFlag )
        if( (op = open( command[ op_ind + 1 ], O_WRONLY | O_CREAT | O_APPEND , S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) ) > -1) {
          fcntl(op, F_SETFD, FD_CLOEXEC);
          dup2(op, 1);
        }
        else
          printf("Error in opening output file, %d", errno);

      // If '>' was detected
      if ( ipFlag ){
        if( (ip = open( command[ ip_ind + 1], O_RDWR)) > -1)
          dup2(ip, 0);
        else
          printf("Error in opening input file");
      }
      

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
          
          // Change sirectory using system call 'chdir'
          ret = chdir(command[1]);

          // If change is successful, ret = 0
          if (ret == 0 )
            printf("Directory changed successfully");
          else
            printf("Please provide target directory");
          
        }
   
      
      else if( (pid = fork ()) == 0) { 
        
        // piping
        dup2(buffd, 0);

        if (endP != NULL){
          dup2(pipefd[1], 1);
        }
        
        close(pipefd[0]);

        if ( backgrnd )
          setpgid( getpid(), 0);
         

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
          runCommand(command[0], argLen);
          //printf("%s: No mathcing command found", command[0]);    
        
        
      } 
      else
      // Handling background and non-background processes
      {
        if (backgrnd != 1)
          waitpid(pid, &status, NULL);
        else{
          lastPid = waitpid(-1, &status, WNOHANG);
          
        }
        
        
        close(pipefd[1]);
        buffd = pipefd[0];

        // Restore STDIN and STDOUT
        // For '>'
        if (opFlag)
        {
          dup2(stdoBack, 1);
        
          close(op);
        }

        // For '<'
        if (ipFlag){
          dup2(stdiBack, 0);

          close(ip);
        }

        // Count the number of background processes 
        // and print the output 
        if (backgrnd){
          countProc++;
          printf("[%d] \t %d \n", countProc, pid);
    
        
        }
      }

      // If end of string is detected, restore STDIN and STDOUT
      if(endP == NULL){       
        dup2(stdoBack, 1);
        dup2(stdiBack, 0); 
        break;

      }
      
      // Point at start of other token (for tokenizaion by '|')    
      startP = (endP + 1);
    }
 
  }
}
    

/**
@brief Runs the shell
  Using system processs and child process for calling
  various executable to mimic the shell
*/
void main()
{
  // Compile al files
  initShell();

  // Run the shell
  runMyshell();

}
