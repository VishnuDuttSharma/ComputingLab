
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
	semMy = sem_open("/mysem", O_CREAT|O_EXCL, 0644, 0);

	sleep(1);
	for( i = 0; i < 4; i++){
		//if(sem_post( semMy) == -1)
		//	perror("sem_post");

		//sem_getvalue( semMy, &b);
		//printf("Sem value: %d \n", *a);
		sem_wait( semMy);
		//sleep(1);
	}

	sem_close( semMy);
	sem_unlink(SEM_NAME);

	return 0;
}
