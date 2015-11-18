/**
 * @file hotel.c
 *
 * @author Vishnu Dutt Sharma (12EC35018)
 *
 * @brief Function to run all the processes (Chef, Waiter, service room)
 *
 *	Function takes sleep time for threads in Chef and  Waiter as user input
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>


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
 *\fn void initParam()
 *
 *	@param None
 *
 *	@return Void
 *
 *	@brief Function for Initializing parameters (shared memory etc)
 *
 */
void initParam()
{
    // SROT: Shared Memory
    srotPnt = shmget( (key_t)SHARED_MEM_SROT, NUM_GUESTS*sizeof( SRO_table ), 0777|IPC_CREAT);
    if( srotPnt < 0)
    perror("shmget");

    mySRO = (SRO_table *)shmat(srotPnt, 0, 0);

    // Guest Count: Shared Memory
    guestPnt = shmget( (key_t)SHARED_MEM_GUEST, 1*sizeof(int), 0777|IPC_CREAT);
    //if( guestPnt < 0)
    //    perror("shmget");
    guestCount = (int *)shmat(guestPnt, 0, 0);

    // Time ID: Shared Memory
    timeId = shmget( (key_t)SHARED_MEM_TIME, sizeof(struct timeval), 0777|IPC_CREAT);
    start = (struct timeval *)shmat(timeId, 0 ,0);

    // Occupancy array: Shared Memory
    occpID = shmget( (key_t)SHARED_MEM_OCCP, 3*sizeof(int), 0777|IPC_CREAT);
    occpInd = (int *)shmat(occpID, 0 ,0);

    // Master break flag: Shared Memory
    masterBreakid = shmget( (key_t)SHARED_MEM_MASTERBR, 1*sizeof(int), 0777|IPC_CREAT);
    masterBreak = (int *)shmat(occpID, 0 ,0);
    masterBreak[0] = 0;

}

/**
 *\fn void print_srot()
 *
 *	@param None
 *
 *	@return Void
 *
 *	@brief Function for printing Service Room Occupancy Status
 *
 */
void print_srot()
{
    int i;

    printf("Guest Name\tEntry Time\tRoom ID\n");

    for( i = 0; i < 3; i++ ){
        printf("%s\t%lu\t\t%d\n", mySRO[ occpInd[i % NUM_GUESTS] ].name, mySRO[ occpInd[i % NUM_GUESTS] ].entryTime, mySRO[ occpInd[i % NUM_GUESTS] ].roomId );
    }
}

/**
 *\fn void print_guestbook()
 *
 *	@param None
 *
 *	@return Void
 *
 *	@brief Function for printing Guest Book
 *
 */
void print_guestbook()
{
    int i;

    printf("Sl. No.\tName\t\tEntry Time\tExit Time\tRoom No.\n");

    for (i  = 0; i < guestCount[0]; i++){
        printf("%4d\t%s\t%lu usec\t\t%lu usec\t%4d\n", i, mySRO[i % NUM_GUESTS].name, mySRO[i % NUM_GUESTS].entryTime, mySRO[i % NUM_GUESTS].exitTime, mySRO[i % NUM_GUESTS].roomId);
    }
}

/**
 *  \fn void make_srot()
 *  @param None
 *
 *  @return Void
 *
 *  @brief Function for making Service Room Occupancy Table
 */
void make_srot()
{
    int i = 0;

    for(i  = 0; i < NUM_GUESTS; i++){
        sprintf((mySRO[i]).name, "Guest:%3d", i);
        //(mySRO[i]).entryTime = i;
        //(mySRO[i]).roomId = 1 + (i % 3);

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
 *	@brief Main process, runs all other processes
 *   Function forks and runs other process using 'execlp'
 */
int main(int argc, char *argv[])
{
    int i, status, resp;
    pid_t pid1;

    if( argc != 7){
        printf("Please provide all the inputs ");
        return -1;
    }
    else{
        // Initialize parameters
        initParam();
        // Make the guest table
        make_srot();


        printf("Please choose an option: \n");
        printf("1. Print Guest Book \n2. Print Service Room Occumpancy Table \n3. Exit \n" );
        scanf("%d", &resp);

        // get current time
        gettimeofday(start, NULL);

        for(i = 0; i < 3; i++){

            pid1 = fork();
            if ( pid1 == 0){

                switch( i ){
                    case 0:
                    execlp("./chef.out", "./chef.out", argv[1], argv[2], argv[3], NULL);
                    break;

                    case 1:
                    execlp("./waiter.out", "./waiter.out", argv[4], argv[5], argv[6], NULL);
                    break;

                    case 2:
                    execlp("./serviceRoom.out", "./serviceRoom.out", NULL);
                    break;


                }
                sleep(1);
            }
        }
    }
    for(i = 0; i < 3; i++){
        wait(&status);
    }

    if (resp == 1)
        print_guestbook();
    else if (resp == 2)
        print_srot();
    else if(resp == 3)
        masterBreak[0] = 1;
    else
        printf("Please enter a valid input");

    shmctl( masterBreakid, IPC_RMID, 0);
    shmctl( occpID, IPC_RMID, 0);
    shmctl( timeId, IPC_RMID, 0);
    shmctl( srotPnt, IPC_RMID, 0);
    shmctl( guestPnt, IPC_RMID, 0);
    //while(1){
    //  printf("Enter 1 to exit at the end  \n\n");
    //    scanf( "%d", &i);
    //    if(i == 1)/
    //       break;
    //  }
    return 0;
}
