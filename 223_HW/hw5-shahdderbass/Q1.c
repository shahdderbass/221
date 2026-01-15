/*
Shahd Derbass
ID: 2231172085

HW5
Q1

In the following code, the operation needs two shared variables, but they are locked separately, not together.So other threads can run in between the two locks, leading to inconsistent results.
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "csapp.h"
int A = 100;
int B = 100;

sem_t lockA, lockB;

void *thread(void *arg) {
	P(&lockA);
    	A -= 10;                      
    	V(&lockA);

    // Race condition occurs here
    // Another thread might run its Step 1 before this thread does Step 2

    	P(&lockB);
    	B += 10;                      
    	V(&lockB);

    	return NULL;
}

int main() {
    	pthread_t tid1, tid2;

    	pthread_create(&tid1, NULL, thread, NULL);
    	pthread_create(&tid2, NULL, thread, NULL);

    	pthread_join(tid1, NULL);
    	pthread_join(tid2, NULL);
	
    	printf("A = %d, B = %d\n", A, B);

    	return 0;
}

