/**
* @file waiter.c
*
*	@author Vishnu Dutt Sharma (12EC35018)
*
*	@brief Function to collect and distribute resources (Maach, Dal & Bhaat) as described in Assignment 4a.
*
*		Function takes user input to perform task of picking resources on 3 threads(tasks being semaphore signalling).
*		The user input decides the sleep time for the processes running in threads.
*       Function MUST get three arguments, which are stored in sleep time varibles for the three processes
*		Otherwise, -1 is returned
*       The output of this process is stored in a LOG file named 'logFixedDelay.csv'
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

/**
 * SHARED_MEM_COUNTER value of the key for shared memory used for storing counter for serial number in LOG file
 * SHARED_MEM_BREAK value of the key for shared memory used for storing the variable responsible for breaking the process loops
 * SHARED_MEM_ITEMS value of the key for shared memory used  for storing number of Maach, Dal and Bhaat produced by Chef process
 * SHARED_MEM_SROT value of the key for shared memory used for Service Room Occumpancy Table
 * SHARED_MEM_TIME value of the key for shared memory used for storing start time
 * SHARED_MEM_OCCP value of the key for shared memory used for storing number of guests in occumpancy table
 * SHARED_MEM_MASTERBR value of the key for shared memory used for master break flag
 */
#define SHARED_MEM_COUNTER 1000
#define SHARED_MEM_BREAK 1001
#define SHARED_MEM_ITEMS 1002
#define SHARED_MEM_SROT 1100
#define SHARED_MEM_TIME 1500
#define SHARED_MEM_OCCP 2105
#define SHARED_MEM_MASTERBR 2205

/**
  * SEM_NAME_MAACH string used as name in named semaphore for Maach
  * SEM_NAME_DAL string used as name in named semaphore for Dal
  * SEM_NAME_BHAAT string used as name in named semaphore for Bhaat
  * SEM_MAACH_ENABLE string used as name in named semaphore maachEnable
  * SEM_DAL_ENABLE string used as name in named semaphore dalEnable
  * SEM_BHAAT_ENABLE string used as name in named semaphore for bhaatEnable
  */
#define SEM_NAME_MAACH "/semMaach"
#define SEM_NAME_DAL "/semDal"
#define SEM_NAME_BHAAT "/semBhaat"
#define SEM_MAACH_ENABLE "/maachEnable"
#define SEM_DAL_ENABLE "/dalEnable"
#define SEM_BHAAT_ENABLE "/bhaatEnable"


/**
 *	@brief \var maachEnable Pointer to semaphore for enabling 'Maach' in Service Room process
 *	@brief \var dalEnable Pointer to semaphore for enabling 'Dal' in Service Room process
 *	@brief \var bhaatEnable Pointer to semaphore for enabling 'Bhaat' in Service Room process
 */
sem_t *maachEnable, *dalEnable , *bhaatEnable;

/**
 *  @brief \var start Variable for storing starting time of the process
 *  @brief \var stop Variable for storing current time
 *  @brief \var slTime Sleep time array for three threads
 *  @brief \var semVal Array for storing current value of the semaphore
 *  @brief \var timeId Shared memory id for start time semaphore
 *  @brief \var masterBreakid Shared memory ID for master break flag
 *  @brief \var masterBreak Master break flag, 1 means breaking
 *	@brief \var maachSem Pointer to semaphore for 'Maach' (as provided in Assignment 4)
 *	@brief \var dalSem Pointer to semaphore for 'Dal' (as provided in Assignment 4)
 *	@brief \var bhaatSem Pointer to semaphore for 'Bhaat' (as provided in Assignment 4)
 *	@brief \var maachEnable Pointer to semaphore for enabling 'Maach' in Service Room process
 *	@brief \var dalEnable Pointer to semaphore for enabling 'Dal' in Service Room process
 *	@brief \var bhaatEnable Pointer to semaphore for enabling 'Bhaat' in Service Room process
 *  @brief \var maachVal Value of the \var maachSem
 *  @brief \var dalVal Value of the \var dalSem
 *  @brief \var bhaatVal Value of the \var bhaatSem
 */
struct timeval *start, stop;
int slTime[3];
int semVal[3];
int timeId;
int masterBreakid;
int *masterBreak;
sem_t *maachSem, *dalSem ,*bhaatSem, *maachEnable, *dalEnable , *bhaatEnable;
int maachVal, dalVal, bhaatVal;

/**
 * @brief Structure for passing arguments to function in 'pthread_create'
 * semType Type of the semaphore, 0: maach, 1: dal, 2: bhaat
 */
struct my_waiter{
    int semType; // 0: maach, 1: dal, 2: bhaat
};
struct my_waiter waiterStruct[3]; //Array for storing data for each waiter

