/***
Author: Vishnu Dutt Sharma
		12EC35018

Input:
	User input, path of files to be removed
	additional options: 
		'-d' means directory to be removed (only if empty)
		'-r' means non-empty directories can also be removed
		no option: files and empty directories to be removed
Objective: 
	To remove directories, files as specified by user
*/
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

// Variables for naifgation over directories
DIR *dp; // directory pointer
struct dirent *filename; // iterator over files
struct stat marker; // for getting info about files


/*
removeNED(char *)
Input:
	path: path of the file to e removed
Objective:
	The function recursively remove the files in the folder
	and then folder is removed
	Folder is specified by path
*/
void removeNED( char *path)
{
	// Temprorary variables for navigating over directories
	DIR *tempDP;
	struct dirent *tempName;
	struct stat tempMark;

	// Various strings for storing names
	char *fullname;
	char tempFl[512];
	char backup[512];

	
	// Open directory
	tempDP = opendir(path);
	sprintf(backup,"%s",path);
		
	// Iterate over directory
	while( (tempName = readdir(tempDP)) != NULL)
	{
		fullname = (tempName -> d_name);
		// Leave if filename = '.' or '..'
		if( (strcmp(fullname, ".") == 0) || (strcmp(fullname,"..") == 0) )
				continue;
		
		
		// Append filname in pat
		sprintf(tempFl, "%s", path);
		
		strcat(tempFl, "/");
		strcat(tempFl,fullname);
		
		// get info about file
		stat(tempFl, &tempMark);
		
		// If current file is a directory ( determined by S_ISDIR)
		// Call the same function to remove files
		if(S_ISDIR(tempMark.st_mode))
		{
			removeNED(tempFl);
		}
		// Remove files
		else
			remove(tempFl);
		
	}

	// Remove parent folder
	remove(backup);
	
	// Store original path 
	path = backup;

}


int main(int argc, char *argv[])
{

	int status,i,j; //status of removal, iterators
	char *name;		// name of the file
	int delDir;		// flag for deleting directory
	int startItr;	// stting point for iterator
	int recurDel;	// flag for recusrsive deletion 
	char pathname[] = "./"; // path
	int fileFound;  // Status of search for file
	
	if(argc > 1)
	{
		
		
		if((strcmp(argv[1], "-d") == 0))
		{
			delDir = 1; //delete directory 
			startItr = 2; // iteration to start from argv[2]
			recurDel = 0; // recursive delete not allowed

		}
		else if ((strcmp(argv[1], "-r") == 0))
		{
			delDir = 1; //delete directory
			startItr = 2;// iteration to start from argv[2]
			recurDel = 1;// recursive delete allowed
		}

		else
		{
			delDir = 0; //do not delete directory
			startItr = 1; // iteration to start from argv[1]
			recurDel = 0; // recursive delete not allowed	
		}


		//start iteration over files
		for(i = startItr; i < argc; i++)
		{	

			//open directory
			dp = opendir(".");

			fileFound = 0; // 0 means file found
			while( (filename = readdir(dp)) != NULL)
			{


				name = (filename -> d_name);

				// Leave if filename = '.' or '..'
				if( (strcmp(name, ".") == 0) || (strcmp(name,"..") == 0) )
					continue;

				stat(name, &marker);

						
				// Check if name macthes with file to be deleted
				if( strcmp(name, argv[i]) == 0)
				{
					if( S_ISDIR(marker.st_mode)) // check if the file is a directory
					{
						if( delDir) // if directory deletion is allowed
						{
							status = remove( name);
							if (status == 0)
								fileFound = 0;

							else if( (status != 0) && recurDel) // check status (!= 0 means non-emplty file)
							{	

								// if recursive deletion is allowed
								strcat(pathname,name); 
								
								removeNED(pathname);
								fileFound = 0;
							}

							else if( (status != 0) && (recurDel == 0) )
							{
								printf("Input is not an empty directory");
								fileFound = 0;
							}
						}
						else
						{
							printf("Input is a directory. Not allowed");
							fileFound  = 0;
						}
					}
					else{
						status = remove(name); // remove file

						if( status != 0)
							printf("Error: Please check the input");
						else
							fileFound = 0;
					}

				}

			}
			
			if (fileFound != 0)
				printf("Input file/directory not found");
		}

	}
	else
		printf("Enter path for removal \n");

	return 0;
}