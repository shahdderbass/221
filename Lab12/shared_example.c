#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 2

void *thread(void *vargp);

char **ptr;        // Global variable

int main(void) {
    int i;
    pthread_t tid;
    char *msgs[N] = {
        "Hello from foo",
        "Hello from bar"
    };

    ptr = msgs;

    for (i = 0; i < N; i++) {
        if (pthread_create(&tid, NULL, thread, (void *)(long)i) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }

    pthread_exit(NULL);
}

void *thread(void *vargp) {
    int myid = (int)(long)vargp;
    static int cnt = 0;
    printf("[%d]: %s (cnt = %d)\n", myid, ptr[myid], ++cnt);
    return NULL;
}

