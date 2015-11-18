/**
* @file chef.c
*
*	@author Vishnu Dutt Sharma (12EC35018)
*
*	@brief Function to generate resources (Maach, Dal & Bhaat) as described in Assignment 4.
*
*		Function takes user input to perform task allocation on 3 threads(tasks being semaphore signalling).
*		The user input decides the sleep time for the processes running in threads.
*		If no argument is provided (no argv[1]), then delay is equal to FIXED_SLEEP_TIME and logFixedDelay.csv is the LOG file. Otherwise,
*   If three arguments are provided, they are stored in sleep time varibles for the three processes
*		Else if argv[1] is '1': Fixed delay, delay = argv[2], logFixedDelay.txt is the LOG file
*		Else If argv[2] is '2': Random delay, limited by MAX_SLEEP_TIME, logRandDelay.txt is the LOG file
*		Otherwise, No delay i.e. delay = 0, logNoDelay.txt is the LOG file
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/stat.h>

/**
*	@brief \def MAX_SLEEP_TIME Maximum time upto which sleep time (int) can be provided to processes when using random sleep time allocation
*	@brief \def FIXED_SLEEP_TIME This time (int) is the default time when no user input is given
*/
#define MAX_SLEEP_TIME 6
#define FIXED_SLEEP_TIME 1


/**
 * SHARED_MEM_COUNTER value of the key for shared memory used for storing counter for serial number in LOG file
 * SHARED_MEM_BREAK value of the key for shared memory used for storing the variable responsible for breaking the process loops
 * SHARED_MEM_ITEMS value of the key for shared memory used  for storing number of Maach, Dal and Bhaat produced by Chef process
 */
#define SHARED_MEM_COUNTER 1000
#define SHARED_MEM_BREAK 1001
#define SHARED_MEM_ITEMS 1002

/**
* SEM_NAME_MAACH string used as name in named semaphore for Maach
* SEM_NAME_DAL string used as name in named semaphore for Dal
* SEM_NAME_BHAAT string used as name in named semaphore for Bhaat
*/
#define SEM_NAME_MAACH "/semMaach"
#define SEM_NAME_DAL "/semDal"
#define SEM_NAME_BHAAT "/semBhaat"



/**
*	@brief \var maachSem Pointer to semaphore for 'Maach' (as provided in Assignment 4)
*	@brief \var dalSem Pointer to semaphore for 'Dal' (as provided in Assignment 4)
*	@brief \var bhaatSem Pointer to semaphore for 'Bhaat' (as provided in Assignment 4)
* @brief \var maachVal Value of the \var maachSem
* @brief \var dalVal Value of the \var dalSem
* @brief \var bhaatVal Value of the \var bhaatSem
*/
sem_t *maachSem, *dalSem , *bhaatSem;
int maachVal, dalVal, bhaatVal;

/**
 * @brief Structure for passing arguments to function in 'pthread_create'
 *  id: process id, 1: Maach & Dal, 2: Dal & Bhaat, 3: Bhaat & Maach
 *  slTime: slTime for the process
 */
struct my_arg{
  int id;
  int slTime;
};
struct my_arg argThrd[3];


/**
 *  @brief shmid Shared memory ID for variable used for breaking process
 *  @brief shmItem Shared memory ID for integer array of size 3, total number of items
 *  @brief shmCount Shared memory ID for counter for writing Serial number in the LOG file
 *  @brief chefAddr Variable for storing process breaker varible
 *  @brief itemCnt Variable for storing total number of resources available (Maach, Dal & Bhaat)
 *  @brief count Variable for storing ciunter for Serial Number
 */
int shmid, shmItem, shmCount;
int *chefAddr;
int *itemCnt;
int *count;

/**
* @brief \var start Variable for storing starting time of the process
* @brief \var stop Variable for storing current time
*	@brief \var slTime Sleep time array for three threads
*/
struct timeval start, stop;
int slTime[3];

/**
*	@brief \var chefMutex Mutex for avoiding deadlock
*/
pthread_mutex_t chefMutex;

/**
*	@brief \var service[3] Array of three threads (pthread)
*	@brief \var chefAttr[3] Array of attributes for three threads in array 'service[]''
*/
pthread_t service[3];
pthread_attr_t chefAttr[3];

