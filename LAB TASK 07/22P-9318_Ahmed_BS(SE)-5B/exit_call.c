#include <stdio.h>
#include <stdlib.h> // Include for exit() function

void anotherExit(); // Function Prototype

int main()
{
    int num;
    printf("Enter a Number: ");
    scanf("%i", &num);

    if(num > 25)
    {
        printf("exit 1\n");
        exit(1); // Exit point 1
    }
    else
    {
        anotherExit(); // Call another exit function
    }

    return 0;
}

void anotherExit()
{
    printf("Exit 2\n");
    exit(2); //Exit point 2
}

