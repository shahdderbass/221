#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1000000     // Number of increments per thread
#define T 4           // Number of threads

long counter = 0;     // Shared global variable

void *worker(void *arg) {
    for (int i = 0; i < N; i++) {
        counter++;    // Increment shared counter
    }
    return NULL;
}

int main(void) {
    pthread_t threads[T];

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

    printf("Expected counter value = %d\n", N * T);
    printf("Actual   counter value = %ld\n", counter);

    return 0;
}

