 #include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main(void){
    pid_t c = fork();
    if (c==0){ while (1){ puts("child alive"); sleep(1);} }
    else { while (1){ puts("parent alive"); sleep(2);} }
}

