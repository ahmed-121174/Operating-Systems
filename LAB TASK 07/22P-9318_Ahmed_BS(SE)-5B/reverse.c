#include<unistd.h>
#include<stdio.h>
int main()
{
    for(int i = 10; i >= 0; i--)
    {
        printf("%d\n",i);
        sleep(1); //delay for 1 second
    }
    return 0;
}
