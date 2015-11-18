/**
 * @file chef.c
 *
 *	@author Vishnu Dutt Sharma (12EC35018)
 *
 *	@brief Function to perform task allocation (mimicing OS) as described in Assignment 4a.
 *
 *		Function takes user input to perform task allocatin on 3 threads(tasks being semaphore signalling).
 *		The user input decides the sleep time for the processes running in threads.
 *		If no argument is provided (no argv[1]), then delay is equal to FIXED_SLEEP_TIME and logFixedDelay.txt is the LOG file. Otherwise,
 *		if argv[1] is '1': Fixed delay, delay = argv[2], logFixedDelay.txt is the LOG file
 *		If argv[2] is '2': Random delay, limited by MAX_SLEEP_TIME, logRandDelay.txt is the LOG file
 *		Otherwise, No delay i.e. delay = 0, logNoDelay.txt is the LOG file
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

/**
 *	@brief \def MAX_SLEEP_TIME Maximum time upto which sleep time (int) can be provided to processes when using random sleep time allocation
 *	@brief \def FIXED_SLEEP_TIME This time (int) is the default time when no user input is given
*/
#define MAX_SLEEP_TIME 6
#define FIXED_SLEEP_TIME 1

/**
 *	@brief \var maachSem Semaphore for 'Maach' (as provided in Assignment 4)
 *	@brief \var dalSem Semaphore for 'Dal' (as provided in Assignment 4)
 *	@brief \var bhaatSem Semaphore for 'Bhaat' (as provided in Assignment 4)
*/
sem_t maachSem, dalSem ,bhaatSem;

/**
 *	@brief \var slTime1 Sleep time for first thread
 *	@brief \var slTime2 Sleep time for second thread
 *	@brief \var slTime3 Sleep time for third thread
*/
int slTime1, slTime2, slTime3;


/**
 *	@brief \var start Variable for storing starting time of the 'Chef' process (main process)
 *	@brief \var stop Variable for storing stopping time for each thread for making entry in LOG
*/
struct timeval start, stop;

/**
 *	@brief \var mutex Mutex for avoiding deadlock
*/
pthread_mutex_t mutex;


/**
 *	@brief \var service[3] Array of three threads (pthread)
 *	@brief \var attr[3] Array of attributes for three threads in array 'service[]''
*/
pthread_t service[3];
pthread_attr_t attr[3];

/**
 *	@brief \var count Variable for writing serial number in LOG file while making entry
 *	@brief \var breaker Flag for breaking out of loop running in threads (for breaking, breaker = 1)
*/
static int count = 1;
int breaker;

/**
 *	@brief \var fileName[256] String for storing different filenames for writing in different LOG files for different user inputs
*/
char fileName[256];

/**
\fn void enterLog(int id, char *item1)

 *	@param id ID (provided by us) of the thread
 *	@param item1 String indicating the name of the item (Maach, Dal or Bhaat)
 *
 *	@return Void
 *
 *	@brief Function for making entry in the LOG file
 *
 *		This function makes entry in the LOG file (pointed by 'fileName')
 *		as soon as a semaphore becomes zero. It writes 'Serial Number',
 *		'Thread Number', 'Item name' and 'Time elasped since start of 'Chef' process
*/
void enterLog(int id, char *item1)
{
	// Get current time
	gettimeofday(&stop, NULL);

	FILE *fp; //File pointer for opening file

	// OPen file in append mode
	fp = fopen(fileName, "a");

	// Make entry
	fprintf(fp, "%3d \t\t %d \t\t %s \t %lu\n", count++, id, item1, ( stop.tv_usec - start.tv_usec ));

	// Close the file
	fclose(fp);
}

