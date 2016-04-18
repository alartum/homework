#define DEBUG

#include <stdio.h>
#include "mylib.h"
#include "dynamiclib.h"
#include <errno.h>


int main()
{
    INIT_INFO ("TransposeMatrix");
    long unsigned n = 0;
    INPUT (n);
    if (n <= 0)
        return errno = EIO, 1;
    long unsigned* matrix = InputDynamic (n, n);
    if (!matrix)
        perror ("Problem:");

    TransposeMatrix (matrix, n, n);
    COMMENT("Transposed matrix:");
    PrintMatrix (matrix, n, n);

    free (matrix);
    return 0;
}
