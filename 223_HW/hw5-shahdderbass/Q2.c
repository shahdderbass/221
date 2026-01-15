/*
Shahd Derbass
ID: 2231172085

HW5
Q2

This program creates two threads, one representing a writer and one representing a reader, and uses semaphores to solve a writer-priority version of the Readers–Writers problem. A writer is allowed to block new readers by acquiring the readTry semaphore, ensuring that writers always get priority to access the shared resource before readers. Multiple readers can still read concurrently when no writer is waiting. The program prints the actions of each thread to clearly demonstrate the synchronization behavior.
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "csapp.h"

int i = 0, readcnt, writecnt;
sem_t 	mutex;		// lock to safely change readcnt 
sem_t	wmutex;		// lock to safely change writecnt
sem_t	readTry;	// to block readers
sem_t	shared;		// to lock shared data "i"

void *writer(void *args){
	while(1){
		P(&wmutex);
		writecnt++;
		if(writecnt == 1)
			P(&readTry);		// blocks readers
		V(&wmutex);
		
		P(&shared);
		i++;
		printf("Writer updated value to %d\n", i);
		V(&shared);

		P(&wmutex);
		writecnt--;
		if(writecnt == 0)
			V(&readTry);		// unblocks readers
		V(&wmutex);
	
		sleep(1);
	}
	return NULL;
}

void *reader(void *args){
	while(1){
		P(&readTry);			// checks if writer blocked reader
		P(&mutex);
		readcnt++;
		if(readcnt == 1)
			P(&shared);
		V(&mutex);
		
		printf("Reader read value: %d\n", i);
		
		P(&mutex);
		readcnt--;
		if(readcnt == 0)
			V(&shared);
		V(&mutex);
		V(&readTry);
		
		sleep(1);
	}
	return NULL;
}
	
int main(){
	pthread_t tid1;	// writer
	pthread_t tid2;	// reader

	Sem_init(&mutex, 0, 1);
    	Sem_init(&wmutex, 0, 1);
    	Sem_init(&readTry, 0, 1);
    	Sem_init(&shared, 0, 1);

	pthread_create(&tid1, NULL, writer, NULL);
	pthread_create(&tid2, NULL, reader, NULL);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	return 0;
}
