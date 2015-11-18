/**
@file mymkdir.c
@author Vishnu Dutt Sharma (12EC35018)
@brief Creates directories and subdirectories
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

/**
@brief Variable to store names of directories after parsing
*/

char **dirToken; 

/**
\fn int parser(char *, const char *)

 @param string1: Input string to be parsed
 @param delimiter: delemiter for tokenization


 @return number of arguments (number of token)

@brief Tokenization of the string on the basis of delimiter
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
  dirToken = (char **)malloc((intItr) * sizeof(char *));
  

  while( itr != NULL)
  {
    // Store tokens in array while iterating
    dirToken[intItr] = itr;
    
    itr = strtok(NULL, delimiter);
    if(itr == NULL)
      break;
    
  	intItr++;
    dirToken[intItr] = (char *)malloc( (strlen(itr)) * sizeof(char) );
  	  
  }
     
  // Return total number of arguments
  return intItr+1;
}

/**
\fn void dirMaker(char *)

@param name: direcory name to be created

@brief function creates directories and sub-directories
	this function calls system call mkdir() 
	to create the desired directory
	If the input is long e.g.'/home1/user/file', where
	first directory does not exist, then the folders are creted one by one

*/	
void dirMaker(char *name)
{
	int status; 		//status:return value of mkdir()
	char path[] = "." ; // current directory path
	char fullPath[512]; // variable for storing path along with file name
	int numDir; 		// variable to store number of directories
	int j; 				//iterator

	numDir = parser( name, "/");
	sprintf(fullPath,"%s",path);
	
	for(j = 0; j < numDir; j++)
			{
				// Adding directory name to previous path
				strcat(fullPath,"/");
				strcat(fullPath, dirToken[j]);
				
				status = mkdir(fullPath, 0777); // make directory with all permissions to all
				
				if( status == 0)
					printf("Directory '%s' created successfully\n", fullPath);
				else
					printf("directory not created\n");
			}
		


}

/**
@brief The program creates directories and sub-directories

Directoris can be created directly by using 'mkdir' system call.
If sub-directories are to be created, function first opens the 
existing directory and then created subidirectories inside
*/
void main( int argc, char *argv[])
{

	int i; // iterators
	
	if(argc < 2)
		printf("Error: No directory name provided\n");
	else{

		for (i = 1; i < argc; i++){

			dirMaker( argv[i]);
		}
	}

	//return 0;
}