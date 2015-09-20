#ifndef MYLIB_H_INCLUDED
#define MYLIB_H_INCLUDED
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <errno.h>
#include <assert.h>

#define TYPE long unsigned
#define SPEC "%lu"
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
// Reading double number by its name. isOK = true if success, = false in other case
bool DoInput (TYPE *var, const char name[]);
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

    return false;
}

bool IsPrime (long unsigned n)
{
    for (long unsigned i = 2; i * i <= n; i ++)
        if (n % i == 0)
            return false;

    return true;
}
#endif // MYLIB_H_INCLUDED
