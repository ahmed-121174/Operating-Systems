#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>

int main() {
    pid_t pid;
    pid=fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    else if (pid == 0) {
        printf("Child process created. PID: %d\n", getpid());
        pause();
        printf("Child process exiting.\n");
        exit(0);
    }
    else {
        printf("Parent process. PID: %d\n", getpid());
        printf("Sending SIGTERM to child process with PID: %d\n", pid);
        if (kill(pid, SIGTERM) == 0) {
            printf("Signal sent successfully.\n");
        } else {
            printf("Failed to send signal");
        }
        printf("Parent waiting for 120 seconds.\n");
        sleep(120);
        wait(NULL);
        printf("Parent process exiting.\n");
    }

    return 0;
}

