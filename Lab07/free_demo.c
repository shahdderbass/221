#include <stdio.h>
#include <stdlib.h>

int main(void) {
    size_t n = 4;
    int *a = (int*)malloc(n * sizeof(int));
    if (!a) { perror("malloc"); return 1; }
    for (size_t i = 0; i < n; i++) a[i] = (int)(10 + i);

    printf("Before free: a[0]=%d\n", a[0]);
    free(a);
    a = NULL; 

    a = (int*)malloc(n * sizeof(int));
    if (!a) { perror("malloc"); return 1; }
    for (size_t i = 0; i < n; i++) a[i] = (int)(20 + i);
    printf("After reallocation: a[0]=%d\n", a[0]);
    free(a);
    return 0;
}
