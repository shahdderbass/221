#define _POSIX_C_SOURCE 200809L
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file> [N]\n", argv[0]);
        return 1;
    }
    size_t N = (argc >= 3) ? strtoul(argv[2], NULL, 10) : 64;

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) { perror("open"); return 1; }
    struct stat st;
    if (fstat(fd, &st) == -1) { perror("fstat"); close(fd); return 1; }
    if (st.st_size == 0) { fprintf(stderr, "Empty file.\n"); close(fd); return 1; }

    size_t len = st.st_size;
    void *addr = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED) { perror("mmap"); close(fd); return 1; }

    size_t to_print = (N < (size_t)len) ? N : (size_t)len;
    write(STDOUT_FILENO, addr, to_print);
    write(STDOUT_FILENO, "\n", 1);

    munmap(addr, len);
    close(fd);
    return 0;
}
