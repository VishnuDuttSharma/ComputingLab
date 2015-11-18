/**
* @file hotel.c
*
*	@author Vishnu Dutt Sharma (12EC35018)
*
*	@brief Function to run all the processes (Chef, Waiter, service room)
*
*		Function takes sleep time for threads in Chef and  Waiter as user input
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
*	\fn int main(int argc, char *argv[])
*
*	@param argc Number of arguments provided by user
*	@param argv[] Arguments for deciding sleep time for threads and for opening specific LOG file
*
*	@return '0' for normal exit, '-1' for abnormal exit/error
*
*	@brief Main process, runs all other processes
*   Function forks and runs other process using 'execlp'
*/
int main(int argc, char *argv[])
{
  int i;
  pid_t pid1;

  if( argc != 7){
    printf("Please provide all the inputs ");
    return -1;
  }
  else{
    for(i = 0; i < 2; i++){
      if( ( pid1 = fork() ) == 0){

        switch( i ){
          case 0:
          execlp("./chef.out", "./chef.out", argv[1], argv[2], argv[3], NULL);
          break;

          case 1:
          execlp("./waiter.out", "./waiter.out", argv[4], argv[5], argv[6], NULL);
          break;

        }
      }
    }
    while(1){
      printf("Enter 1 to exit at the end  \n\n");
      scanf( "%d", &i);
      if(i == 1)
        break;
    }

  }
  return 0;
}
