#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *worker(void *arg) {
	int num = *(int*)arg;
    	if(num == 0)
		printf("Thread 0: Hello from thread 0\n");
	else if(num == 1)
		printf("Thread 1: Shahd Derbass\n");
	else if(num == 2)
		printf("Thread 2: 2026\n");
    	
	return NULL;
}

int main(void) {
    	pthread_t tid[3];
	int thread_id[3];
	for(int i = 0; i < 3; i++){
		thread_id[i] = i;
    		if (pthread_create(&tid[i], NULL, worker, &thread_id[i]) != 0) {
        		perror("pthread_create");
        		exit(1);
    	}
	
	}
    	for(int i = 0; i < 3; i++){

		if (pthread_join(tid[i], NULL) != 0) {
        		perror("pthread_join");
        		exit(1);
    		}
	}
    	printf("Main thread exiting.\n");
    	return 0;
}

