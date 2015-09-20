#include <stdio.h>
#include "mylib.h"
#include <errno.h>

bool IsSquareDevided (long unsigned n)
{
    for (long unsigned i = 2; i * i <= n; i ++)
        if (n % (i * i) == 0)
            return true;

     return false;
}

int main()
{
    COMMENT("Input a number to know if it has quadratic divisors:");
    long unsigned n = 0;
    INPUT(n);
    if (errno)
        perror("Problem");

    if (IsSquareDevided(n))
        COMMENT ("YES");
    else
        COMMENT ("NO");

    return 1;
}
