#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *print_message(void *arg) {
    const char *msg = (const char *)arg;
    printf("%s\n", msg);
    return NULL;
}

int main(void) {
    pthread_t t1, t2;

    if (pthread_create(&t1, NULL, print_message, "Message from thread 1") != 0) {
        perror("pthread_create t1");
        exit(1);
    }

    // TODO: create t2 to run print_message with argument "Message from thread 2"
    // Fill the missing line below.
    // if (pthread_create(...)) { ... }
    if (pthread_create(&t2, NULL, print_message, "Message from thread 2") != 0) {
        perror("pthread_create t2");
        exit(1);
    }
    if (pthread_join(t1, NULL) != 0) {
        perror("pthread_join t1");
        exit(1);
    }
    if (pthread_join(t2, NULL) != 0) {
        perror("pthread_join t2");
        exit(1);
    }

    printf("Both threads finished.\n");
    return 0;
}

