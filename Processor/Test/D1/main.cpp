#define DEBUG

#include <stdio.h>
#include "mylib.h"
#include <errno.h>

int main()
{
    COMMENT ("Input a natural number to calculate the amount of good words:");
    long unsigned n = 0;
    INPUT(n);

    if (errno)
        perror("Problem");
    for (long unsigned i = 0; i <= n; i ++)
    {
        for (long unsigned j = 0; 2*j <= i + 1; j ++)
        {
            long unsigned currentNumber = Cnk (i - j + 1, j);

            if (currentNumber > 0)
                printf ("%lu ", currentNumber);
            else
                break;
        }
        printf ("\n");
    }

    return 0;
}
