#define _POSIX_C_SOURCE 200809L
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input-file>\n", argv[0]);
        return 1;
    }
    const char *path = argv[1];
    int fd = open(path, O_RDONLY);
    if (fd < 0) { perror("open"); return 1; }

    struct stat st;
    if (fstat(fd, &st) == -1) { perror("fstat"); close(fd); return 1; }
    if (st.st_size == 0) {
        fprintf(stderr, "Error: file is empty.\n");
        close(fd);
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        close(fd);
        return 1;
    } else if (pid == 0) {
        size_t len = st.st_size;
        void *addr = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
        if (addr == MAP_FAILED) { perror("mmap"); _exit(1); }
        ssize_t written = 0;
        ssize_t n = write(STDOUT_FILENO, addr, len);
        if (n < 0) { perror("write"); munmap(addr, len); _exit(1); }
        written += n;
        munmap(addr, len);
        _exit(0);
    } else {
        int status = 0;
        waitpid(pid, &status, 0);
        write(STDOUT_FILENO, "\n", 1);
        close(fd);
        return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
    }
}
