i#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    int N = 5;
    size_t size = N * sizeof(int);

    // Create a shared memory region backed by a file
    int fd = open("data.bin", O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    // Set file size
    if (ftruncate(fd, size) < 0) {
        perror("ftruncate");
        close(fd);
        exit(1);
    }

    // Map the file into memory
    int *arr = mmap(
        NULL, size, PROT_READ | PROT_WRITE,
        MAP_SHARED, fd, 0
    );

    if (arr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        exit(1);
    }

    // Initialize array and display
    for (int i = 0; i < N; i++) {
        arr[i] = i + 1;
        printf("Parent wrote: %d\n", arr[i]);
    }

    munmap(arr, size);
    close(fd);
    return 0;
}

