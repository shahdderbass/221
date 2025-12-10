#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *worker(void *arg) {
    printf("Hello from a worker thread!\n");
    return NULL;
}

int main(void) {
    pthread_t tid;

    if (pthread_create(&tid, NULL, worker, NULL) != 0) {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_join(tid, NULL) != 0) {
        perror("pthread_join");
        exit(1);
    }

    printf("Main thread exiting.\n");
    return 0;
}