/**
 *	@brief \var waiterMutex Mutex for avoiding deadlock
 */
pthread_mutex_t waiterMutex;

/**
 *	@brief \var waiterThrd[3] Array of three threads (pthread)
 *	@brief \var waiterAttr[3] Array of attributes for three threads in array 'waiterThrd[]''
 */
pthread_t waiterThrd[3];
pthread_attr_t waiterAttr[3];

/**
 *  @brief shmid Shared memory ID for variable used for breaking process
 *  @brief shmItem Shared memory ID for integer array of size 3, total number of items
 *  @brief shmCount Shared memory ID for counter for writing Serial number in the LOG file
 *  @brief waiterAddr Variable for storing process breaker varible
 *  @brief countItem Variable for storing total number of resources available (Maach, Dal & Bhaat)
 *  @brief count Variable for storing ciunter for Serial Number
 */
int shmid, shmItem, shmCount; //Shared memory
int *waiterAddr;
int *countItem;
int *count;

/**
 *	@brief \var fileName[256] String for storing different filenames for writing in different LOG files for different user inputs
 */
char fileName[256];

/**
 *\fn void enterLog(int id, char *item1)
 *
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

    // Make entry
    fprintf(fp, "%d,Waiter,%d,%s,%lu\n", count[0]++, id, item1, ( stop.tv_usec - start[0].tv_usec ));

    // Close the file
    fclose(fp);
}

/**
 *	\fn void *runModule()
 *
 *	@param Structure for getting argumnets: process type & sleep time
 *
 *	@return Void
 *
 *	@brief Function for collecting resources being produced by chef process
 *
 *		This function runs in an infinite loop breakable by global
 *		variable breaker (signalled by main process). Based on semType
 *    it waits for a semaphore signal and on the basis of the availablity
 *    of other two resources it collects one of them and decrements the
 *    number of resources
 */
void *runModule( void *myStruct)
{
    int type;

    // Initializing argument structure
    struct my_waiter *data1 = (struct my_waiter *)myStruct;
    // Getting type of semaphore
    type = data1->semType;

    printf("Initializing waiter %d \n", type);
    while(1){
        //printf("\t Counts: %d,%d, %d \n", countItem[0], countItem[1], countItem[2]);

        // Sleep for fixed time determined by user input
        sleep( slTime[type] );

        switch( type){
            case 0: // Maach Recieved
                sem_wait( maachSem);
                pthread_mutex_lock( &waiterMutex ); // Lock the thread

                if ( countItem[1] > 0 ){ //Dal Recieved
                    enterLog( 3, "Maach" );
                    enterLog( 3, "Dal" );

                    countItem[0]--;
                    countItem[1]--;

                    // Signal the availability
                    sem_post( maachEnable);
                }
                else if (countItem[2] > 0){ //Bhaat Received
                    enterLog( 2, "Maach" );
                    enterLog( 2, "Bhaat" );

                    countItem[0]--;
                    countItem[2]--;

                    // Signal the availablity
                    sem_post( maachEnable);
                }
                break;

            case 1:
                sem_wait( dalSem); // Dal Received
                pthread_mutex_lock( &waiterMutex ); // Lock the thread

                if ( countItem[2] > 0 ){ // Bhaat Received
                    enterLog( 1, "Dal" );
                    enterLog( 1, "Bhaat" );

                    countItem[1]--;
                    countItem[2]--;

                    // Signal the availability
                    sem_post( dalEnable);
                }
                else if (countItem[0] > 0){ // Maach Received
                    enterLog( 100, "Dal" );
                    enterLog( 100, "Maach" );

                    countItem[1]--;
                    countItem[0]--;

                    // Signal the availablity
                    sem_post( dalEnable);
                }
                break;

            case 2: // Bhaat Received
                sem_wait( bhaatSem);
                pthread_mutex_lock( &waiterMutex ); // Lock the thread

                if ( countItem[0] > 0 ){ // Maach Received
                    enterLog( 100, "Bhaat" );
                    enterLog( 100, "Maach" );

                    countItem[2]--;
                    countItem[0]--;

                    // Signal the availability
                    sem_post( bhaatEnable);

                }
                else if ( countItem[1] > 0 ){ // Dal Received
                    enterLog( 100, "Bhaat" );
                    enterLog( 100, "Dal" );

                    countItem[2]--;
                    countItem[1]--;

                    // Signal the availablity
                    sem_post( bhaatEnable);

                }
                break;

            default:;

        }

        pthread_mutex_unlock( &waiterMutex ); // Unlock the thread

        if(waiterAddr[0] == 1)
            if( ( (countItem[0] <= 0) && (countItem[1] <= 0) ) || ( (countItem[1] <= 0) && (countItem[2] <= 0) ) || ( (countItem[0] <= 0) && (countItem[2] <= 0) ) ){
                masterBreak[0] = 1;
                break;
            }

        if( masterBreak[0] )
        {
            printf("Master break on Waiter %d \n", type);
            break;
        }
    }

}

