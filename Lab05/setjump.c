#include <setjmp.h>
#include <stdio.h>

jmp_buf env;
int main(void){
    int r = setjmp(env);
    static int tries = 0;
    if (r == 0 && tries == 0){
        tries++;
        puts("first pass, jumping...");
        longjmp(env, 123);
    } else {
        printf("resumed with r=%d, tries=%d\n", r, tries);
    }
}

