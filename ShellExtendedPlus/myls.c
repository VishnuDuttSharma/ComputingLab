/**
@file myls.c
@author Vishnu Dutt Sharma (12EC35018)
@brief Prints the list of files with details 
	To print the list of all the files inside the current directory
	along with following details:
	1. read, write and execution permission for User, Group & Other
	2. Owner ID
	3. Group ID
	4. FIle Size
	5. Time of last modifiction (MONth Day Time(Hrs:Min))
	6. Name of the file
Implementation: 
	Iterate over all files in direory eith the help of 'dirent.h'
	Get the neded data with the help of 'stat.h' ''
*/

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>


/**
\fn void detailer(int num)

@param num: number less than 8 (and  > 0) provided by main, used for
	determination of permissions

@return Void: A list is printed

@brief Prints details of files in a directory (similar to 'ls -a')
	Prints file permissions for User, Group and Other. Contains 10 letters
	1st (starting from right) letter is blank;
	2nd data is the numer of links
	3rd, 6th, 9th: Describes read permission for User, Group and Others respectively
	4th, 7th, 10th: Describes write permission for User, Group and Others respectively
	5th, 8th, 11th:  Describes permission for User, Group and Others respectively 
	('-' means permission is not given)


*/
void detailer(int num)
{
	// Following cases are based on Standard table for permission
	
	if ( num & 4)
		printf("r");
	else
		printf("-");
	if ( num & 2)
		printf("w");
	else
		printf("-");
	if ( num & 1)
		printf("x");
	else
		printf("-");

}

/**
@brief Lists all the files with details
 	Details are obtained by file attributes
*/
void main()
{
	DIR *dir;					// Varible for directory handling
	struct dirent *fileName;	// Structure used with DIR* for iterating over ll files
	struct stat marker;			// Structure to determine if the object is directory or not

	struct tm *timeInfo;		// Helps in formatting time	
	char *fullname;				// For storing filenmes during iterations
	char timeString[20];		// String to store/hold time in propoer format 

	struct group *grp;			// Helps in getting group id
	struct passwd *pwd;			// Helps in getting user id

	// Open current directory
	if (( dir = opendir(".")) == NULL)  // Error if equal to NULL
		printf("Error occured in opening current directory\n");
	
	else{

		// Start the itertion proess
		while( (fileName = readdir(dir)) != NULL){

			fullname = (fileName -> d_name);

			// '.' and '..' are not needed.
			if ( ( strcmp(fullname, ".") != 0) && ( strcmp(fullname , "..") != 0) )
			{
				// Getting stat on the basis of name
				stat(fullname, &marker);


				printf("-");  // First letter

				// -(ur)(uw)(ue)(gr)(gw)(ge)(or)(ow)(oe)
				// For each permission, get corresponding 3 bits
				// feed the 3 bit as a number to detailer() to print permission
				detailer(((marker.st_mode) & S_IRWXU) >> 6); //Print User permissions
				detailer(((marker.st_mode) & S_IRWXG) >> 3); //Print Group permissions
				detailer(((marker.st_mode) & S_IRWXO) >> 0); //Print Otherpermissions
				

				// st.nlink gives numer of links
				printf(" %d ",(int)marker.st_nlink);

				// Getting parent ID
				pwd = getpwuid(marker.st_uid);
				printf("%s ",pwd->pw_name);

				// Getting curent usr ID
				grp = getgrgid(marker.st_gid);
				printf("%s ", grp->gr_name);

				
				// Getting File size
				printf("%8d ",(int)marker.st_size);

				// Getting localtime and formatting it
				timeInfo = localtime( &(marker.st_mtime) );
				strftime(timeString, 20, "%b  %d %H:%M", timeInfo);
				printf("%s ",timeString);

				// At last, erite the name of the file
				printf("%s \n", fileName->d_name);
			}

		}
		// Close the directory
		closedir (dir);
	}

}