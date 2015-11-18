/**
@file mytail.c
@author Vishnu Dutt Sharma (12EC35018)

@param -N: Number of lines to be read
@param file: file to be read


@brief To print the last 'N' lines of input 'file'
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
@brief The function will create an array of size 
	specified by user input '-N' for an input file. Each line is
	fed as input to the array and at the last whole array is printed.

	If no '-N' is given whole file is printed. Same happens if
	number of lines to be read is greater than the number of lines in 
	the input file

@return Void: 'N' line from the file will be printed on the terminal
*/

void main(int argc, char *argv[])
{

	FILE *fp; // file pointer

	char *line = NULL; // string for storing a line from a file 
	size_t len = 0;	// number of bytes
	ssize_t read;	// read status for the file, -1 will End of File is reached

	int itr = 0,i; // Iterators

	char **lineArray;	// Array for storing lines

	int lines = 0;	// Number of lines

	// If no number is given print all lines
	if(argc == 2)
	{
		fp = fopen( argv[1], "rb");
		while ( (read = getline(&line, &len, fp)) != -1 )
			printf("%s",line);
	}
	else if( argc > 2)
	{
		// Reading number of lines to be read an file name from command line input
		if(argv[1][0] == '-'){	
			lines = -1 * atoi( argv[1] );
			fp = fopen( argv[2], "rb");
		}
		
		else if(argv[2][0] == '-'){
			lines = -1 * atoi( argv[2] );
			fp = fopen( argv[1], "rb");
		}

		else{
			printf("Error: Proper Input Needed");
			
		}

		// Assign size to array for storing line (N lines)		
		lineArray = (char **)malloc(lines * sizeof(char *));


		// Open file
		if ( fp == NULL )
		{
			printf("No such file exists");
			exit(-1);
		}
			
		// Read each line and keep stroing in lineArray in 
		// in a circular/rotating fashion 
		while ( (read = getline(&line, &len, fp)) != -1 )
		{
			lineArray[itr % lines] = (char *)malloc(strlen(line) * sizeof(char));

			sprintf(lineArray[itr % lines], "%s", line);
			
			itr++;
		}

		// If number of lines is lesser than the input, print all
		if(itr < lines)
			lines = itr;

		// Printing N lines
		for(i = 0; i < lines; i++)
			printf("%s", lineArray[(itr +i) % lines]);

		// Deallocate the memory
		free(line);
	}
	else
		printf("Provide all arguments");


}