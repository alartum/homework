#include <stdio.h>
#include "mylib.h"
#include <errno.h>

int main()
{
    COMMENT ("Input the amount of numbers to test:");
    long unsigned n = 0;
    INPUT (n);
    COMMENT ("Input the numbers:");
    if (errno)
        perror("Problem");

    COMMENT ("Now, input the numbers:");
    for (; n > 0; n --)
    {
        long unsigned a = 0;
        INPUT(a);
        if (IsPrime (a))
            printf ("%lu  ", a);
    }

    return 0;
}
