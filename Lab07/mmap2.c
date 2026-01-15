#define _POSIX_C_SOURCE 200809L
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <file> <message>\n", argv[0]);
        return 1;
    }
    int fd = open(argv[1], O_RDWR);
    if (fd < 0) { perror("open"); return 1; }

    struct stat st;
    if (fstat(fd, &st) == -1) { perror("fstat"); close(fd); return 1; }
    if (st.st_size == 0) { fprintf(stderr, "Empty file.\n"); close(fd); return 1; }

    size_t len = st.st_size;
    char *addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) { perror("mmap"); close(fd); return 1; }

    size_t msg_len = strlen(argv[2]);
    size_t to_copy = (msg_len < len) ? msg_len : (size_t)len;
    memcpy(addr, argv[2], to_copy);

    if (msync(addr, to_copy, MS_SYNC) == -1) perror("msync");
    munmap(addr, len);
    close(fd);
    return 0;
}
