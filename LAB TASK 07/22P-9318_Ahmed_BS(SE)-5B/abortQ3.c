#include <stdio.h>
#include <stdlib.h>

void f3()
{
    abort();
}

int main()
{
    atexit(f3); // register f3 to run when the program exits
    exit(0); // Exit
}

