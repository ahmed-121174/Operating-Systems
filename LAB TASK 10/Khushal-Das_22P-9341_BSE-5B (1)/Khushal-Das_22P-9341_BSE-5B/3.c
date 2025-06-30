#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
int main() {
    pid_t pid;
    pid = fork();

     if (pid == 0) {
        printf("Child process created. PID: %d\n", getpid());
        
        if(kill(getppid(), SIGTERM)==0){
        printf("Child send terminate signal to parent process PID: %d\n",getppid());
        }
        else{
        printf("Failed to send terminate signal");
        }
        printf("Child process waiting for 120 seconds.\n");
        sleep(120);
        
    } else {
        
    }
}

