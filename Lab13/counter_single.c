#include <stdio.h>

#define N 1000000

int main(void) {
    long counter = 0;
    for (int i = 0; i < N; i++) {
        counter++;
    }
    printf("Final counter value = %ld\n", counter);
    return 0;
}

