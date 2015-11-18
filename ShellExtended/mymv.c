/**
@file mymv.c
@author: Vishnu Dutt Sharma
		12EC35018

@param argv:	Needs Itwo inputs from user, first one is the source location,
	Other is the destination

@brief
	Moves one file to other location OR one directory to 
	another location.
	
	For implementation system call 'remove' isused

Errors may occur
	1. If any of the input is incomplete or erroneous
	2. If both inputs are not files/directory
*/
#include <stdio.h>
#include <string.h>
#include <dirent.h>

void main(int argc, char *argv[])
{
	

	if (argc > 2)
	{

		// system call 'rename' is used
		if( rename( argv[1], argv[2]) )
			printf( "Error in location. Provide correct input" );

	}
	else
		printf("Please enter all arguments");

}