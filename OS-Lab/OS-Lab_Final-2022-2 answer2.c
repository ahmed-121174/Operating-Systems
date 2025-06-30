#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    printf("Parent Process (PID: %d)\n", getpid());

    // Create Child 1
    pid_t child1 = fork();

    if (child1 == 0) {
        // Inside Child 1
        printf("Child 1 (PID: %d, Parent PID: %d)\n", getpid(), getppid());

        // Create Child 4
        pid_t child4 = fork();
        if (child4 == 0) {
            // Inside Child 4
            printf("Child 4 (PID: %d, Parent PID: %d)\n", getpid(), getppid());
        } else {
            // Create Child 5
            pid_t child5 = fork();
            if (child5 == 0) {
                // Inside Child 5
                printf("Child 5 (PID: %d, Parent PID: %d)\n", getpid(), getppid());
            }
        }
    } else {
        // Create Child 2
        pid_t child2 = fork();

        if (child2 == 0) {
            // Inside Child 2
            printf("Child 2 (PID: %d, Parent PID: %d)\n", getpid(), getppid());
        } else {
            // Create Child 3
            pid_t child3 = fork();
            if (child3 == 0) {
                // Inside Child 3
                printf("Child 3 (PID: %d, Parent PID: %d)\n", getpid(), getppid());

                // Create Child 6
                pid_t child6 = fork();
                if (child6 == 0) {
                    // Inside Child 6
                    printf("Child 6 (PID: %d, Parent PID: %d)\n", getpid(), getppid());
                }
            }
        }
    }

    // Ensure all processes complete
    sleep(1);
    return 0;
}

