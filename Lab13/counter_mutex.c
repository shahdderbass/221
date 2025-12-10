#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define N 1000000
#define T 4

long counter = 0;
pthread_mutex_t counter_lock;   // TODO: initialize this

void *worker(void *arg) {
    for (int i = 0; i < N; i++) {
    	pthread_mutex_lock(&counter_lock);
	counter++;
	pthread_mutex_unlock(&counter_lock);
    }
    return NULL;
}

int main(void) {
    pthread_t threads[T];

    pthread_mutex_init(&counter_lock, NULL);

    for (int i = 0; i < T; i++) {
        if (pthread_create(&threads[i], NULL, worker, NULL) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }

    for (int i = 0; i < T; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(1);
        }
    }

    pthread_mutex_destroy(&counter_lock);

    printf("Expected counter value = %d\n", N * T);
    printf("Actual   counter value = %ld\n", counter);
    return 0;
}

