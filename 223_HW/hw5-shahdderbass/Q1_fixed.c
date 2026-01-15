/*
Shahd Derbass
ID: 2231172085

HW5
Q1

To fix the race condition, one mutex must be used to protect the entire operation. Locking each variable separately still allows other threads to run between the steps, causing races. By using one lock for both variables, the whole operation happens atomically and the race condition is removed.

*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "csapp.h"
int A = 100;
int B = 100;

sem_t lock;

void *thread(void *arg) {
	// locking the whole operation
	P(&lock);
    	A -= 10;                      
    	B += 10;                      
    	V(&lock);

    	return NULL;
}

int main() {
    	pthread_t tid1, tid2;
	
	Sem_init(&lock,0,1);

    	pthread_create(&tid1, NULL, thread, NULL);
    	pthread_create(&tid2, NULL, thread, NULL);

    	pthread_join(tid1, NULL);
    	pthread_join(tid2, NULL);
	
    	printf("A = %d, B = %d\n", A, B);

    	return 0;
}