/**
 *	\fn void initWaiter()
 *
 *	@param None
 *
 *	@return Void
 *
 *	@brief Initialise parameters needed for running all processes(e.g. File, threads etc)
 *
 */
void initWaiter()
{
    // Initialize Shared memories
    shmid = shmget( (key_t)SHARED_MEM_BREAK, 1*sizeof(int), 0777|IPC_CREAT);
    if( shmid < 0)
    perror("shmget");

    shmItem = shmget( (key_t)SHARED_MEM_ITEMS, 3*sizeof(int), 0777|IPC_CREAT);
    if( shmItem < 0)
    perror("shmget");

    shmCount = shmget( (key_t)SHARED_MEM_COUNTER, 1*sizeof(int), 0777|IPC_CREAT);
    if( shmCount < 0)
    perror("shmget");

    timeId = shmget( (key_t)SHARED_MEM_TIME, sizeof(struct timeval), 0777|IPC_CREAT);
    start = (struct timeval *)shmat(timeId, 0 ,0);

    masterBreakid = shmget( (key_t)SHARED_MEM_MASTERBR, 1*sizeof(int), 0777|IPC_CREAT);
    masterBreak = (int *)shmat(masterBreakid, 0 ,0);


    // Attach shared memories
    waiterAddr = (int *) shmat(shmid, 0, 0);
    countItem = (int *) shmat(shmItem, 0 , 0);
    count = (int *) shmat(shmCount, 0, 1);

    // Open existing named semaphores
    maachSem = sem_open(SEM_NAME_MAACH, 0);
    if( maachSem == SEM_FAILED)
        printf("\n error maach Sem: %d \n", errno);

    dalSem = sem_open(SEM_NAME_DAL, 0);
    if( dalSem == SEM_FAILED)
        printf("\n error dal sem: %d \n", errno);

    bhaatSem = sem_open(SEM_NAME_BHAAT, 0);
    if( bhaatSem == SEM_FAILED)
        printf("\n error bhaat sem: %d \n", errno);

    // Create semaphores for service room
    maachEnable = sem_open( SEM_MAACH_ENABLE, O_CREAT, 0644, 0);
    dalEnable = sem_open( SEM_DAL_ENABLE, O_CREAT, 0644, 0);
    bhaatEnable = sem_open( SEM_BHAAT_ENABLE, O_CREAT, 0644, 0);


}

/**
 *	\fn void runWaiter()
 *
 *	@param None
 *
 *	@return Void
 *
 *	@brief Initialize and run all the threads
 *
 *    The function initializes mutex, thread attrinutes and threads and then runs processes
 *    on the threads
 */
void runWaiter()
{
    int i; // iterator

    initWaiter();
    // Initialize mutex
    pthread_mutex_init( &waiterMutex, NULL);

    // Create threads and call processes
    for (i = 0; i < 3; i++){

        // Initialize attributes
        pthread_attr_init( &waiterAttr[i]);

        // Write the semaphore type
        waiterStruct[i].semType = i;

        // Create the thread
        pthread_create( &waiterThrd[i], &waiterAttr[i], runModule, (waiterStruct+i) );
    }

    waiterAddr[0] = 0;

}


/**
 *	\fn int main()
 *
 *	@param argc Number of user inputs
 *   @param argv User inputs
 *
 *	@return Void
 *
 *	@brief Main function, initializes sleep time and decided file to be written to & runs waiter process
 */
int main(int argc, char *argv[])
{
    int i;

    sprintf(fileName, "%s" ,"logFixedDelay.csv");
    if(argc == 4){
        slTime[0] = atoi( argv[1] );
        slTime[1] = atoi( argv[2] );
        slTime[2] = atoi( argv[3] );
    }
    else{
        printf("Provide all arguments for waiter process \n");
        return -1;
    }

    // Run the main process
    runWaiter();

    while(1){

        //printf("child: %d, %d, %d \n", countItem[0], countItem[1], countItem[2] );

        // Break if master flag is 1
        if (masterBreak[0]){
            printf("Master break on waiter\n" );
            return ;
            break;
        }
        if( (countItem[0] == 0))
            break;

        sleep(1);
    }


    for (i = 0; i < 3; i++){
        pthread_join( waiterThrd[i], NULL);
    }

    // Remove all shared memories
    shmctl(shmid, IPC_RMID, 0);
    shmctl(shmCount, IPC_RMID, 0);
    shmctl(shmItem, IPC_RMID, 0);

    // Detach all the shared memories
    shmdt(countItem);
    shmdt(count);
    shmdt(waiterAddr);

    return 0;
}
