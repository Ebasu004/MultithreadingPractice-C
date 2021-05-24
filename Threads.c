#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define Sync 1
//if unsync change to 0 if synced change to >0

int Current = 0; //declaring shared variable
void *add(void *ID); 
pthread_barrier_t b; //declaring barrier variable
pthread_mutex_t m; //declaring mutex variable

int main(int argc, char *argv[]){

	if(argv[1] == NULL){
		printf("The input value for threads is invalid\n End of Program...");
		return 0;
	}

	//number of threads must be under 50 more than 0 and a valid int

	int threadnum = atoi(argv[1]);
	
	if(threadnum < 1 || threadnum > 50){
		printf("The input value for threads is invalid\n End of Program...");
		return 0;
	}
	
	pthread_t threads[threadnum];
	int r, i, j;
	pthread_barrier_init(&b, NULL, threadnum);

	for(i = 0; i < threadnum; i++){
		void *ptr = (void*)&threads[i];
		if((r = pthread_create(&threads[i], NULL, &add, ptr))){
			printf("Thread creation failed...%d",r);
		}
	}
	
	for(j = 0; j < threadnum; j++){
		pthread_join(threads[j], NULL);
	}

	return 0;
}
//thread changing shared variable function
void *add(void *number){

	int count, val;
	int id = *((int*)(number));

	for(count = 0; count < 20; count++){ 
		//each thread adds up to 20
		if(random()>RAND_MAX/2){
			usleep(500);
		}
		//lock mutex
		#if Sync > 0 //if current sync is more than 0 run program synced
			pthread_mutex_lock(&m);
		#endif
		//print out current value and thread number
		val = Current;
		printf("The thread with number id %d sees value %d\n",id,val);
		Current = val +1;
		//unlock mutex
		#if Sync > 0
			pthread_mutex_unlock(&m);
		#endif
	}
	
	pthread_barrier_wait(&b);
	val = Current;
	printf("The thread with number id %d sees final value of %d\n",id,val);
	//print the final value that each thread sees
}


