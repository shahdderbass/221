#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    int balance;
} account_t;

account_t acc;
pthread_mutex_t acc_lock;   

void *deposit(void *arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&acc_lock);    
        acc.balance = acc.balance + 1;
        pthread_mutex_unlock(&acc_lock);  
    }
    return NULL;
}

void *withdraw(void *arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&acc_lock);    
        acc.balance = acc.balance - 1;
        pthread_mutex_unlock(&acc_lock);  
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;

    acc.balance = 0;

    pthread_mutex_init(&acc_lock, NULL);   

    pthread_create(&t1, NULL, deposit, NULL);
    pthread_create(&t2, NULL, withdraw, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&acc_lock);     

    printf("Final balance = %d\n", acc.balance);
    return 0;
}

