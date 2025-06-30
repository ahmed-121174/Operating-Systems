#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#define BUFFER_SIZE 100
int main()
{
	int fd1[2];
	int fd2[2];
	pid_t pid;
	char message[BUFFER_SIZE];
	char buffer[BUFFER_SIZE];
	if(pipe(fd1)==-1 || pipe(fd2)==-1)
	{
		printf("pipe failed");
		exit(1);
	}
	pid=fork();
	if(pid<0)
	{
		printf("fork failed");
		exit(1);
	}
	if(pid>0)
	{
		close(fd1[0]);
		close(fd2[1]);
		while (1) 
		{
		 	printf("P1: ");
			fgets(message, BUFFER_SIZE, stdin);
			message[strcspn(message, "\n")] = '\0';
			/*The strcspn() function searches for the first occurrence in a string of
			 any of the specified characters and returns the length of the string up
			 to that point*/
			write(fd1[1], message, strlen(message) + 1);
			read(fd2[0], buffer, BUFFER_SIZE);
			printf("P2 Says: %s\n", buffer);
		}
	}
	else
	{
		close(fd1[1]);
		close(fd2[0]);
		while (1)
		{
			read(fd1[0], buffer, BUFFER_SIZE);
			printf("P1 Says: %s\n", buffer);
			printf("P2: ");
			fgets(message, BUFFER_SIZE, stdin);
			message[strcspn(message, "\n")] = '\0';\
			write(fd2[1], message, strlen(message) + 1);
		}
	}
	return 0;
}
