/***
Author: Vishnu Dutt Sharma
		12EC35018

Input:
	Additional option "-a" can be provided as input for listing
	all processes

Objective:
	Lists all proceses along with following:
	1. PID
	2. Device being used (TTY)
	3. Status (sleepeing etc, abreviation are used)
	4. Time of CPU usage
	5. Command/Name for the process

Implementation:
	Program foes iteratively in '/proc' 
	directory and subdirectory and gathers data from various
	files
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <linux/kdev_t.h>

/*
gerDevMap(int, int)

Input:
	major: a major number, found using linux/kdev_t.h
	minor: a minor number, found using linux/kdev_t.h
Objective:
	This function provides a mapping for finding out the device
	Data provided by '/proc/<PID>/stat' is converted into Major
	and Minor numbers using  'linux/kdev_t.h' and those numbers
	are mapped here

	The mapping is taken from '/proc/devices'
*/
void getDevMap(int major, int minor)
{
	switch(major)
	{
		case 1:
			printf("mem%d\t",minor);
			break;
		case 4:
			printf("tty%d\t",minor);
			break;
		case 5:
			printf("/dev/tty%d\t",minor);
			break;
		case 6:
			printf("lp%d\t",minor);
			break;
		case 7:
			printf("vcs%d\t",minor);
			break;
		case 10:
			printf("misc%d\t",minor);
			break;
		case 13:
			printf("input%d\t",minor);
			break;
		case 21:
			printf("sg%d\t",minor);
			break;
		case 29:
			printf("fb%d\t",minor);
			break;
		case 81:
			printf("video4linux%d\t",minor);
			break;
		case 99:
			printf("ppdev%d\t",minor);
			break;
		case 108:
			printf("ppp%d\t",minor);
			break;
		case 116:
			printf("alsa%d\t",minor);
			break;
		case 128:
			printf("ptm%d\t",minor);
			break;
		case 136:
			printf("pts/%d\t",minor);
			break;
		case 180:
			printf("usb%d\t",minor);
			break;
		case 189:
			printf("usb_device%d\t",minor);
			break;
		case 216:
			printf("rfcomm%d\t",minor);
			break;
		case 226:
			printf("drm%d\t",minor);
			break;
		case 251:
			printf("hidraw%d\t",minor);
			break;
		case 252:
			printf("bsg%d\t",minor);
			break;
		case 253:
			printf("watchdog%d\t",minor);
			break;
		case 254:
			printf("rtc%d\t",minor);
			break;
		default:
			printf("?\t");
	}
  
}


