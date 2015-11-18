
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>	/*  This file is necessary for using shared			*/
#include <fcntl.h>
#include <semaphore.h>

#define SEM_NAME "/mysem"

int main()
{

	int shmid, status;
	int *a, b;
	int i;
	sem_t *semMy;
	semMy = sem_open("/mysem",  1);

	for( i = 0; i < 4; i++){
		//if(sem_post( semMy) == -1)
		//	perror("sem_post");
    sem_wait( semMy );
    printf("Howdy\n");
		//sem_getvalue( semMy, &b);
		//printf("Sem value: %d \n", *a);
		//sem_wait( semMy);
		sleep(2);
	}

	sem_close( semMy);
	sem_unlink(SEM_NAME);

	return 0;
}
