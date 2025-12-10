#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    int balance;
} account_t;

account_t acc;

void *deposit(void *arg) {
    for (int i = 0; i < 100000; i++) {
        acc.balance = acc.balance + 1;
    }
    return NULL;
}

void *withdraw(void *arg) {
    for (int i = 0; i < 100000; i++) {
        acc.balance = acc.balance - 1;
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;

    acc.balance = 0;

    if (pthread_create(&t1, NULL, deposit, NULL) != 0) {
        perror("pthread_create deposit");
        exit(1);
    }

    if (pthread_create(&t2, NULL, withdraw, NULL) != 0) {
        perror("pthread_create withdraw");
        exit(1);
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final balance = %d\n", acc.balance);

    return 0;
}

