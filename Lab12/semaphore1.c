#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t semaphore;

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t semaphore;

void *worker(void *arg) {
    int id = (int)(long)arg;

    printf("Thread %d: waiting to enter critical section\n", id);
    sem_wait(&semaphore);   // down / P

    printf("Thread %d: inside critical section\n", id);
    sleep(1);               // simulate work
    printf("Thread %d: leaving critical section\n", id);

    sem_post(&semaphore);   // up / V
    return NULL;
}

int main(void) {
    pthread_t t1, t2, t3;
	/*
    // Initialize semaphore with value 1
    if (sem_init(&semaphore, 0, 1) != 0) {
        perror("sem_init");
        exit(1);
    }
	*/
    // Initialize semaphore with value 2
    if (sem_init(&semaphore, 0, 2) != 0) {
        perror("sem_init");
        exit(1);
    }
    pthread_create(&t1, NULL, worker, (void *)0);
    pthread_create(&t2, NULL, worker, (void *)1);
    pthread_create(&t3, NULL, worker, (void *)2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    sem_destroy(&semaphore);
    return 0;
}


