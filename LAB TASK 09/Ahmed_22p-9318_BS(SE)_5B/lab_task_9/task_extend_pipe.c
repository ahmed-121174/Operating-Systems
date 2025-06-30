#include<unistd.h>
#include<stdio.h>
int main()
{
    int pid; // for storing fork() return
    int pfd[2]; // for pipe file descriptors
    char aString[20]; // Temporary storage
    pipe(pfd); // create our pipe
    pid = fork(); // create child process
    if (pid == 0)
    { //child
        write(pfd[1], "Hello",5); // Write onto pipe
        close(pfd[1]); //closing write pipe in the child process
    }
    else
    { //parent
        printf("Before read, aString contains: '%s'\n",aString);

        // read from the pipe
        read(pfd[0], aString, 5); // read from pipe into aString
        aString[5] ='\0';

        // After reading, print the contents of aString (it should be "Hello")
        printf("After read, aString contains: '%s'\n", aString);

        close(pfd[0]); // close read end of the pipe in the parent process
    }
    return 0;
}