/**
*	@brief \var breaker Flag for breaking out of loop running in threads (for breaking, breaker = 1)
*/
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
*		'Thread Number', 'Entry Maker','Item name' and 'Time elasped since start of 'Chef' process
*/
void enterLog(int id, char *item1)
{

  // Get current time
  gettimeofday(&stop, NULL);

  FILE *fp; //File pointer for opening file

  // OPen file in append mode
  fp = fopen(fileName, "a");

  if (fp == NULL)
    perror("fopen");
  // Make entry
  fprintf(fp, "%d,Chef,%d,%s,%lu\n", count[0]++, id, item1, ( stop.tv_usec - start.tv_usec ));
  //*count = cnt;
  // Close the file
  fclose(fp);
}

/**
*	\fn void *runService()
*
*	@param None
*
*	@return myStruct Structure for getting argumnets: process id & sleep time
*
*	@brief Function for running task for all threads in chef processes
*
*		This function runs in an infinite loop breakable by global
*		variable breaker (signalled by main process). Using id and Switch, first it
*		chooses the type of resuorces and sleep time for thread. Then it makes the thread sleep for time indicated by slTime array and then locks the
*		thread by mutex . After this, it increaments
*		value of semaphore for next processes. After that, lock is opened. If
*		breaker is '1', it break out of loop otherwise same process is repeated again
*/
void *runService(void *myStruct)
{

  int i1, slTime1;
  struct my_arg *data1 = (struct my_arg *)myStruct;

  i1 = (data1 -> id);
  slTime1 = (data1 -> slTime);

  while( 1 ){

    breaker = chefAddr[0];
    sleep( slTime1 );

    pthread_mutex_lock( &chefMutex );

    // Process differ on the basis of the id
    switch( i1 ){
      // Maach & Dal
      case 1:
        enterLog(1, "Maach"); // Make the entry in the LOG
        itemCnt[0]++;// Increase the count in shared memory
        sem_post( maachSem); // Signal the semaphore for waiter process

        enterLog(1, "Dal"); // Make the entry in the LOG
        itemCnt[1]++;// Increase the count in shared memory
        sem_post( dalSem); // Signal the semaphore for waiter process

        break;

      // Dal & Bhaat
      case 2:
        enterLog(2, "Dal"); // Make the entry in the LOG
        itemCnt[1]++;// Increase the count in shared memory
        sem_post( dalSem); // Signal the semaphore for waiter process

        enterLog(2, "Bhaat"); // Make the entry in the LOG
        itemCnt[2]++;// Increase the count in shared memory
        sem_post( bhaatSem); // Signal the semaphore for waiter process

      break;

      // Bhaat & Maach
      case 3:
        enterLog(3, "Bhaat"); // Make the entry in the LOG
        itemCnt[2]++;// Increase the count in shared memory
        sem_post( bhaatSem); // Signal the semaphore for waiter process

        enterLog(3, "Maach"); // Make the entry in the LOG
        itemCnt[0]++;// Increase the count in shared memory
        sem_post( maachSem); // Signal the semaphore for waiter process

      break;

    }

    pthread_mutex_unlock( &chefMutex );

    //printf("Parent %d counts: %d, %d, %d\n", i1, itemCnt[0], itemCnt[1], itemCnt[2]);

    if(breaker)
      break;

  }

  return NULL;
}

