// **Question 1: Distributed Video Processing System**
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main() {
    int pipe1[2], pipe2[2], pipe3[2];
    pipe(pipe1); // PV1 -> PV2
    pipe(pipe2); // PV2 -> PV3
    pipe(pipe3); // PV3 -> PV4

    if (fork() == 0) { // PV1
        close(pipe1[0]);
        char files[3][20] = {"video1.mp4", "video2.mp4", "video3.mp4"};
        for (int i = 0; i < 3; i++) {
            write(pipe1[1], files[i], strlen(files[i]) + 1);
        }
        close(pipe1[1]);
    } else if (fork() == 0) { // PV2
        close(pipe1[1]);
        close(pipe2[0]);
        char file[20];
        while (read(pipe1[0], file, sizeof(file)) > 0) {
            strcat(file, "_transcoded");
            write(pipe2[1], file, strlen(file) + 1);
        }
        close(pipe1[0]);
        close(pipe2[1]);
    } else if (fork() == 0) { // PV3
        close(pipe2[1]);
        close(pipe3[0]);
        char file[40];
        while (read(pipe2[0], file, sizeof(file)) > 0) {
            strcat(file, "_QC_Passed");
            write(pipe3[1], file, strlen(file) + 1);
        }
        close(pipe2[0]);
        close(pipe3[1]);
    } else { // PV4
        close(pipe3[1]);
        char file[60];
        while (read(pipe3[0], file, sizeof(file)) > 0) {
            printf("Distributed: %s\n", file);
        }
        close(pipe3[0]);
    }
    return 0;
}

// **Question 2: Embedded System for Telehealth Monitoring**
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t lock;

void* monitor(void* arg) {
    char* device = (char*)arg;
    for (int i = 0; i < 3; i++) {
        sem_wait(&lock);
        printf("%s: Monitoring...\n", device);
        sem_post(&lock);
        sleep(1); // Simulate monitoring interval
    }
    return NULL;
}

int main() {
    pthread_t threads[5];
    char* devices[] = {"Heart Rate", "Oxygen", "Blood Pressure", "Glucose", "Respiratory"};

    sem_init(&lock, 0, 1);

    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, monitor, devices[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&lock);
    return 0;
}

// **Question 3: Shell Script for File Management**
#include <stdio.h>
int main() {
    printf("#!/bin/bash\n");
    printf("echo \"Enter directory name:\"\n");
    printf("read dir\n");
    printf("if [ ! -d \"$dir\" ]; then mkdir $dir; fi\n");
    printf("mkdir -p $dir/Images $dir/Documents $dir/Others\n");
    printf("echo \"Enter number of files to generate:\"\n");
    printf("read count\n");
    printf("for i in $(seq 1 $count); do touch $dir/Images/file$i.jpg; done\n");
    printf("echo \"Summary generated.\"\n");
}

// **Question 4: Process Tree with fork()**
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Parent PID: %d\n", getpid());
    if (fork() == 0) {
        printf("Child1 PID: %d\n", getpid());
    } else if (fork() == 0) {
        printf("Child2 PID: %d\n", getpid());
    } else {
        printf("Parent finished\n");
    }
    return 0;
}

