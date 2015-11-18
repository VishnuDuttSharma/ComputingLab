/***
Author: Vishnu Dutt Sharma
		12EC35018

Objective:
	To print the path of current working directory

	System call 'getcwd()' is used
*/
#include <stdio.h>
#include <unistd.h>


int main()
{
	char cwd[256]; // For storing the path name

	// Use system call 'getcwd()'
	if ( getcwd(cwd, sizeof( cwd )) != NULL)
		printf("%s\n", cwd);
	else
		printf("Error Occured");

	return 0;
}