/**
 *	\fn void *runService1()
 *
 *	@param None
 *
 *	@return Void
 *
 *	@brief Function for running task for thread 1
 *
 *		This function runs in an infinite loop breakable by global
 *		variable breaker (signalled by main process). It makes the
 *		thread sleep for time indicated by slTime1 and then locks the
 *		thread my mutex . After this, it waits for semaphores Maach and Dal
 *		to become zero, makes entry for them using enterLog(1, item_name), and then increaments
 *		value of semaphore for next processes. After that, lock is opened. If
 *		breaker is '1', it break out of loop otherwise same process is repeated again
*/
void *runService1()
{

	while(1){

		// Make the function to sleep
		sleep( slTime1 );

		// Lock the thread
		pthread_mutex_lock( &mutex );


			/*  Maach */
		// Wait for semaphore to become zero
		sem_wait( &maachSem );
		// Make the entry in LOG file
		enterLog(1, "Maach");
		// Increament semaphore value for next process
		sem_post(&maachSem);


			/*  Dal */
		// Wait for semaphore to become zero
		sem_wait( &dalSem );
		// Make the entry in the LOG file
		enterLog(1, "Dal");
		// Increament semaphore value for next process
		sem_post(&dalSem);

		// Unlock the thread
		pthread_mutex_unlock( &mutex );

		// Break if 'breaker' = 1 (signalled by main process)
		if(breaker)
			break;
	}
}


/**
 *	\fn void *runService2()
 *
 *	@param None
 *
 *	@return Void
 *
 *	@brief Function for running task for thread 2
 *
 *		This function runs in an infinite loop breakable by global
 *		variable breaker (signalled by main process). It makes the
 *		thread sleep for time indicated by slTime2 and then locks the
 *		thread my mutex . After this, it waits for semaphores Dal and Bhaat
 *		to become zero, makes entry for them using enterLog(2, item_name), and then increaments
 *		value of semaphore for next processes. After that, lock is opened. If
 *		breaker is '1', it break out of loop otherwise same process is repeated again
*/
void *runService2()
{

	while(1){

		// Make the function to sleep
		sleep( slTime2 );

		// Lock the thread
		pthread_mutex_lock( &mutex );


			/*  Dal */
		// Wait for semaphore to become zero
		sem_wait( &dalSem );
		// Make the entry in LOG file
		enterLog(2, "Dal");
		// Increament semaphore value for next process
		sem_post(&dalSem);


			/*  Bhaat */
		// Wait for semaphore to become zero
		sem_wait( &bhaatSem );
		// Make the entry in the LOG file
		enterLog(2, "Bhaat");
		// Increament semaphore value for next process
		sem_post(&bhaatSem);

		// Unlock the thread
		pthread_mutex_unlock( &mutex );


		// Break if 'breaker' = 1 (signalled by main process)
		if(breaker)
			break;
	}
}


/**
 *	\fn void *runService3()
 *
 *	@param None
 *
 *	@return Void
 *
 *	@brief Function for running task for thread 3
 *
 *		This function runs in an infinite loop breakable by global
 *		variable  breaker (signalled by main process). It makes the
 *		thread sleep for time indicated by slTime3 and then locks the
 *		thread my  mutex . After this, it waits for semaphores Dal and Bhaat
 *		to become zero, makes entry for them using enterLog(3, item_name), and then increaments
 *		value of semaphore for next processes. After that, lock is opened. If
 *		breaker is '1', it break out of loop otherwise same process is repeated again
*/
void *runService3()
{
	while(1){

		// Make the function to sleep
		sleep( slTime3);

		// Lock the thread
		pthread_mutex_lock( &mutex );

			/*  Bhaat */
		// Wait for semaphore to become zero
		sem_wait( &bhaatSem );
		// Make the entry in LOG file
		enterLog(3, "Bhaat");
		// Increament semaphore value for next process
		sem_post(&bhaatSem);

			/*  Maach */
		// Wait for semaphore to become zero
		sem_wait( &maachSem );
		// Make the entry in the LOG file
		enterLog(3, "Maach");
		// Increament semaphore value for next process
		sem_post(&maachSem);


		// Unlock the thread

		pthread_mutex_unlock( &mutex );
		// Break if 'breaker' = 1 (signalled by main process)
		if(breaker)
			break;
	}
}


