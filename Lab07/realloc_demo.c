#include <stdio.h>
#include <stdlib.h>

static void print_arr(const char *msg, int *a, size_t n) {
    printf("%s (len=%zu):", msg, n);
    for (size_t i = 0; i < n; i++) printf(" %d", a[i]);
    putchar('\n');
}

int main(void) {
    size_t n = 5;
    int *a = (int*)malloc(n * sizeof(int));
    if (!a) { perror("malloc"); return 1; }
    for (size_t i = 0; i < n; i++) a[i] = (int)(i+1);
    print_arr("Initial", a, n);

    size_t new_n = 10;
    int *b = (int*)realloc(a, new_n * sizeof(int));
    if (!b) { perror("realloc (grow)"); free(a); return 1; }

    for (size_t i = n; i < new_n; i++) b[i] = (int)(100 + (int)i);
    print_arr("After grow", b, new_n);


    size_t shrink_n = 3;
    int *c = (int*)realloc(b, shrink_n * sizeof(int));
    if (!c) { perror("realloc (shrink)"); free(b); return 1; }
    print_arr("After shrink", c, shrink_n);

    free(c);
    return 0;
}
