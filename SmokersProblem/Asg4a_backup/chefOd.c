#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

sem_t maachSem;
sem_t dalSem;
sem_t bhaatSem;


struct my_arg{
	int id;
	int slTime;
	char *p;
};


struct my_arg argThrd[3];


struct timeval start, stop;

pthread_mutex_t mutex;

pthread_t service[3];
pthread_attr_t attr[3];

static int count = 1;
int breaker;

void enterLog(int id, char *item1)
{
	gettimeofday(&stop, NULL);
	FILE *fp;

	fp = fopen("log.txt", "a");

	fprintf(fp, "%d \t\t %d \t\t %s \t %lu\n", count++, id, item1, ( stop.tv_usec - start.tv_usec ));

	fclose(fp);
}

void *runService1()
{
	
	while(1){

		sleep( rand() % 4);
		
		pthread_mutex_lock( &mutex );

		sem_wait( &maachSem );
		enterLog(1, "Maach");
		sem_post(&maachSem);


		sem_wait( &dalSem );
		enterLog(1, "Dal");
		sem_post(&dalSem);
		

		

		pthread_mutex_unlock( &mutex );


		if(breaker)
			break;
	}
}

void *runService2()
{
	
	while(1){

		sleep( rand() % 4);
		
		pthread_mutex_lock( &mutex );

		
		sem_wait( &dalSem );
		enterLog(2, "Dal");
		sem_post(&dalSem);

		sem_wait( &bhaatSem );
		enterLog(2, "Bhaat");
		sem_post(&bhaatSem);
		

		pthread_mutex_unlock( &mutex );
		

		if(breaker)
			break;
	}
}

void *runService3()
{
	while(1){
	
		sleep( rand() % 4);
		
		pthread_mutex_lock( &mutex );

		sem_wait( &bhaatSem );
		enterLog(3, "Bhaat");
		sem_post(&bhaatSem);

		sem_wait( &maachSem );
		enterLog(3, "Maach");
		sem_post(&maachSem);



		pthread_mutex_unlock( &mutex );

		if(breaker)
			break;
	}
}

void *runService(void *myStruct)
{

	int i1, slTime1;
	struct my_arg *data1 = (struct my_arg *)myStruct;

	i1 = (data1 -> id);
	slTime1 = (data1 -> slTime);

	printf("data: %d, %d \n",i1, slTime1 );


	while( 1 ){


		sleep( slTime1 );


		pthread_mutex_lock( &mutex );

		
		switch( i1 ){
			case 1:
				sem_wait( &maachSem );
				enterLog(1, "Maach");
				sem_post( &maachSem);

				sem_wait( &dalSem );
				enterLog(1, "Dal");
				sem_post( &dalSem);

				break;	

			case 2:
				sem_wait( &dalSem );
				enterLog(2, "Dal");
				sem_post( &dalSem);

				sem_wait( &bhaatSem );
				enterLog(2, "Bhaat");
				sem_post( &bhaatSem);

				break;	
			
			case 3:
				sem_wait( &bhaatSem );
				enterLog(3, "Bhaat");
				sem_post( &bhaatSem);

				sem_wait( &maachSem );
				enterLog(3, "Maach");
				sem_post( &maachSem);

				break;	
				
		}

		pthread_mutex_unlock( &mutex );

		//printf("");

		if(breaker)
			break;

	}

	return NULL;
}

void initAll()
{
	int i;
	FILE *fp;
	
	fp = fopen( "log.txt", "w");
	fprintf(fp, "Sl.No\tThread No\tITEM\t TIME (in Microseconds)\n");
	fclose( fp );

	pthread_mutex_init( &mutex, NULL);

	sem_init( &maachSem, 0, 1);
	sem_init( &dalSem, 0 ,1);
	sem_init( &bhaatSem, 0 ,1);

	breaker = 0;
	
	for (i = 0; i < 3; i++){
		
		pthread_attr_init( &attr[i]);

		argThrd[i].id = (i + 1);
		argThrd[i].slTime = 1;
		argThrd[i].p = "ultron";

		printf("Starting %d\n", argThrd[i].id);
		pthread_create( &service[i], &attr[i], runService, (argThrd+i) );
	}


}


int main(int argc, char *argv[])
{
	int status;
	int i;

	gettimeofday(&start, NULL);
	
	initAll();

	
	sleep(8);
	breaker = 1;

	for (i = 0; i < 3; i++){
		status = pthread_join( service[i], NULL);

		if( status != 0)
			perror("pthread");
	}

	return 0;
}