/**
 *	\fn void initAll()
 *
 *	@param None
 *
 *	@return Void
 *
 *	@brief Initialise parameters needed for running all processes(e.g. File, threads etc)
 *
 *		This function opens file indicated by fileName and writes title, initializes  mutex,
 *		initializes three semaphores  maachSem ,  dalSem,  bhaatSem , initializes attributes
 *		for three threads (default values) then create three threads and call processes in them
*/
void initAll()
{
	int i; // iterator
	FILE *fp; // file pointer for LOG file

	// open LOG file and write titles
	fp = fopen( fileName, "w");
	fprintf(fp, "Sl.No\tThread No\tITEM\t TIME (in Microseconds)\n");
	fclose( fp );

	// Initialize break flag to '0', no breaking
	breaker = 0;

	// Initialize mutex
	pthread_mutex_init( &mutex, NULL);

	// Initialize all semaphores
	sem_init( &maachSem, 0, 1);
	sem_init( &dalSem, 0 ,1);
	sem_init( &bhaatSem, 0 ,1);

	for (i = 0; i < 3; i++)
		pthread_attr_init( &attr[i]);

	// Get the starting time
	gettimeofday(&start, NULL);

	// Create threads and call processes
	pthread_create( &service[0], &attr[0], runService1, NULL );
	pthread_create( &service[1], &attr[1], runService2, NULL );
	pthread_create( &service[2], &attr[2], runService3, NULL );


}

/**
 *	\fn int main(int argc, char *argv[])
 *
 *	@param argc Number of arguments provided by user
 *	@param argv[] Arguments for deciding sleep time for threads and for opening specific LOG file
 *
 *	@return '0' for normal exit, '-1' for abnormal exit/error
 *
 *	@brief Main process/ Chef process. Runs three threads and makes LOG entry for resource allocation by them
 *
 *		If no argument is provided (no argv[1]), then delay is equal to  FIXED_SLEEP_TIME and  logFixedDelay.txt is the LOG file. Otherwise,
 *		if argv[1] is '1': Fixed delay, delay = argv[2],  logFixedDelay.txt is the LOG file
 *		If argv[2] is '2': Random delay, limited by  MAX_SLEEP_TIME,  logRandDelay.txt is the LOG file
 *		Otherwise, No delay i.e. delay = 0, logNoDelay.txt is the LOG file
*/
int main(int argc, char *argv[])
{
	int status; // Status of the join_thread
	int i; // iterator


	// If arguments are given
	if( argc > 1){

		// First argument
		i = atoi( argv[1] );


		switch(i){
			// Fixed Delay
			case 1:
				if (argc < 2){
					printf("Please Provide Sleep Time \n");
					return -1;
				}

				// Set global time values
				slTime1 = slTime2 = slTime3 = atoi( argv[2] );

				// Set filename
				sprintf(fileName, "%s", "logFixedDelay.txt");
				break;

			// Random Delay
			case 2:
				// Set global time values
				slTime1 = rand() % MAX_SLEEP_TIME;
				slTime2 = rand() % MAX_SLEEP_TIME;
				slTime3 = rand() % MAX_SLEEP_TIME;

				// Set filename
				sprintf(fileName, "%s", "logRandDelay.txt");
				break;

			default:
				// Delay = 0, as default
				slTime1 = slTime2 = slTime3 = 0;
				// Set filename
				sprintf(fileName, "%s", "logNoDelay.txt");
		}
	}

	// If no argument is given
	else{
		slTime1 = slTime2 = slTime3 = FIXED_SLEEP_TIME;

		sprintf(fileName, "%s", "logFixedDelay.txt");
	}

	// Initialize all threads and run processes
	initAll();

	// Wait for some time
	sleep(10);

	// Break the loop
	breaker = 1;

	// Join all the threads
	for (i = 0; i < 3; i++){
		status = pthread_join( service[i], NULL);

		if( status != 0)
			perror("pthread");
	}

	return 0;
}
