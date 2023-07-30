#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int fd[2];
    fd_set set;
    struct timeval timeout;
    char buf[256];
    int nbytes;

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
        char *msg = "Hello, parent process!";
        write(fd[1], msg, strlen(msg));
        close(fd[1]);
        exit(0);
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
                nbytes = read(fd[0], buf, sizeof(buf));
                printf("Received message from child process: %s\n", buf);
            }
        }
        wait(NULL); // wait for child process to finish
        exit(0);
    }
}

