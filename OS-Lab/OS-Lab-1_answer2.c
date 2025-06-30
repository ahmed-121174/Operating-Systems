#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 100  // Assuming a maximum size for the matrix
int matrix[MAX_SIZE][MAX_SIZE];
int result[MAX_SIZE * 2]; // Resultant array with twice the size of the number of rows

pthread_mutex_t mutex;  // Mutex for synchronizing thread access to the result array

// Structure to pass data to the threads
typedef struct {
    int row;
    int startIndex;
    int step;
    int *sum;
} ThreadData;

// Function to read the matrix from file
void readMatrixFromFile(const char *filename, int *size) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file\n");
        exit(1);
    }

    fscanf(file, "%d", size);  // Read the size of the matrix
    for (int i = 0; i < *size; i++) {
        for (int j = 0; j < *size; j++) {
            fscanf(file, "%d", &matrix[i][j]);
        }
    }

    fclose(file);
}

// Function for the thread to sum even-indexed values
void *sumEvenIndexValues(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int sum = 0;
    for (int i = data->startIndex; i < MAX_SIZE; i += data->step) {
        sum += matrix[data->row][i];
    }
    
    pthread_mutex_lock(&mutex);
    result[data->row * 2] = sum;
    pthread_mutex_unlock(&mutex);
    
    printf("Thread for even indices in row %d completed with sum %d\n", data->row, sum);
    return NULL;
}

// Function for the thread to sum odd-indexed values
void *sumOddIndexValues(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int sum = 0;
    for (int i = data->startIndex; i < MAX_SIZE; i += data->step) {
        sum += matrix[data->row][i];
    }

    pthread_mutex_lock(&mutex);
    result[data->row * 2 + 1] = sum;
    pthread_mutex_unlock(&mutex);

    printf("Thread for odd indices in row %d completed with sum %d\n", data->row, sum);
    return NULL;
}

// Function for summing the resultant array
void *sumResultantArray(void *arg) {
    int totalSum = 0;
    int size = *((int *)arg);
    for (int i = 0; i < size; i++) {
        totalSum += result[i];
    }

    printf("Total sum of resultant array: %d\n", totalSum);
    return NULL;
}

int main() {
    int size;
    readMatrixFromFile("file.txt", &size);
    
    pthread_t threads[size * 2];  // Two threads per row (even and odd index threads)
    pthread_t sumThread;         // Thread to sum the resultant array

    pthread_mutex_init(&mutex, NULL);

    // Create threads for each row
    for (int i = 0; i < size; i++) {
        ThreadData evenData = {i, 0, 2, NULL}; // Even index thread data
        ThreadData oddData = {i, 1, 2, NULL};  // Odd index thread data

        pthread_create(&threads[i * 2], NULL, sumEvenIndexValues, (void *)&evenData);
        pthread_create(&threads[i * 2 + 1], NULL, sumOddIndexValues, (void *)&oddData);
    }

    // Wait for all row threads to complete
    for (int i = 0; i < size * 2; i++) {
        pthread_join(threads[i], NULL);
    }

    // Create and start the thread for summing the resultant array
    pthread_create(&sumThread, NULL, sumResultantArray, (void *)&size);
    pthread_join(sumThread, NULL);

    pthread_mutex_destroy(&mutex);

    return 0;
}
Explanation
Matrix Reading: readMatrixFromFile() reads the matrix from file.txt.
Thread Creation:
Two threads are created for each row:
sumEvenIndexValues() sums even-indexed values and places them in result[i * 2].
sumOddIndexValues() sums odd-indexed values and places them in result[i * 2 + 1].
Synchronization:
A mutex (pthread_mutex_t) ensures thread-safe writes to the result[] array.
Summing the Resultant Array:
The sumResultantArray() function is called in a separate thread to sum the result[] array.
Compilation and Execution
Compile the code:

bash
Copy code
gcc -o matrix_sum matrix_sum.c -lpthread
Run the executable:

bash
Copy code
./matrix_sum
Input File Format (file.txt)
Ensure that file.txt is formatted as follows:

mathematica
Copy code
4  // Size of the matrix (N x N)
1 2 3 4
5 6 7 8
9 10 11 12
13 14 15 16
Output
The program will output the sum of even and odd indexed elements for each row and the total sum of the result[] array.
