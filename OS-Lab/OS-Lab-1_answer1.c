#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>

#define MAX_PROCESSES 4
#define MAX_RESOURCES 4

// Matrix and vector declarations
int allocation[MAX_PROCESSES][MAX_RESOURCES];
int max[MAX_PROCESSES][MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];
int available[MAX_RESOURCES];

// Semaphore array for process synchronization
sem_t sem[MAX_PROCESSES];

// Function to read matrices and vectors from a file
void readFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file\n");
        exit(1);
    }

    // Read Allocation Matrix
    printf("Enter Allocation Matrix:\n");
    for (int i = 0; i < MAX_PROCESSES; i++) {
        for (int j = 0; j < MAX_RESOURCES; j++) {
            fscanf(file, "%d", &allocation[i][j]);
            printf("%d ", allocation[i][j]);
        }
        printf("\n");
    }

    // Read Max Matrix
    printf("Enter Max Matrix:\n");
    for (int i = 0; i < MAX_PROCESSES; i++) {
        for (int j = 0; j < MAX_RESOURCES; j++) {
            fscanf(file, "%d", &max[i][j]);
            printf("%d ", max[i][j]);
        }
        printf("\n");
    }

    // Read Available Resources
    printf("Enter Available Resources:\n");
    for (int i = 0; i < MAX_RESOURCES; i++) {
        fscanf(file, "%d", &available[i]);
        printf("%d ", available[i]);
    }
    printf("\n");

    fclose(file);
}

// Function to calculate Need Matrix
void calculateNeed() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        for (int j = 0; j < MAX_RESOURCES; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}

// Banker's Algorithm to detect a safe sequence
bool isSafe(int safeSequence[]) {
    int work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES] = { false };

    // Initialize work vector to available resources
    for (int i = 0; i < MAX_RESOURCES; i++) {
        work[i] = available[i];
    }

    int count = 0;
    while (count < MAX_PROCESSES) {
        bool found = false;

        for (int i = 0; i < MAX_PROCESSES; i++) {
            if (!finish[i]) {
                // Check if the process's need can be satisfied with available resources
                bool canExecute = true;
                for (int j = 0; j < MAX_RESOURCES; j++) {
                    if (need[i][j] > work[j]) {
                        canExecute = false;
                        break;
                    }
                }

                if (canExecute) {
                    // Assume process can be executed
                    for (int j = 0; j < MAX_RESOURCES; j++) {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    safeSequence[count++] = i;
                    found = true;
                    break;
                }
            }
        }

        // If no process was found, the system is not in a safe state
        if (!found) {
            return false;
        }
    }
    return true;
}

// Simulate process execution
void *processFunction(void *arg) {
    int processID = *(int *)arg;

    sem_wait(&sem[processID]); // Wait for semaphore signal
    printf("Hello! Process %d is executed\n", processID);
    sem_post(&sem[processID]); // Signal to release semaphore

    return NULL;
}

// Function to simulate a random request and check safe sequence
void randomRequest() {
    srand(time(0));

    int processID = rand() % MAX_PROCESSES;
    int request[MAX_RESOURCES];

    // Generate a random request
    for (int i = 0; i < MAX_RESOURCES; i++) {
        request[i] = rand() % (need[processID][i] + 1); // Can't request more than need
    }

    // Check if request can be granted
    bool canGrant = true;
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (request[i] > available[i]) {
            canGrant = false;
            break;
        }
    }

    if (canGrant) {
        // Pretend to allocate resources
        for (int i = 0; i < MAX_RESOURCES; i++) {
            available[i] -= request[i];
            allocation[processID][i] += request[i];
            need[processID][i] -= request[i];
        }

        // Check for safe sequence
        int safeSequence[MAX_PROCESSES];
        if (isSafe(safeSequence)) {
            printf("Safe sequence detected: ");
            for (int i = 0; i < MAX_PROCESSES; i++) {
                printf("P%d ", safeSequence[i]);
            }
            printf("\n");

            // Create threads and synchronize using semaphores
            pthread_t threads[MAX_PROCESSES];
            for (int i = 0; i < MAX_PROCESSES; i++) {
                sem_init(&sem[i], 0, 1);
            }

            for (int i = 0; i < MAX_PROCESSES; i++) {
                pthread_create(&threads[i], NULL, processFunction, (void *)&safeSequence[i]);
            }

            for (int i = 0; i < MAX_PROCESSES; i++) {
                pthread_join(threads[i], NULL);
            }
        } else {
            printf("No safe sequence found. Request rejected.\n");
        }

        // Revert allocation for next iteration
        for (int i = 0; i < MAX_RESOURCES; i++) {
            available[i] += request[i];
            allocation[processID][i] -= request[i];
            need[processID][i] += request[i];
        }
    } else {
        printf("Request cannot be granted immediately. Request exceeds available resources.\n");
    }
}

int main() {
    readFile("file.txt");
    calculateNeed();

    // Simulate random requests until a safe sequence is found
    while (true) {
        randomRequest();
        sleep(1); // Optional delay for simulation
    }

    return 0;
}
Explanation
Reading Input: The readFile() function reads the matrices and available resources from file.txt.
Calculating Need: The calculateNeed() function calculates the Need matrix.
Banker's Algorithm: The isSafe() function checks if the system is in a safe state and generates a safe sequence if it exists.
Thread Creation: Semaphores (sem_t) and pthread are used for process synchronization.
Random Requests: The randomRequest() function simulates random process requests.
Compilation and Execution
Compile the code:

bash
Copy code
gcc -o banker's_algorithm banker's_algorithm.c -lpthread
Run the executable:

bash
Copy code
./banker's_algorithm
