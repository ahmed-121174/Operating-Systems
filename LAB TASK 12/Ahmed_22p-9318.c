#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#define MAX_SIZE 100

int fib_array[MAX_SIZE];
pthread_mutex_t mutex; // Mutex to synchronize access to the shared array

int fib(int n)
{
    if (n <= 0)
        return 0;
    else if (n == 1)
        return 1;

    int a = 0, b = 1, temp;
    for (int i = 2; i <= n; i++)
    {
        temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

// Calculate Fibonacci number at index n and store it in the array
void *calculate_fib(void *arg)
{
    int index = *((int *)arg);
    free(arg); // Free dynamically allocated memory

    // Calculate Fibonacci number
    int result = fib(index);

    // Lock mutex to safely store the result in the shared array
    pthread_mutex_lock(&mutex);
    fib_array[index] = result;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main()
{
    int n;
    printf("Enter a number to calculate (up to %d): ", MAX_SIZE);
    scanf("%d", &n);

    // Input check
    if (n <= 0 || n > MAX_SIZE)
    {
        printf("Invalid input. Please enter a number between 1 and %d.\n", MAX_SIZE);
        return 1;
    }

    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);

    // Create an array of thread IDs
    pthread_t threads[n];

    // Create threads to calculate Fibonacci numbers
    for (int i = 0; i < n; i++)
    {
        int *index = malloc(sizeof(int)); // Dynamically allocate memory for index
        *index = i;
        if (pthread_create(&threads[i], NULL, calculate_fib, (void *)index) != 0)
        {
            printf("Failed to create thread\n");
            return 1;
        }
    }

    // Wait for all threads to finish execution
    for (int i = 0; i < n; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Print the sequence up to n terms
    printf("Fibonacci sequence up to %d terms:\n", n);
    for (int i = 0; i < n; i++)
    {
        printf("%d ", fib_array[i]);
    }
    printf("\n");

    // Calculate and display the sum of the last two indices
    int last_index = n - 1;
    int sum_last_two = fib_array[last_index - 1] + fib_array[last_index];
    printf("The sum of the last two Fibonacci numbers (%d + %d) is: %d\n", 
           fib_array[last_index - 1], fib_array[last_index], sum_last_two);

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
