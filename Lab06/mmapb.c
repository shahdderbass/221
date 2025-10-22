#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    int N = 5;
    size_t size = N * sizeof(int);

    int fd = open("data.bin", O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    if (ftruncate(fd, size) < 0) {
        perror("ftruncate");
        close(fd);
        exit(1);
    }

    int *arr = mmap(
        NULL,        // Fill here
        size,        // Fill here
        PROT_READ | PROT_WRITE,
        MAP_SHARED,  // Fill here
        fd,
        0            // Fill here
    );

    if (arr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        exit(1);
    }

    for (int i = 0; i < N; i++)
        arr[i] = i + 1;

    pid_t pid = fork();

    if (pid == 0) {
        // TODO: Child multiplies each element by 10
    	for(int i = 0; i < N; i++){
		arr[i] *= 10;
	}
    } else if (pid > 0) {
        // TODO: Parent waits, then prints updated values
    	wait(0);
	for(int i = 0; i < N; i++){
		printf("arr[%d] = %d\n", i+1, arr[i]);
	}
    }

    munmap(arr, size);
    close(fd);
    return 0;
}

