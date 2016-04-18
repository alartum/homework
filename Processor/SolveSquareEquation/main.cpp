#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "mylib.h"

const int INFINITE_ROOTS = -1;

// Solving quadratic equation. Get coefs a, b, c and x1, x2 for storing roots.
// Returns number of roots (INFINITE_ROOTS may also be returned).
int SolveSquareEquation (double a, double b, double c, double* x1, double* x2);

int main ()
{
    INIT_INFO ("SolveSquareEquation");

    COMMENT ("Enter a, b, c for ax^2 + bx + c = 0:");
    double a = 0, b = 0, c = 0;

    if (!INPUT (a) || !INPUT (b) || !INPUT(c))
        return 1;

    double x1 = 0, x2 = 0;
    int nRoots = SolveSquareEquation(a, b, c, &x1, &x2);

    switch (nRoots)
    {
    case 0:
        printf ("#No roots\n");
        break;
    case 1:
        printf ("#One root: x1 = %lg\n", x1);
        break;
    case 2:
        printf ("#Two roots: x1 = %lg, x2 = %lg\n", x1, x2);
        break;
    case INFINITE_ROOTS:
        printf ("#Infinite number of roots\n");
        break;
    }

    return 0;
}

int SolveSquareEquation (double a, double b, double c, double* x1, double* x2)
{
    assert (x1);
    assert (x2);

    if (!IS_ZERO(a))
    {
        if (IS_ZERO(b))
        {
            if (IS_ZERO(c))
            {
                *x1 = 0;

                return 1;
            }
            else
            {
                double sqrX = - c / a;
                if (sqrX > 0)
                {
                    *x1 = sqrt (sqrX);
                    *x2 = - *x1;

                    return 2;
                }
                else
                {
                    return 0;
                }
            }
        }
        else
        {
            if (IS_ZERO(c))
            {
                *x1 = 0;
                *x2 = - b / a;

                return 2;
            }
            else
            {
                double a2 = 2 * a;
                double D = b*b - 2*a2*c;

                if (IS_ZERO(D))
                {
                    *x1 = -b / a2;

                    return 1;
                }
                else if (D > 0)
                {
                    double sqrtD = sqrt (D);
                    *x1 = (-b + sqrtD) / a2;
                    *x2 = (-b - sqrtD) / a2;

                    return 2;
                }
                else
                {
                    return 0;
                }
            }
            *x1 = -b / (2 * a);

            return 1;
        }
    }
    else if (!IS_ZERO(b))
    {
        *x1 = - c/b;

        return 1;
    }
    else if (!IS_ZERO(c))
        return 0;
    else
        return INFINITE_ROOTS;
}
