/** @file Assignment4a.c
*  @brief The Chef Process
*
*  The chef process is integral to the hotel. The role of this process is to cook and then put the
*  three resources, macch, dal and bhaat. But the chef process can cook and deliver only two items at once.
*  It models the scenario with the help of three threads.
*
*  @author Deep K. Shroti
*  @bug No know bugs.
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <error.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>

// #include <sys/time.h>
#include <time.h>

#define CODE_END_KEY 7000
#define DAL_COUNTER_KEY 7001
#define BHAAT_COUNTER_KEY 7002
#define MAACH_COUNTER_KEY 7003

#define handle_error_en(en, msg) \
do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

static int serial_num  = 0;

pthread_t waiterThread1, waiterThread2, waiterThread3;
pthread_mutex_t waiter_mutex;

sem_t *maachSem, *dalSem, *bhaatSem;
sem_t *bhaatEnable, *dalEnable, *maachEnable ;

FILE *fp;
int waiter_thread1_Sleep, waiter_thread2_Sleep, waiter_thread3_Sleep;


int code_end_shmid;
int *code_end;

int dal_counter_shmid, maach_counter_shmid, bhaat_counter_shmid;
int *maach_counter, *dal_counter, *bhaat_counter;




// Maach prepared
void *waiterThreadFunc1(void *param) {
  printf("waiterThreadFunc1:%d\n", getpid());
  sleep(4);
  while( 1 ){

    printf("waiterThreadFunc1 ");

    if( (maach_counter[0] <= 0) || (dal_counter[0] <= 0) || (bhaat_counter[0] <= 0) )
      break;


    /* acquire the mutex lock */
    sem_wait(maachSem);
    // sleep(1);
    pthread_mutex_lock(&waiter_mutex);
    printf("%d, %d, %d\n",maach_counter[0],dal_counter[0],bhaat_counter[0]);
    if(((bhaat_counter[0]<=0 && dal_counter[0]<=0) && (dal_counter[0]<=0 && maach_counter[0]<=0) && (maach_counter[0]<=0 && bhaat_counter[0]<=0)))
    break;
    else{  if (dal_counter[0] > 0){
      maach_counter[0]--;
      dal_counter[0]--;
      if(insertItem("Maach Dal", 1)) {
        fprintf(stderr, " Producer report error condition\n");
      }
      else {
        sem_post(bhaatEnable);
      }
    }
    else if(bhaat_counter[0] > 0){
      maach_counter[0]--;
      bhaat_counter[0]--;
      if(insertItem("Maach Bhaat", 1)) {
        fprintf(stderr, " Producer report error condition\n");
      }
      else {
        sem_post(dalEnable);
      }
    }

    /* release the mutex lock */
    pthread_mutex_unlock(&waiter_mutex);
    sleep(waiter_thread1_Sleep);
  }

}


// if(bhaat_counter == 0 && maach_counter == 0 && dal_counter == 0){
//   printf("waiterThreadFunc1 break");
//   break;
// }
}


// Dal prepared
void *waiterThreadFunc2(void *param) {
  printf("waiterThreadFunc2:%d\n", getpid());
  sleep(4);
  while(1){

    printf("waiterThreadFunc2 ");


    // if(!(((bhaat_counter[0]==0 && dal_counter[0]==0) || (dal_counter[0]==0 && maach_counter[0]==0) || (maach_counter[0]==0 && bhaat_counter[0]==0)) && (code_end[0]==1))){}

    if( (maach_counter[0] <= 0) || (dal_counter[0] <= 0) || (bhaat_counter[0] <= 0) )
      break;

    /* acquire the mutex lock */
    sem_wait(dalSem);
    // sleep(2);
    pthread_mutex_lock(&waiter_mutex);
    printf("%d, %d, %d\n",maach_counter[0],dal_counter[0],bhaat_counter[0]);

    if(((bhaat_counter[0]<=0 && dal_counter[0]<=0) && (dal_counter[0]<=0 && maach_counter[0]<=0) && (maach_counter[0]<=0 && bhaat_counter[0]<=0)))
    break;
    else{    printf("<<1>>");
    if (maach_counter[0] > 0){
      dal_counter[0]--;
      maach_counter[0]--;
      if(insertItem("Dal Maach", 2)) {
        fprintf(stderr, " Producer report error condition\n");
      }
      else {
        // printf("producer produced Bhaat");
        sem_post(bhaatEnable);
      }
    }
    else if(bhaat_counter[0] > 0){
      dal_counter[0]--;
      bhaat_counter[0]--;
      if(insertItem("Dal Bhaat", 2)) {
        fprintf(stderr, " Producer report error condition\n");
      }
      else {
        sem_post(maachEnable);
      }
    }


    /* release the mutex lock */
    pthread_mutex_unlock(&waiter_mutex);
    sleep(waiter_thread2_Sleep);
    // if(bhaat_counter == 0 && maach_counter == 0 && dal_counter == 0){
    //   printf("waiterThreadFunc2 break");
    //   break;
    // }
  }
}

}


