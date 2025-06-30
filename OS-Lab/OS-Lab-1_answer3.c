#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>

#define MATRIX_SIZE 6
#define SHM_KEY 1234  // Key for shared memory

// Semaphore structure for synchronization
struct sembuf sem_op;

// Matrix multiplication function
void multiplyMatrixRow(int matrix[MATRIX_SIZE][MATRIX_SIZE], int row, int factor) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        matrix[row][i] *= factor;
    }
}

// Signal handler for SIGINT (CTRL+C)
void handleSigInt(int sig) {
    printf("\nCTRL+C detected. Processing matrix...\n");
    // Create a pipe for communication between processes
    int pipe_fd[2];
    pipe(pipe_fd);

    // Initialize the matrix
    int matrix[MATRIX_SIZE][MATRIX_SIZE] = {
        {1, 2, 3, 4, 5, 6},
        {7, 8, 9, 10, 11, 12},
        {13, 14, 15, 16, 17, 18},
        {19, 20, 21, 22, 23, 24},
        {25, 26, 27, 28, 29, 30},
        {31, 32, 33, 34, 35, 36}
    };

    // Multiply the first two rows by 2
    multiplyMatrixRow(matrix, 0, 2);
    multiplyMatrixRow(matrix, 1, 2);

    // Fork the first child process
    pid_t pid1 = fork();
    if (pid1 == 0) {
        // Child process 1: Receives the matrix through the pipe
        close(pipe_fd[1]);  // Close write end
        int received_matrix[MATRIX_SIZE][MATRIX_SIZE];
        read(pipe_fd[0], received_matrix, sizeof(received_matrix));
        close(pipe_fd[0]);

        // Multiply the 3rd and 4th rows by 3
        multiplyMatrixRow(received_matrix, 2, 3);
        multiplyMatrixRow(received_matrix, 3, 3);

        // Create shared memory segment
        int shm_id = shmget(SHM_KEY, sizeof(received_matrix), IPC_CREAT | 0666);
        int *shm_ptr = (int *)shmat(shm_id, NULL, 0);

        // Copy the modified matrix to shared memory
        memcpy(shm_ptr, received_matrix, sizeof(received_matrix));

        // Fork the second child process
        pid_t pid2 = fork();
        if (pid2 == 0) {
            // Child process 2: Reads from shared memory
            int *shm_ptr2 = (int *)shmat(shm_id, NULL, 0);

            // Multiply the 5th and 6th rows by 4
            multiplyMatrixRow(shm_ptr2, 4, 4);
            multiplyMatrixRow(shm_ptr2, 5, 4);

            // Print the final matrix in reverse order
            printf("Final Matrix:\n");
            for (int i = MATRIX_SIZE - 1; i >= 0; i--) {
                for (int j = 0; j < MATRIX_SIZE; j++) {
                    printf("%d ", shm_ptr2[i * MATRIX_SIZE + j]);
                }
                printf("\n");
            }

            // Detach and remove shared memory
            shmdt(shm_ptr2);
            shmctl(shm_id, IPC_RMID, NULL);

            exit(0);
        } else {
            // Parent process (child 1) detaches shared memory
            shmdt(shm_ptr);

            exit(0);
        }
    } else {
        // Parent process (main program)
        close(pipe_fd[0]);  // Close read end
        write(pipe_fd[1], matrix, sizeof(matrix));
        close(pipe_fd[1]);

        // Wait for child processes to finish
        wait(NULL);
        wait(NULL);
    }
}

int main() {
    // Set up signal handler for CTRL+C
    signal(SIGINT, handleSigInt);

    // Run an infinite loop to keep the program running
    while (1) {
        printf("Press CTRL+C to trigger the matrix processing...\n");
        sleep(1);
    }

    return 0;
}
Shell Script to Compile and Execute
Create a shell script (run_matrix.sh) to compile and execute the program and save the output to Desktop/Myname/directory/file.txt:

bash
Copy code
#!/bin/bash

# Compile the program
gcc -o matrix_program matrix_program.c -lrt

# Run the program and save the output to the specified file
./matrix_program > ~/Desktop/Myname/directory/file.txt
Explanation of the Program
Signal Handling:

The signal() function sets up the handleSigInt() function to run when CTRL+C (SIGINT) is detected.
Pipe IPC:

Used to pass the matrix from the main process to the first child process.
Shared Memory IPC:

Used for communication between the first and second child processes.
Synchronization:

The synchronization is achieved through the inherent process synchronization provided by the pipe() and shmget()/shmat() functions.
Execution Instructions
Make the shell script executable:

bash
Copy code
chmod +x run_matrix.sh
Run the script:

bash
Copy code
./run_matrix.sh
Expected Output
The program waits until CTRL+C is pressed.
Once CTRL+C is detected, the program processes the matrix in steps and outputs the final matrix in a backward sequence to the specified file (Desktop/Myname/directory/file.txt).
