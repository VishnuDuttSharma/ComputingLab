/* 
Author: Vishnu Dutt Sharma
Roll No.: 12EC35018
Objective: 
	This module takes two inputs from user: 
		(1) Number which has two prime factors
		(2) Number of processes
Objective:
	It prints the prime factor of the input (assuming the input has only two prime factors)
	It also prints the time taken in calculation
	Whole processe is done by searching in the range (1,sqrt(number)) by dividing this operation
	in 'm' processes
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <sys/mman.h>


// Waiting time for the child process
int wait_time = 1;

// Global variable (shared memeory) for storing prime factor
long int *pFactor;


/* primeFactorTwo(long int, long int, long int)
Input: 
	num: Number whose prime factors are to be determined
	start: Integer starting from which the search is to be performed
	stop: Integer at which search is to be stopped 
Output:
	i: Number which is a prime factor in the range (start, stop)
Objective:
	The function finds the smallest prime factor in range (start, stop)
	If no prime factor is found then '1' is returned
	If a prime factor is found, it is printed
*/
long int primeFactorTwo(long int num, long int start, long int stop)
{	
	long int i, result = 1;
	for(i = start; i <= stop; i++){
		
		if( num % i == 0 ){
			result = i;
			break;
		}
	}

	if (result != 1){

	
	printf("\nPrime factors: %ld, %ld\n",result, num / result);
	}

	return result;
}



int main()
{
	int m; // Number of processes
	long int n, len, start, stop, resp, status; 
	pid_t pid_c; // flag for determining if a child is generated on forking
	int i = 0;  // Iterator
	struct timeval T1, T2; // T1: Start time, T2: Stop time

	
	// User Inputs
	printf("\nInput: \nNumber = ");
	scanf("%ld",&n);

	printf("Processes = ");
	scanf("%d",&m);


	// Starting Time
	gettimeofday(&T1, NULL);

	// Length of each section/part
	len = sqrt(n) / m;

	for(i = 0; i < m; i++){

		pid_c = fork();

		if (pid_c == 0){

			// Start from number = 2
			start =  2 + i * len;
			// Stop at number, length = sqrt(number)/ no. of processes
			// For last section Stop point should be sqrt(number)
			if( i != m - 1)
				stop = 2 + (i + 1) * len - 1;
			else
				stop = sqrt(n);

			// Apply function for finding prime factor
			resp = primeFactorTwo(n, start, stop);
		
			// Waiting 	
			sleep(wait_time);
			
			// Exiting with process number for tracking during debugging
			exit(i);
		}

	}

	// Waiting for all child processes to return
	for(i = 0; i < m; i++)
		wait(&status); // Status can help in debugging
	
	// Stopping time
	gettimeofday(&T2, NULL);

	// Time in execution
	printf("Time = %lu microseconds\n", T2.tv_usec - T1.tv_usec);


	return 0;
}