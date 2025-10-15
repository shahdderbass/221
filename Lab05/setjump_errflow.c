#include <stdio.h>
#include <setjmp.h>

jmp_buf env;

int main() {
    int items[] = {1,2,3,4,5};
    int i = 0;

    if (setjmp(env) != 0) {
        printf("Error on item %d, skipping\n", items[i]);
        i++;
    }

    for (; i < 5; i++) {
        printf("Parsing item %d\n", items[i]);
        if (items[i] == 2 || items[i] == 4)
            longjmp(env, 1);
        else
            printf("Item %d is safe\n", items[i]);
    }

    printf("Parsing complete.\n");
    return 0;
}

