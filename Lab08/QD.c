#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int fd_out = -1;

void handle_error(const char *msg) {
    fprintf(stderr, "ERROR: %s\n", msg);
    perror("System Error Details");

    if (fd_out != -1) {
        close(fd_out);
    }
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file_1> <output_file_2>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *file1_name = argv[1];
    const char *file2_name = argv[2];

    fd_out = open(file2_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out == -1) {
        handle_error("Failed to open or create shared output file (File 2)");
    }

    pid_t pid = fork();

    if (pid < 0) {
        handle_error("Fork failed");
    }

    if (pid == 0) {
        // CHILD PROCESS: Counts alphabets
        FILE *fp1 = fopen(file1_name, "r");
        if (fp1 == NULL) {
            handle_error("Child failed to open input file (File 1)");
        }

        int c;
        long alphabet_count = 0;
        while ((c = fgetc(fp1)) != EOF) {
            if (isalpha(c)) {
                alphabet_count++;
            }
        }
        
        if (ferror(fp1)) {
            fclose(fp1);
            handle_error("Child error reading from input file (File 1)");
        }
        fclose(fp1);

        char output_buffer[100];
        int len = snprintf(output_buffer, sizeof(output_buffer),
                           "Child PID %d counted %ld alphabets.\n", getpid(), alphabet_count);
        
        if (write(fd_out, output_buffer, len) == -1) {
            handle_error("Child failed to write to File 2");
        }

        exit(EXIT_SUCCESS);

    } else {
        // PARENT PROCESS: Counts digits
        FILE *fp1 = fopen(file1_name, "r");
        if (fp1 == NULL) {
            handle_error("Parent failed to open input file (File 1)");
        }

        int c;
        long digit_count = 0;
        while ((c = fgetc(fp1)) != EOF) {
            if (isdigit(c)) {
                digit_count++;
            }
        }
        
        if (ferror(fp1)) {
            fclose(fp1);
            handle_error("Parent error reading from input file (File 1)");
        }
        fclose(fp1);

        char output_buffer[100];
        int len = snprintf(output_buffer, sizeof(output_buffer),
                           "Parent PID %d counted %ld digits.\n", getpid(), digit_count);

        if (write(fd_out, output_buffer, len) == -1) {
            handle_error("Parent failed to write to File 2");
        }
        
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            handle_error("Parent failed to wait for child");
        }
        
        if (close(fd_out) == -1) {
            handle_error("Parent failed to close File 2");
        }

        printf("Successfully processed files. Results written to %s\n", file2_name);
        return EXIT_SUCCESS;
    }
}
