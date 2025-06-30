#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include<unistd.h>
sem_t semaphore;

void* thread1_function(void* arg) {
    printf("Thread 1: Waiting for signal...\n");
    sem_wait(&semaphore); // Waits until sem_post is called
    printf("Thread 1: Received signal, proceeding...\n");
    return NULL;
}

void* thread2_function(void* arg) {
    sleep(1); // Simulate some work before signaling
    printf("Thread 2: Sending signal...\n");
    sem_post(&semaphore); // Signal to thread 1
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    sem_init(&semaphore, 0, 0); // Initial value of 0, so thread 1 waits
    pthread_create(&thread1, NULL, thread1_function, NULL);
    pthread_create(&thread2, NULL, thread2_function, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    sem_destroy(&semaphore); // Cleanup
    return 0;
}

