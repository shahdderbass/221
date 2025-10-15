#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

volatile sig_atomic_t count = 0;

void on_int(int sig){
    (void)sig;
    write(STDOUT_FILENO, "inside the handler function\n", 28);
}

int main(void){
    struct sigaction sa = {0};
    sa.sa_handler = on_int;
    sigaction(SIGINT, &sa, NULL); // Ctrl-C
    int i=1;
    while (1){
        printf("%d : inside the main function\n", i++);
        usleep(400000);
    }
    return 0;
}

