/*
Author: Vishnu Dutt Sharma
Roll No.: 12EC35018
 
Input: 
	The module takes name of files to be read as input from terminal
Objective & Algorithm:
	It prints the total number of vowels and consonents (combined) in all files
	This operation is to be performed using multiple processes
	Each child process opens different files and each process saves number of
	vowels and consonents in a file saved with the name of process in a separate directory
	Then all '.txt' files are opened and numbe of vowels and consonents are
	read and summed up
Caution:
	Directory should not include any '.txt' file
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <dirent.h>

/* reader(const char *, int)
Input: 
	name: string which is the name of the file to be opened
	pid: process id with which the file name is to e stored
Output:
	No explicit output
Objective:
	This function opens text files by name, counts the numer of vowels and 
	consonents. Then function saves number of consonents and vowels in <pid>.txt file
*/
void reader(const char *name, int pid)
{
	FILE *fp; 
	char nameOut[20]; // String to store the name of output file
	int asc; // ASCII value
	long int vowel=0, cons=0; // Number of vowles and consonents

	fp = fopen(name, "rb");

	// If PID is 0, it is not a child process and thus should not be read
	//if(pid == 0)
	//	return;

	while( (asc = getc(fp)) != EOF){
	 	
	 	if ( (asc < 65) || (asc > 122) || ((asc > 90) && (asc < 97)) ) ;
	 	else{
	 		switch(asc){
	 			case 65:  // A
	 			case 69:  // E
	 			case 73:  // I
	 			case 79:  // O
	 			case 85:  // U
	 			case 97:  // a
	 			case 101: // e
	 			case 105: // i
	 			case 111: // o
	 			case 117: // u

	 				vowel++;
	 				break;
	 			
	 			default:
	 				cons++;
	 		}
	 	}

	}
	fclose(fp);

	// Adding '.txt' to the PID for making output file
	sprintf(nameOut, "PIDout/%d.txt", pid);

	fp = fopen(nameOut, "w");

	// Writing the number of vowel and consonents in the file
	fprintf(fp, "%ld %ld",vowel, cons);
	fclose(fp);

}

int main(int argc, char const *argv[])
{
	DIR *FD; // Pointer for opening directory
	struct dirent *fileIn; // For storing file names in directory
	int outfileLen; // For storing length of the name of output file (pointed by filein)

	pid_t pid; // for storing status of fork
	int pid_c; // Process ID
	FILE *fp;  // File pointer for opening files
	
	// Variables for storing number of vowels and consonents
	long int vowel = 0, cons = 0, vowel_sum = 0, cons_sum = 0;
	
	// iterator and Exit status
	int i, status;
	char nameSum[20]; // Variable for storing file name

	struct timeval T1, T2; // T1: Start time, T2: Stop time


	gettimeofday(&T1, NULL);

	// Reading files one by one
	if(argc > 1){

		for(i = 1; i < argc; i++){

			// Forking
			pid = fork();

			if(pid == 0){

				pid_c = getpid();
				
				reader(argv[i], pid_c);

				// Exiting with iterator value (for help in debugging) 
				exit(i);
			}
		}

		for(i = 1; i < argc; i++){

			wait(&status);	// To keep track of child (for debugging)
			
		}


	}

	// Opening current directory
	if (NULL == (FD = opendir("PIDout")))
			printf("Error");

	// Opening files in directory iteratively
	while( (fileIn = readdir(FD)) != NULL){
	
		sprintf(nameSum, "%s", fileIn->d_name); //name of the file to be opened	
		outfileLen = strlen(fileIn->d_name); // length of the name of the file (nameSum)


		// Checking if extension is '.txt'
		if( (nameSum[outfileLen - 1 ] == 't') && (nameSum[outfileLen - 2 ] == 'x') && (nameSum[strlen(fileIn->d_name) - 3 ] == 't') && (nameSum[strlen(fileIn->d_name) - 4 ] == '.') ){
			
			
			sprintf(nameSum, "PIDout/%s", fileIn->d_name);

			// Opening and reading number of vowels and consonents from each file
			fp = fopen(nameSum, "r");

			fscanf(fp,"%ld %ld",&vowel, &cons);
			vowel_sum += vowel;
			cons_sum += cons;
			
			// Closing current file before opening new
			fclose(fp);

		}
	}
	gettimeofday(&T2, NULL);

	// Printing output
	printf("Total number of vowels: %ld\n", vowel_sum);
	printf("Total number of consonents: %ld\n\n", cons_sum);
	// Time taken in execution
	printf("Time = %lu microseconds\n\n", T2.tv_usec - T1.tv_usec);


	return 0;
}