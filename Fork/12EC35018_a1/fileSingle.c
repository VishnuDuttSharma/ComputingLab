/*
Author: Vishnu Dutt Sharma
Roll No.: 12EC35018
Objective: 
Input: 
	The module takes name of files to be read as input from terminal
Objective:
	It prints the total number of vowels and consonents (combined) in all files
*/

#include <stdio.h>
#include <stdlib.h>

// Global variabls for storing number vowels and consonents for each iteration
long int vowel, cons;
	
/* reader(const char *)
Input: name of the file to be opened
Output: No explicit output, updates global variables 'vowel' and 'cons'
Objective:
The function opens the file specified by 'name' and counts the number of vowels and
consonents in that file
This function updates global variables 'vowel' and 'cons'
*/
void reader(const char *name)
{
	FILE *fp; // Single file pointer for I/O operations for all files
	int asc; // Variables for storing ASCII value
	vowel = 0; // Number of vowels (for single file)
	cons = 0;  // Number of consonents (for single file)
	
	fp = fopen(name, "rb");

	while( (asc = getc(fp)) != EOF){
	 	
	 	// If not a letter, do nothing
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

	// Close file
	fclose(fp);
}

int main(int argc, char *argv[])
{
	// Variables for storing total number of vowels and consonents
	long int vowel_sum = 0, cons_sum = 0;
	
	int i; // Iterator

	// Reading all files iteratively
	if(argc > 1){

		for(i = 1; i < argc; i++){

			// Reading number of vowels and consonents 
			reader(argv[i]);
			// Summing up number of vowels and consonents
			vowel_sum += vowel;
			cons_sum += cons;
		}
	}

	printf("Total numer of vowels: %ld\n", vowel_sum);
	printf("Total number of consonents: %ld\n", cons_sum);

	return 0;

}