/*
openProc(int)

Input:
	curr: if 1: only processes owned by current owner are listed
		  if 0: all processes are listed
Objective:
	This programs goes through various directories and files in '/proc'
	and gets information about all processes
	
	First it goes into those directories which correspond to a process
	
	Then it opens '/proc/<pid>/status' and gets 'NAME (alternative for COMMAND)',
	'STATUS', and 'USER_ID'

	Now if input specifies the need of processes owned by current user
	only. We check the USER_ID (!=2 for current user) and if needed
	either jump to next process directory or continue

	Now '/proc/<pid>/stat' is opened and 'TTY/DEVICE' and 'TIME' 
	are obtained

	At last, we go to '/proc/<pid>/cmdline' and get 'COMMAND' (the command 
	input for running the process). If is is blank then 'NAME' is printed 
	asobtained in provious step

*/
void openProc(int curr)
{
	// Variables for directory navigation
	DIR *DP;  // directory pointer
	struct dirent *filename; // iterator over files in directory
	struct stat marker; // for getting various info about file

	// Variables for navigation in a sub-directory
	DIR *subDP;  
	struct dirent *subilename;

	// For storing strings and filenames temprarily
	char *buffer;
	char fullname[20];

	// Used in getline, helps in reading files line by line
	char *readLine = NULL; // string where line is read/stored
	size_t readLen = 0;  //  length of line

	// Variables used for storing inf to be printed
	int pid; // PID
	char pName[100];  // Name of the porcess 
	char pStatus[10]; // Status of the process
	int read = 0;     // To help in determing if 'cmdline' file is empty 
	long unsigned int pTime; // Time
	int parentId; // parent id/ user is corresponding to process

	FILE *fp;  // file pointer for reading the files

	int major,minor; // variables to store MAjor and Minor number while determining TTY

	int temp;  // Temporary integer, used as iterator
	int tempChar; // Temporary char, used as helper in reading files
	long unsigned int tempTime; // Temporary variale for storing time


	// Open the '/proc' directory
	DP = opendir("/proc");

	// Start printing all data
	printf("   PID TTY\t STAT   TIME\t COMMAND\n");
	

	// Start iterating
	while( (filename = readdir(DP)) != NULL)
	{
		buffer = filename->d_name;
		
		// If name is an integer, it is a process folder
		if ( (pid = atoi(buffer)) != 0)
		{
			stat(buffer, &marker);


			// Open '/proc/<PID>/status' file
			sprintf(fullname, "/proc/%s/status",buffer);
			if( (fp = fopen(fullname, "rb")) == NULL)
				printf("Error in opening file");
				
			else
			{
				// Read Process name 'NAME'
				getline(&readLine, &readLen, fp);
				
				// Move pointer a little ahead to read only data, not descriptor
				sprintf(pName, "%s",readLine + 6);
				
				// Avoid newline by placing '\0' before it
				pName[ strlen(pName) - 2] = '\0';

				// Read process status 'STATUS', similar to 'NAME'
				getline(&readLine, &readLen, fp);
				sprintf(pStatus, "%s", readLine + 7);
				pStatus[2] = '\0';
			
				// Get user/parent id, stored in readline
				for(temp =0 ; temp < 4;temp++)
					getline(&readLine, &readLen, fp);
				readLine[strlen(readLine) - 1] = '\0';

				fclose(fp);
			}

			// If needed (as specified by user input), continue on the 
			// basis of parent process/ user id
			// USer id == 2 means 'root'
			if( (strcmp(readLine+6, "2") == 0) && (curr == 1))
				continue;			

			// print PID
			printf("%6d  ", pid);

			// Open '/proc/<PID>/status' file
			sprintf(fullname, "/proc/%s/stat",buffer);
			if( (fp = fopen(fullname, "rb")) == NULL)
				printf("Error in opening file");
				
			else
			{
				// Read device id from its location
				temp = 0;
				while(1){
					tempChar = getc(fp);
					if(tempChar == ' ')
						temp++;
					if(temp == 6)
						break;
				}
				fscanf(fp,"%d",&temp);
				
				// Convert into Major and Minor numbers
				major = MAJOR(temp);
				minor = MINOR(temp);

				// Print device id
				getDevMap(major, minor);

				// Get time in jiffeies
				temp = 0;
				while(1){
					tempChar = getc(fp);
					if(tempChar == ' ')
						temp++;
					if(temp == 7)
						break;
				}

				// Calculate total time taken in User and Kernel mode
				fscanf(fp,"%lu%lu", &pTime, &tempTime); //pTime: User mode, tempTime: kernel Time
				pTime += tempTime;  

				fclose(fp);
			}
			
			// Print STATUS
			printf(" %s ", pStatus);
			
			// Print TIME by converting from jiffies to hrs:min
			pTime = pTime / sysconf (_SC_CLK_TCK);
			
			// printing TIME with proper formatting
			printf("\t%3lu:%.2lu\t", (pTime / 60), (pTime % 60));
			

			sprintf(fullname, "/proc/%s/cmdline",buffer);
			if( (fp = fopen(fullname, "rb")) == NULL)
				printf("Error in opening file");
				
			else
			{
				// If file is empty, print NAME
				read = getline(&readLine, &readLen, fp);
				if(read == -1)
					printf("[%s]\n", pName);
				else
				{
				// Print COMMAND if available 
				//to keep the screen cleaner, print maximum 60 characters 
					if(strlen(readLine) > 60)
						readLine[60] = '\0';

					printf("%s\n", readLine);
				}

				fclose(fp);
			}

		}
		
	}
}


int main(int argc, char *argv[])
{
	if(argc > 1){
		if(strcmp(argv[1],"-a") == 0)
			openProc(0);
	}

	else
		openProc(1);
		
	return 0;
}
