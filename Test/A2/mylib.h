#ifndef MYLIB_H_INCLUDED
#define MYLIB_H_INCLUDED
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <errno.h>
#include <assert.h>

// Type of operations
#define TYPE long unsigned
#define SPEC "%lu"

#define MY_NAME "Alartum"
#define INIT_INFO(x) ( printf ("#---" x " by " MY_NAME "---\n") )
#define IS_ZERO(x) ( fabs (x) < DBL_EPSILON )
// Macro for more comfortable input
#define INPUT(x) ( DoInput (&(x), #x) )
#define COMMENT(x) ( printf ("#" x "\n") )

#ifdef DEBUG
#define DPRINT(x) ( PrintDebug ((x), #x) )
#else
#define DPRINT(x) ;
#endif // DEBUG

const int TRY_AMOUNT = 3;
// Reading number of type TYPE by its name. True returned if success, false - otherwise.
bool DoInput (TYPE *var, const char name[]);
long unsigned Cnk (long unsigned n, long unsigned k);
int PrintDebug (TYPE var, const char name[]);
bool IsPrime (long unsigned n);
// DownFactorial calculates F = n*(n-1)*(n-2)*...*(n-down+1). So, (n - down) is not included.
long unsigned DownFactorial (long unsigned n, long unsigned down);
long unsigned Cnk (long unsigned n, long unsigned k);

int PrintDebug (TYPE var, const char name[])
{
    printf ("(?)%s = " SPEC "\n", name, var);

    return 0;
}

bool DoInput (TYPE *var, const char name[])
{
    assert (var);
    printf ("#%s>", name);
    if (scanf (SPEC, var) == 1)
        return true;
    else
    {
        printf ("#Invalid input\n");
        errno = EIO;
    }
    for (int i = 0; i < TRY_AMOUNT - 1; ++i)
    {
        printf ("#%s>", name);
        if (scanf ("%*s" SPEC, var) == 1)
            return true;
        else
        {
            printf ("#Invalid input\n");
            errno = EIO;
        }
    }

    return perror ("#Input error"), false;
}

bool IsPrime (long unsigned n)
{
    for (long unsigned i = 2; i * i <= n; i ++)
        if (n % i == 0)
            return false;

    return true;
}

// DownFactorial calculates F = n*(n-1)*(n-2)*...*(n-down+1). So, (n - down) is not included.
long unsigned DownFactorial (long unsigned n, long unsigned down)
{
    if (n == 0)
        return 1;

    long unsigned result = n;
    for (long unsigned i = 1; i < down; i ++)
    {
        result *= n - i;
        // Result must not be too big
        assert (result > 0);
    }
    return result;
}

long unsigned Cnk (long unsigned n, long unsigned k)
{
    assert (n >= 0);
    assert (k >= 0);
    assert (k <= n);

    if (k == 1 || k == n - 1)
        return n;
    else if (k == 0 || k == n)
        return 1;

    long unsigned difference = n - k;

    return DownFactorial(n, k - 1) / DownFactorial(difference, difference);
}
#endif // MYLIB_H_INCLUDED
