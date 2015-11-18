#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <semaphore.h>


#define NUM_GUESTS 30
#define SHARED_MEM_GUEST 2005


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

int guestPnt;
int *guestCount;

typedef struct SRO_tables{
  char name[16];
  float entryTime;
  int roomId;
}SRO_table;

SRO_table *mySRO;

void update_GuestBook( SRO_table *myTable)
{
  FILE *fp;

  fp = fopen("GuestBook.txt", "a");
  fprintf(fp, "%s,%f,%d\n", myTable->name, myTable->entryTime, myTable->roomId);

  fclose(fp);
}

void make_srot()
{
    int i = 0;
    mySRO = ( SRO_table *)malloc( NUM_GUESTS * sizeof(SRO_table));

    for(i  = 0; i < NUM_GUESTS; i++){
      sprintf((mySRO[i]).name, "Guest:%3d", i);
      (mySRO[i]).entryTime = i;
      (mySRO[i]).roomId = 1 + (i % 3);

    }
}

void serveGuest(int item)
{
  while(1){
    /*
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
    */
    update_GuestBook( &mySRO[ guestCount[0]++ ] );

    if(guestCount[0] >= NUM_GUESTS)
      break;
  }
}

void initService()
{
  maachEnable = sem_open( SEM_MAACH_ENABLE, 0);
  dalEnable = sem_open( SEM_DAL_ENABLE, 0);
  bhaatEnable = sem_open( SEM_BHAAT_ENABLE, 0);
}

int main(int argc, char *argv[])
{
  pid_t pid;
  int i;

  /*
  guestPnt = shmget( (key_t)SHARED_MEM_GUEST, 1*sizeof(int), 0777|IPC_CREAT);
  if( guestPnt < 0)
    perror("shmget");

  guestCount = (int *)shmat(guestPnt, 0, 0);

  guestCount[0] = 1;
*/

  //initService();
  make_srot();

  for(i = 0; i < NUM_GUESTS-1; i++)
    update_GuestBook( &mySRO[i] );
  //for(i = 0; i < 3; i++){
//    if ((pid = fork()) == 0){
//      serveGuest( i );
//    }
//  }

  //shmctl(guestCount, IPC_RMID, 0);
  /*
  sem_close( maachEnable);
  sem_close( dalEnable);
  sem_close( bhaatEnable);

  sem_unlink(SEM_MAACH_ENABLE);
  sem_unlink(SEM_DAL_ENABLE);
  sem_unlink(SEM_BHAAT_ENABLE);
  */
}
