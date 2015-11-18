/***
Author: Vishnu Dutt Sharma
		12EC35018

Objective:
	To change working directory
Input:
	New directory path
*/

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int ret; // variable to  store return of 'chdir()'

	char cwd[256]; // For storing the path name
  
  // Use system call 'getcwd()'
  getcwd(cwd, sizeof( cwd ));

  printf("%s\n",cwd);

	if(argc > 1){
		printf("input: %s\n", argv[1]);
		ret = chdir(argv[1]);

		// If change is successful, ret = 0
		if (ret == 0 )
			printf("Directory changed successfully");
	}
	else
		printf("Please provide target directory");

	  getcwd(cwd, sizeof( cwd ));

  printf("%s\n",cwd);


	return 0;
}