// Bhaat Prepared
void *waiterThreadFunc3(void *param) {
  printf("waiterThreadFunc3:%d\n", getpid());
  sleep(4);
  while( 1 ){
    printf("waiterThreadFunc3 ");


    if( (maach_counter[0] <= 0) || (dal_counter[0] <= 0) || (bhaat_counter[0] <= 0) )
      break;

    /* acquire the mutex lock */
    sem_wait(bhaatSem);
    pthread_mutex_lock(&waiter_mutex);
    printf("%d, %d, %d\n",maach_counter[0],dal_counter[0],bhaat_counter[0]);
    if(((bhaat_counter[0]<=0 && dal_counter[0]<=0) && (dal_counter[0]<=0 && maach_counter[0]<=0) && (maach_counter[0]<=0 && bhaat_counter[0]<=0)))
    break;
    else{

      if (dal_counter[0] > 0){
        bhaat_counter[0]--;
        dal_counter[0]--;
        if(insertItem("Bhaat Dal", 3)) {
          fprintf(stderr, " Producer report error condition\n");
        }
        else {
          sem_post(maachEnable);
        }
      }
      else if(maach_counter[0] > 0){
        bhaat_counter[0]--;
        maach_counter[0]--;
        if(insertItem("Bhaat Maach", 3)) {
          fprintf(stderr, " Producer report error condition\n");
        }
        else {
          sem_post(dalEnable);
        }
      }

      /* release the mutex lock */
      pthread_mutex_unlock(&waiter_mutex);
      sleep(waiter_thread3_Sleep);
      // if(((bhaat_counter[0]==0 && dal_counter[0]==0) || (dal_counter[0]==0 && maach_counter[0]==0) || (maach_counter[0]==0 && bhaat_counter[0]==0)) && (code_end[0]==1))
      //   break;
      // if(dal_counter == 0 && maach_counter == 0 && dal_counter == 0){
      //     printf("waiterThreadFunc3 break");
      //     break;
      // }
    }
  }
}






/** @brief Insert Item write a given item and thread number on the log
*
*  This function helps to write on the log
*  It takes input as the item, thread number.
*  It calculates the time at which the process is being written.
*
*
*  @param char *item the item to print on log
*  @param int thread_num the tread which calls this function
*  @return prints Thread, Item, and Time on the log
*
*/
int insertItem(char *item, int thread_num) {

  serial_num++;
  fp = fopen("log","a");
  if( fp == NULL){
    printf("Error! File not Created\n");
    return -1;
  }else{
    fprintf(fp, "Wait: %3d\t\t%3d\t%12s\t ------- msec",serial_num, thread_num, item);
    fprintf(fp, "\t\tMaach:%3d\t Dal:%3d\t Bhaat:%3d Code_end:%3d\n",maach_counter[0], dal_counter[0], bhaat_counter[0], code_end[0]);
    fclose(fp);
    return 0;
  }
}




