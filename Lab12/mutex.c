#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex;   

void *worker(void *arg) {
    int id = (int)(long)arg;

    printf("Thread %d: waiting to enter critical section\n", id);

    pthread_mutex_lock(&mutex);   

    printf("Thread %d: inside critical section\n", id);
    sleep(1);                    
    printf("Thread %d: leaving critical section\n", id);

    pthread_mutex_unlock(&mutex); 

    return NULL;
}

int main(void) {
    pthread_t t1, t2, t3;

    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("pthread_mutex_init");
        exit(1);
    }

    pthread_create(&t1, NULL, worker, (void *)0);
    pthread_create(&t2, NULL, worker, (void *)1);
    pthread_create(&t3, NULL, worker, (void *)2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    pthread_mutex_destroy(&mutex);

    return 0;
}

