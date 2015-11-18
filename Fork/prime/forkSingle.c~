/*
Author: Vishnu Dutt Sharma
Roll No.: 12EC35018
Objective: 
Input: 
	n: Number which has only two prime factors
Objective:
	It prints the prime factors of the input (this operation is performed in a single process)
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

/* primeFactorTwo(long int)
Input: 
	num: a long integer which has two prime factors
Output: 
	i: a long integer which is one of the prime factor of the input
*/
long int primeFactortwo(long int num)
{
	long int i;
	
	for(i = 2; i <= sqrt(num); i++){
		
		if( num % i == 0 )
			break;
	}

	return i;
}

int main(void)
{
	long int n, out; // number(input) and prime factor(output)
	struct timeval stop, start; // For time calculation

	// User input
	printf("Number: ");
	scanf("%ld",&n);

	// Starting Time
	gettimeofday(&start, NULL);
	out = primeFactortwo(n);
	// Stopping time
	gettimeofday(&stop, NULL);

	
	
	printf("\nPrime factors = %ld, %ld", out, n / out);
	printf("\nTime = %lu microseconds\n", stop.tv_usec - start.tv_usec) ;

	return 0;
}
