#include <stdio.h>
#include <stdlib.h>

int main(void) {
    size_t n = 8;
    int *a = (int*)malloc(n * sizeof(int));
    if (!a) { perror("malloc"); return 1; }


    for (size_t i = 0; i < n/2; i++) a[i] = (int)(100 + i);

    printf("Reading %zu ints (first half initialized):\n", n);
    for (size_t i = 0; i < n; i++) {
        printf("a[%zu] = %d\n", i, a[i]);  
    }

    free(a);
    return 0;
}
