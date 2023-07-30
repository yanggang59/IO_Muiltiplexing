#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 256

int main() {
    int fd[2];
    fd_set set;
    struct timeval timeout;
    char buffer[BUFFER_SIZE];
    int num_w_bytes, num_r_bytes; 

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        // child process
        close(fd[0]); // close unused read end
        printf("Enter text to send: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        num_w_bytes = strlen(buffer);

        if (write(fd[1], buffer, num_w_bytes) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        close(fd[1]);
        exit(EXIT_SUCCESS);
    } else {
        // parent process
        close(fd[1]); // close unused write end
        FD_ZERO(&set);
        FD_SET(fd[0], &set);
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        int ret = select(fd[0] + 1, &set, NULL, NULL, &timeout);
        if (ret == -1) {
            perror("select");
            exit(1);
        } else if (ret == 0) {
            printf("Timeout\n");
            exit(0);
        } else {
            if (FD_ISSET(fd[0], &set)) {
                num_r_bytes = read(fd[0], buffer, sizeof(buffer));
                printf("Received message from child process: %s\n", buffer);
            }
        }
        wait(NULL); // wait for child process to finish
        exit(0);
    }
}

