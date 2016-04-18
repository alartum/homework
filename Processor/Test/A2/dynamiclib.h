#ifndef DYNAMICLIB_H_INCLUDED
#define DYNAMICLIB_H_INCLUDED

#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#define POS(ROW, COLUMN, WIDTH) ( ROW * WIDTH + COLUMN )

long unsigned* InputDynamic (unsigned height, unsigned width)
{
    assert (height > 0 && width > 0);

    long unsigned* matrix = (long unsigned*)calloc (height*width, sizeof(*matrix));
    if (!matrix)
    {
        errno = ENOMEM;
        return NULL;
    }
    for (unsigned i = 0; i < height * width; i ++)
        scanf ("%lu", &matrix[i]);

    return matrix;
}

int TransposeMatrix (long unsigned* matrix, unsigned height, unsigned width)
{
    assert (matrix);
    assert (height > 0 && width > 0);

    for (unsigned i = 0; i < width; i ++)
    {
        for (unsigned j = i; j < height; j ++)
        {
            long unsigned temp = matrix[POS(j, i, width)];

            matrix[POS(j, i, width)] = matrix[POS(i, j, width)];
            matrix[POS(i, j, width)] = temp;
        }
    }

    return 0;
}

int PrintMatrix (long unsigned* matrix, unsigned height, unsigned width)
{
    assert (matrix);
    assert (height > 0 && width > 0);

    for (unsigned i = 0; i < width * height; i ++)
    {
        printf ("%lu ", matrix[i]);
        if (i % width == width - 1)
            printf ("\n");
    }

    return 0;
}
#endif // DYNAMICLIB_H_INCLUDED