/** @brief  The main function
*
*  The main function accepts input from the user in terms of
*  the main funtion sleep time and the mode in which we want
*  the code to run i.e. to set the threads sleep
*  It creates three threads for different cases i.e. Maach-Dal, Dal-Bhaat, Bhaat-Maach
*  Then it sleep the main function for a given seconds
*  Finally it joins all the threads and it exits the program.
*
*  @param char* argv[] the main function sleep time and the mode of thread sleep time
*  @return void
*
*/
void main(int argc, char *argv[]){

  pid_t mypid, parpid;

  // initializations
  int thread_status;
  pthread_mutex_init(&waiter_mutex, NULL);

  // waiter_thread1_Sleep = atoi(argv[1]);
  // waiter_thread2_Sleep = atoi(argv[2]);
  // waiter_thread3_Sleep = atoi(argv[3]);
  // printf("waiter:thread1 sleep %d\n", waiter_thread1_Sleep);
  fp = fopen("log","w");
  if( fp == NULL){
    printf("Error! File not Created\n");
  }else{
    fprintf(fp,"\t\t\tSl.No.\tThread No.\tItem\tTime\n");
    fclose(fp);
  }
  waiter_thread1_Sleep = 1;
  waiter_thread2_Sleep = 1;
  waiter_thread3_Sleep = 1;
  ///////////////////////////////////////////////////////////////////////
  // maachSem = sem_open("/maachSem",0);
  // dalSem = sem_open("/dalSem",0);
  // bhaatSem = sem_open("/bhaatSem",0);

  dalEnable = sem_open("/dalEnable",O_CREAT, 0644,0);
  bhaatEnable = sem_open("/bhaatEnable",O_CREAT, 0644,0);
  maachEnable = sem_open("/maachEnable",O_CREAT, 0644,0);


  code_end_shmid = shmget((key_t)CODE_END_KEY, sizeof(int),0777|IPC_CREAT);
  code_end = (int*)shmat(code_end_shmid, 0,0);
  code_end[0] = 0;
  dal_counter_shmid = shmget((key_t)DAL_COUNTER_KEY, sizeof(int),0777|IPC_CREAT);
  dal_counter = (int*)shmat(dal_counter_shmid, 0,0);
  dal_counter[0] = 0;
  bhaat_counter_shmid = shmget((key_t)BHAAT_COUNTER_KEY, sizeof(int),0777|IPC_CREAT);
  bhaat_counter = (int*)shmat(bhaat_counter_shmid, 0,0);
  bhaat_counter[0] = 0;
  maach_counter_shmid = shmget((key_t)MAACH_COUNTER_KEY, sizeof(int),0777|IPC_CREAT);
  maach_counter = (int*)shmat(maach_counter_shmid, 0,0);
  maach_counter[0] = 0;

  // mypid = getpid();
  // sem_init(&dalEnable, 0, 0);
  // sem_init(&bhaatEnable, 0, 0);
  // sem_init(&maachEnable, 0, 0);

  mypid = getpid();
  parpid = getppid();
  printf("Child Waiter: PID = %u, PPID = %u\n", mypid, parpid);

  thread_status = pthread_create(&waiterThread1, NULL, waiterThreadFunc1, NULL);
  if (thread_status != 0)
  handle_error_en(thread_status, "pthread_create");
  thread_status = pthread_create(&waiterThread2, NULL, waiterThreadFunc2, NULL);
  if (thread_status != 0)
  handle_error_en(thread_status, "pthread_create");
  thread_status = pthread_create(&waiterThread3, NULL, waiterThreadFunc3, NULL);
  if (thread_status != 0)
  handle_error_en(thread_status, "pthread_create");
  maachSem = sem_open("/maachSem", O_CREAT, 0644, 0);
  dalSem = sem_open("/dalSem", O_CREAT, 0644, 0);
  bhaatSem = sem_open("/bhaatSem", O_CREAT, 0644, 0);
  printf("<<2>>");
  int i;
  for(i=0 ; i<2 ; i++){
    dal_counter[0]++;
    insertItem("Dal", 1);
    sem_post(dalSem);

    maach_counter[0]++;
    insertItem("Maach", 1);
    sem_post(maachSem);
    sleep(1);

    maach_counter[0]++;
    insertItem("Maach", 2);
    sem_post(maachSem);

    bhaat_counter[0]++;
    insertItem("Bhaat", 2);
    sem_post(bhaatSem);
    sleep(2);

    bhaat_counter[0]++;
    insertItem("Bhaat", 3);
    sem_post(bhaatSem);

    dal_counter[0]++;
    insertItem("Dal", 3);
    sem_post(dalSem);
    sleep(3);
  }

  // sleep timer for main function
  //  sleep(20);
  code_end[0] = 1;

  // Joining the threads to avoid Zombie threads..
  // sleep(100);
  // printf(">>>>>>>4\n");
  thread_status = pthread_join(waiterThread1, NULL);
  if (thread_status != 0)
  handle_error_en(thread_status, "pthread_join");
  printf("waiterThread1:joined\n");

  thread_status = pthread_join(waiterThread2, NULL);
  if (thread_status != 0)
  handle_error_en(thread_status, "pthread_join");
  printf("waiterThread2:joined\n");

  thread_status = pthread_join(waiterThread3, NULL);
  if (thread_status != 0)
  handle_error_en(thread_status, "pthread_join");
  printf("waiterThread3:joined\n");


  sem_unlink("/dalEnable");
  sem_unlink("/bhaatEnable");
  sem_unlink("/maachEnable");

  printf("\nWaiter: Work done...\n");
  exit(0);
}
