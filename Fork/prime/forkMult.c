/* 
Author: Vishnu Dutt Sharma
Roll No.: 12EC35018
Input: 
	This module takes two inputs from user: 
		(1) Number which has two prime factors
		(2) Number of processes
Objective:
	It prints the prime factor of the input (assuming the input has only two prime factors)
	It also prints the time taken in calculation
	Whole processe is done by searching in the range (1,sqrt(number)) by dividing this operation
	in 'm' processes
Algorithm:
	It divided the range [2,sqrt(n)] into 'm' (number of process) equal(almost) parts and assigns each 
	par to a child. Each child tries to find a prime factor for the input number 'n' in the part/range 
	provided to it. If any child finds the prime factor, it returns '1' as exit status, otherwise it returns '0'
	In the main process, parent reads exit status of all child in a loop and breaks out of the loop as soon as
	the exit status '1' is found
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>


/* primeFactorTwo(long int, long int, long int)
Input: 
	num: Number whose prime factors are to be determined
	start: Integer starting from which the search is to be performed
	stop: Integer at which search is to be stopped 
Output:
	i: Number which is a prime factor in the range (start, stop)
Objective:
	The function finds the smallest prime factor in range (start, stop)
	If no prime factor is found then '0' is returned
	If a prime factor is found,then '1' is printed
*/
int primeFactorTwo(long int num, long int start, long int stop)
{	
	long int i, result = 1;
		
	for(i = start; i <= stop; i++){
		// If number is completely divisible by iterator, iterator is the prime factor 
		if( num % i == 0 ){
			result = i;

			printf("\nPrime factors: %ld, %ld\n",result, num / result);
			return 1; // Prime factor has been obtained
		}
	}

	return 0; // No prime factor obtained
}



int main()
{
	int m; // Number of processes
	long int n, len, start, stop, resp, status; 
	pid_t pid_c; // flag for determining if a child is generated on forking
	int i = 0;  // Iterator
	struct timeval T1, T2; // T1: Start time, T2: Stop time

	
	// User Inputs
	printf("\nPlease Enter The Input: \nNumber = ");
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
		
			// Exiting with status of search, '1' means prime factor is found
			exit(resp);
		}
		


	}

	// Waiting for child processes to return and checking status for breaking
	
	for(i = 0; i < m; i++){
		wait(&status); // Status helps in breaking when number is obtained

		if( WEXITSTATUS(status) == 1) // '1' indicates that prime factor has been found
			break;
	}
	
	// Stopping time
	gettimeofday(&T2, NULL);

	// Time taken in execution
	printf("Time = %lu microseconds\n\n", T2.tv_usec - T1.tv_usec);


	return 0;
}
