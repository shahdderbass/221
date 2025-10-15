#include <stdio.h>
#include <unistd.h>

int main(void){
    printf("spin PID=%d\n", getpid());
    while (1) { puts("spinning..."); sleep(1); }
}

