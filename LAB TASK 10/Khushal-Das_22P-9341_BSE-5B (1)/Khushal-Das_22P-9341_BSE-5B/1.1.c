#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
int main() {
printf("Name: Khushal Das\n");
printf("Roll No: 22P-9341\n");
printf("Section: BSE5B\n");
int a=5;
int b=6;
printf("Sum =%d",a+b);
printf("\nThis process is going to kill!!\n");
kill(getpid( ), SIGTERM);
}
