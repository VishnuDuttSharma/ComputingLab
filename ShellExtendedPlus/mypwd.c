/**
@file mypwd.c
@author Vishnu Dutt Sharma (12EC35018)

@brief Prints the path of current working directory

	System call 'getcwd()' is used
*/
#include <stdio.h>
#include <unistd.h>
/**
@brief system call 'getcwd' is used which retuns the current working directory
*/
void main()
{
	char cwd[256]; // For storing the path name
	int fd;

	// Use system call 'getcwd()'
	if ( getcwd(cwd, sizeof( cwd )) != NULL)
		printf("%s\n", cwd);
	else
		printf("Error Occured");
    
	//return 0;
}