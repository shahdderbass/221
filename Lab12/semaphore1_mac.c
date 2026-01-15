#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>   // For O_CREAT
#include <sys/stat.h>

sem_t *semaphore;   // Named semaphore (pointer)

void *worker(void *arg) {
    int id = (int)(long)arg;

    printf("Thread %d: waiting to enter critical section\n", id);
    sem_wait(semaphore);   // down / P

    printf("Thread %d: inside critical section\n", id);
    sleep(1);
    printf("Thread %d: leaving critical section\n", id);

    sem_post(semaphore);   // up / V
    return NULL;
}

int main(void) {
    pthread_t t1, t2, t3;

    // ✅ Create a NAMED semaphore (works on macOS)
    semaphore = sem_open("/mysem", O_CREAT, 0644, 2);
    if (semaphore == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    pthread_create(&t1, NULL, worker, (void *)0);
    pthread_create(&t2, NULL, worker, (void *)1);
    pthread_create(&t3, NULL, worker, (void *)2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    sem_close(semaphore);
    sem_unlink("/mysem");   // ✅ cleanup
    return 0;
}

