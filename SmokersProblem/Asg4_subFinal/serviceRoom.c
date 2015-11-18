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
 *  NUM_GUESTS Number of guests in the Table
 *  SHARED_MEM_GUEST value of the key for shared memory used for storing guest details
 *  SHARED_MEM_OCCP value of the key for shared memory used for storing number of guests in occumpancy table
 *  SHARED_MEM_MASTERBR value of the key for shared memory used for master break flag
 *  SHARED_MEM_SROT value of the key for shared memory used for Service Room Occumpancy Table
 *  SHARED_MEM_TIME value of the key for shared memory used for storing start time
 */
#define NUM_GUESTS 30
#define SHARED_MEM_GUEST 2005
#define SHARED_MEM_OCCP 2105
#define SHARED_MEM_MASTERBR 2205
#define SHARED_MEM_SROT 1100
#define SHARED_MEM_TIME 1500


/**
 * SEM_MAACH_ENABLE string used as name in named semaphore maachEnable
 * SEM_DAL_ENABLE string used as name in named semaphore dalEnable
 * SEM_BHAAT_ENABLE string used as name in named semaphore for bhaatEnable
 */
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
 *  @brief \struct SRO_table A structure for making entries and keeping
 *  track of all guests
 *
 *  name: String for storing name of the guest
 *  entryTime: For storing the time of entry of the guests
 *  roomId: Room number
 */
typedef struct SRO_tables{
    char name[16];
    unsigned long int entryTime;
    unsigned long int exitTime;
    int roomId;
}SRO_table;

/**
 *  guestPnt: Shared memory pointer for serial number in the Guest Book
 *  guestCount: Shared variable for serial number in the Guest Book
 *  mySRO: Pointer to structure for guest details
 *  sroPnt: Shared memory pointer for guest detail structure
 *  srotValue: Service Room Occupancy Table value
 *  start: Pointer to start time value
 *  stop:  Stop time
 *  timeId: ID for shared time
 *  occpID: ID for shared occupancy variable
 *  occpInd: Shared array for storing index values of curent guests
 *  masterBreakid: ID for shared variable for master break flag
 *  masterBreak: Master break flag
 */
int guestPnt;
int *guestCount;
SRO_table *mySRO;
int srotPnt, *srotValue;
struct timeval *start, stop;
int timeId;
int occpID;
int *occpInd;
int masterBreakid;
int *masterBreak;

/**
 * \fn void update_GuestBook( SRO_table *myTable)
 * @param myTable Pointer to the data structure \struct SRO_table which handles the Service Room Occumpancy
 * @param item Variable for determining room id
 *
 * @return Void
 *
 * @brief Function for updating/ making entries in th Guest Book
 */
void update_GuestBook( SRO_table *myTable, int item)
{
    FILE *fp;

    gettimeofday(&stop, NULL);
    myTable->exitTime = stop.tv_usec - start[0].tv_usec;
    myTable->roomId = item;

    fp = fopen("GuestBook.txt", "a");
    fprintf(fp, "%s,%lu,%lu,%d\n", myTable->name, (myTable->entryTime), (myTable->exitTime), myTable->roomId);

    fclose(fp);
}


/**
 * \fn void serveGuest(int item)
 * @param item Variable for choosing between Maach, Dal, Bhaat related semaphores
 * @return Void
 *
 * @brief Function for updating/ making entries in th Guest Book
 *      Function makes entries in Guest Book file then starts semaphores. On getting signal it
 *      again updates the Guest Book
 */
void serveGuest(int item)
{
    int k = 0;
    while(1)
    {
        k = guestCount[0];
        //if(k >= NUM_GUESTS)
        //    break;

        if( masterBreak[0] )
            break;


        gettimeofday(&stop, NULL);
        mySRO[ guestCount[0] ].entryTime = stop.tv_usec - start[0].tv_usec;

        occpInd[ item ] = guestCount[0] % NUM_GUESTS;

        switch( item ){
            case 0:
                sem_wait( maachEnable);
                break;
            case 1:
                sem_wait( dalEnable);
                break;
            case 2:
                sem_wait( bhaatEnable);
                break;

        }

        // Update Guest Book
        update_GuestBook( &mySRO[ (guestCount[0]++) % NUM_GUESTS ], item+1  );

    }
}


/**
 * \fn void initService()
 * @param None
 * @return Void
 *
 * @brief Function for initializing semaphores, shared memories, start time etc.
 */
void initService()
{
    maachEnable = sem_open( SEM_MAACH_ENABLE, 0);
    dalEnable = sem_open( SEM_DAL_ENABLE, 0);
    bhaatEnable = sem_open( SEM_BHAAT_ENABLE, 0);

    srotPnt = shmget( (key_t)SHARED_MEM_SROT, NUM_GUESTS*sizeof( SRO_table ), 0777|IPC_CREAT);
    if( srotPnt < 0)
        perror("shmget");

    mySRO = (SRO_table *)shmat(srotPnt, 0, 0);

    timeId = shmget( (key_t)SHARED_MEM_TIME, sizeof(struct timeval), 0777|IPC_CREAT);
    start = (struct timeval *)shmat(timeId, 0 ,0);
}


/**
 * \fn int main(int argc, char *argv[])
 *	@param argc Number of arguments provided by user
 *	@param argv[] Arguments for deciding sleep time for threads and for opening specific LOG file
 *
 *	@return '0' for normal exit, '-1' for abnormal exit/error
 *
 *  @brief Main process, runs all other processes
 *
 *      Function initializes shared memories and calls processes related to main process
 */
int main(int argc, char *argv[])
{
    pid_t pid;
    int i;


    guestPnt = shmget( (key_t)SHARED_MEM_GUEST, 1*sizeof(int), 0777|IPC_CREAT);
    //if( guestPnt < 0)
    //    perror("shmget");

    guestCount = (int *)shmat(guestPnt, 0, 0);
    guestCount[0] = 0;

    occpID = shmget( (key_t)SHARED_MEM_OCCP, 3*sizeof(int), 0777|IPC_CREAT);
    occpInd = (int *)shmat(occpID, 0 ,0);
    occpInd[0] = occpInd[1] = occpInd[2] = 0;

    masterBreakid = shmget( (key_t)SHARED_MEM_MASTERBR, 1*sizeof(int), 0777|IPC_CREAT);
    masterBreak = (int *)shmat(masterBreakid, 0 ,0);

    printf("Initializing Service Room\n");

    initService();



    //for(i = 0; i < NUM_GUESTS-1; i++)
    //  update_GuestBook( &mySRO[i] );
    for(i = 0; i < 3; i++){
        if ((pid = fork()) == 0){
            serveGuest( i );
        }
    }

    //shmctl(guestPnt, IPC_RMID, 0);

    sem_close( maachEnable);
    sem_close( dalEnable);
    sem_close( bhaatEnable);

    sem_unlink(SEM_MAACH_ENABLE);
    sem_unlink(SEM_DAL_ENABLE);
    sem_unlink(SEM_BHAAT_ENABLE);

}
