/*
Author: Vishnu Dutt Sharma
Roll No.: 12EC35018

Input: 
	n: Number which has only two prime factors
Objective:
	It prints the prime factors of the input (this operation is performed in a single process)
Algorithm: 
	It searches for a prime factor of input 'n' in a range [2, sqrt(n)] iteratively, and breaks out and print 
	as soon as a number that can divide 'n' is found
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
Objective:
	It returns the number (encountered first) in the range [2, sqrt(num)] ('num' is the input) that can divide 'num'
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
	printf("Please Enter The Input:\nNumber: ");
	scanf("%ld",&n);

	// Starting Time
	gettimeofday(&start, NULL);
	out = primeFactortwo(n);
	// Stopping time
	gettimeofday(&stop, NULL);

	
	// Printing output
	printf("\nPrime factors = %ld, %ld", out, n / out);
	printf("\nTime = %lu microseconds\n\n", stop.tv_usec - start.tv_usec) ;

	return 0;
}
