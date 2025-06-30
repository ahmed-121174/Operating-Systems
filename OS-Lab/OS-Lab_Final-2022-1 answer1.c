#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int pipe_fd[2]; // Pipe for communication between parent and child

// Signal handler for SIGINT in the child process
void handle_signal(int sig) {
    if (sig == SIGINT) {
        printf("[Child] Let me finish my work first.\n");
    }
}

int main() {
    int arr[10];
    srand(time(NULL)); // Seed for random number generation

    // Generate 10 random integers between 20 and 70 and store them in the array
    for (int i = 0; i < 10; i++) {
        arr[i] = rand() % 51 + 20; // Random number between 20 and 70
    }

    // Print the array (unsorted)
    printf("Array: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Create the pipe
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Child process
        close(pipe_fd[0]); // Close the read end of the pipe

        signal(SIGINT, handle_signal); // Set up signal handler for SIGINT

        // Calculate the mean of the array elements
        int sum = 0;
        for (int i = 0; i < 10; i++) {
            sum += arr[i];
            sleep(1); // Wait for 1 second at each index
        }
        int mean = sum / 10;

        // Send the mean to the parent process
        write(pipe_fd[1], &mean, sizeof(mean));
        close(pipe_fd[1]);

        exit(0);
    } else { // Parent process
        close(pipe_fd[1]); // Close the write end of the pipe

        // Wait for 5 seconds before sending SIGINT to the child
        sleep(5);
        kill(pid, SIGINT); // Send SIGINT to the child process

        // Wait for 7 seconds before reading the mean value from the child
        sleep(7);
        int mean;
        read(pipe_fd[0], &mean, sizeof(mean));
        printf("[Parent] Mean value received from child: %d\n", mean);

        close(pipe_fd[0]);

        // Wait for the child process to finish
        wait(NULL);
    }

    return 0;
}

