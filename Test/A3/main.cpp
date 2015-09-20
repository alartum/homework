#undef DEBUG

#include <stdio.h>
#include "mylib.h"
#include <errno.h>

bool IsScalable (long unsigned n)
{
    assert (n > 0);
    if (n == 1)
        return true;

    long unsigned nDiv4 = n >> 2;
    long unsigned newN = nDiv4 << 2;
    unsigned modulo = n - newN;

    DPRINT(n);
    DPRINT(nDiv4);
    DPRINT(newN);
    DPRINT(modulo);
    if (modulo == 2)
        return false;
    if (modulo == 1 || modulo == 0)
        return IsScalable(nDiv4);
    else
        return IsScalable(nDiv4 + 1);
}

int main()
{
    COMMENT ("Input a natural number to know if it is scalable:");
    long unsigned n = 0;
    INPUT(n);
    if (errno)
        perror("Problem");

    if (IsScalable(n))
        printf ("YES\n");
    else
        printf ("NO\n");

    return 1;
}
