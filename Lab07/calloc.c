#include <stdio.h>
#include <stdlib.h>

int main(void) {
    size_t n = 8;
    int *a = (int*)calloc(n, sizeof(int));
    if (!a) { perror("calloc"); return 1; }

    printf("All elements should be zero-initialized:\n");
    for (size_t i = 0; i < n; i++) {
        printf("a[%zu] = %d\n", i, a[i]);
    }

    free(a);
    return 0;
}
