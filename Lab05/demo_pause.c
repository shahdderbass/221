#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

void on_sigint(int s){ /* empty handler wakes pause */ }

int main(void){
    signal(SIGINT, on_sigint);
    printf("Before pause at %ld\n", time(NULL));
    pause(); // waits until a signal is caught
    printf("\nAfter pause at %ld\n", time(NULL));
}