/**
*	\fn void initChef()
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
void initChef()
{
  int i; // iterator
  FILE *fp; // file pointer for LOG file

  // open LOG file and write titles
  fp = fopen( fileName, "w");
  fprintf(fp, "Sl.No,Process Name,Thread No,Item,Time (in Microseconds)\n");
  fclose( fp );

  // Initialize break flag to '0', no breaking
  breaker = 0;


  // Initialize shared memory
  chefAddr = (int *) shmat(shmid, 0, 0);
  itemCnt = (int *) shmat( shmItem, 0, 0);
  count = (int *) shmat(shmCount, 0 ,0);

  count[0] = 1;

  chefAddr[0] = 0;
  itemCnt[0] = itemCnt[1] = itemCnt[2] = 0;

  // Initialize mutex
  pthread_mutex_init( &chefMutex, NULL);

  // Initialize all semaphores
  maachSem = sem_open( SEM_NAME_MAACH, O_CREAT, 0644, 1);
  dalSem = sem_open( SEM_NAME_DAL, O_CREAT, 0644, 1);
  bhaatSem = sem_open( SEM_NAME_BHAAT, O_CREAT, 0644, 1);



  // Get the starting time
  gettimeofday(&start, NULL);

  // Create threads and call processes
  for (i = 0; i < 3; i++){

    // Initialize thread attributes
    pthread_attr_init( &chefAttr[i]);

    // Provide arguments
    argThrd[i].id = (i + 1);
    argThrd[i].slTime = slTime[i];

    // Create the thread and call process
    pthread_create( &service[i], &chefAttr[i], runService, (argThrd+i) );
  }
}


/**
*	\fn void runChef()
*
*	@param numC Number of argumnets, equal to 'argc'
* @param fileC Same as 'argv'
*
*	@return Void
*
*	@brief Function to decide sleep time, LOG file name and to run the Chef process
*
*/
void runChef(int numC, char **fileC)
{
  int status; // Status of the join_thread
  int i; // iterator


  // If arguments are given
  if(numC == 4){
    slTime[0] = atoi( fileC[1] );
    slTime[1] = atoi( fileC[2] );
    slTime[2] = atoi( fileC[3] );
  }
  else if( numC > 1){

    // First argument
    i = atoi( fileC[1] );


    switch(i){
      // Fixed Delay
      case 1:
      if (numC < 2){
        printf("Please Provide Sleep Time \n");
        return;
      }

      // Set global time values
      slTime[0] = slTime[1] = slTime[2] = atoi( fileC[2] );

      // Set filename
      sprintf(fileName, "%s", "logFixedDelay.txt");
      break;

      // Random Delay
      case 2:
      // Set global time values
      slTime[0] = rand() % MAX_SLEEP_TIME;
      slTime[1] = rand() % MAX_SLEEP_TIME;
      slTime[2] = rand() % MAX_SLEEP_TIME;

      // Set filename
      sprintf(fileName, "%s", "logRandDelay.txt");
      break;

      default:
      // Delay = 0, as default
      slTime[0] = slTime[1] = slTime[2] = 0;
      // Set filename
      sprintf(fileName, "%s", "logNoDelay.txt");
    }
  }

  // If no argument is given
  else{
    slTime[0] = slTime[1] = slTime[2] = FIXED_SLEEP_TIME;

    sprintf(fileName, "%s", "logFixedDelay.csv");
  }

  // Initialize all threads and run processes
  breaker = 0;
  initChef();


  // Wait for some time and stop processes
  sleep(10);
  chefAddr[0] = 1;
  breaker = chefAddr[0];

  //printf("Parent Says: %d \n", chefAddr[0]);


  // Join all the threads
  for (i = 0; i < 3; i++){
    status = pthread_join( service[i], NULL);

    if( status != 0)
    perror("pthread");
  }
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
  int i, status;


  // Initialize all shared memories
  shmid = shmget( (key_t)SHARED_MEM_BREAK, 1*sizeof(int), 0777|IPC_CREAT);
  if( shmid < 0)
    perror("shmget");

  shmItem = shmget( (key_t)SHARED_MEM_ITEMS, 3*sizeof(int), 0777|IPC_CREAT);
  if( shmItem < 0)
    perror("shmget");

  shmCount = shmget( (key_t)SHARED_MEM_COUNTER, 1*sizeof(int), 0777|IPC_CREAT);
  if( shmCount < 0)
    perror("shmget");

  runChef(argc, argv);

  // Print final values of all resources
  printf("Total:\n \tMaach : %d \n \tDal: %d \n \tBhaat: %d \n", itemCnt[0], itemCnt[1], itemCnt[2]);


  // Remove all shared memories
  shmctl(shmid, IPC_RMID, 0);
  shmctl(shmCount, IPC_RMID, 0);
  shmctl(shmItem, IPC_RMID, 0);

  // Detach all semapores
  sem_close( maachSem);
  sem_close( dalSem);
  sem_close( bhaatSem);

  // Unlink all named semaphores
  sem_unlink(SEM_NAME_MAACH);
  sem_unlink(SEM_NAME_DAL);
  sem_unlink(SEM_NAME_BHAAT);

  return 0;
}
