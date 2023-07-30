#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>

#define BUFFER_SIZE 256

int main() 
{
    int pipe_fd[2];
    char buffer[BUFFER_SIZE];
    int poll_ret, num_bytes;
    pid_t pid;

    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process write into pipe 
        printf("Enter text to send: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        num_bytes = strlen(buffer);

        if (write(pipe_fd[1], buffer, num_bytes) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        close(pipe_fd[1]);
        exit(EXIT_SUCCESS);
    } else {
        // Parent process read from pipe 
        struct pollfd poll_fds[1];
        poll_fds[0].fd = pipe_fd[0];
        poll_fds[0].events = POLLIN;

        // Using poll to wait for pipe
        poll_ret = poll(poll_fds, 1, -1);
        if (poll_ret == -1) {
            perror("poll");
            exit(EXIT_FAILURE);
        }

        if (poll_fds[0].revents & POLLIN) {
            // read from pipe
            num_bytes = read(pipe_fd[0], buffer, BUFFER_SIZE);
            printf("Received message: %s", buffer);
        }

        close(pipe_fd[0]);
        exit(EXIT_SUCCESS);
    }
}

