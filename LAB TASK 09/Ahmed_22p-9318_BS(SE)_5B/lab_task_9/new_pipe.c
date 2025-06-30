#include <unistd.h>
#include <stdio.h>

int main()
{
    int pfd[2];
    pipe(pfd);
    printf("Read (pfd[0]): %d\n",pfd[0]);
    printf("(Write pfd[1]): %d\n",pfd[1]);
    return 0;
}
