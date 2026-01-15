/*
Shahd Derbass
ID: 2231172085

HW5
Q3

To fix the deadlock that might happen as previously mentioned in Q3.txt, I will let both workers lock semaphores in the same order, "printer_access" then "printer_available", this prevents one worker from holding A while the other holds B, this way the deadlock cannot occur.

*/
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "csapp.h"

sem_t printer_access, printer_available;
// printer_access controls access to the printer
// printer_available indicates whether the printer is available or not


void *worker(void *args){
	int id = (int)(long)args;
	
	while(1){
		printf("Worker %d wants to use the printer.\n", id);

		P(&printer_access);
		P(&printer_available);

		printf("Worker %d is printing...\n", id);
		sleep(1);
		printf("Worker %d finished printing...\n",id);

		V(&printer_access);
		V(&printer_available);

		sleep(1);
	}
	return NULL;
}

int main(){
	pthread_t tid1, tid2;

	Sem_init(&printer_access,0,1);
	Sem_init(&printer_available,0,1);

	pthread_create(&tid1, NULL, worker, (void*)1);
	pthread_create(&tid2, NULL, worker, (void*)2);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	return 0;
}
