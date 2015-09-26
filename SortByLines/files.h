#ifndef FILES_H_INCLUDED
#define FILES_H_INCLUDED

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>


int SaveMapToFile (const char filename[], char** linesMap, size_t nLines)
{
    assert (linesMap);
    if (!linesMap)
    {
        errno = EFAULT;
        return 1;
    }
    FILE *f = fopen (filename, "w");
    if (!f) // fopen() writes to errno automatically
        return 1;

    for (size_t i = 0; i < nLines; i ++)
    {
        assert(linesMap[i]);
        if (!linesMap[i])
        {
            errno = EFAULT;
            return 1;
        }
        fprintf (f, "%s\n", linesMap[i]);
    }

    fclose (f);
    return 0;
}
// Reads file to buffer. Returnes the number of items successfully read
char* ReadFileDynamic (const char filename[], size_t* nRead = NULL)
{
    // Indicator value
    if (nRead)
        *nRead = 0;
    FILE *f = fopen (filename, "rb");
    if (!f) // fopen() writes to errno automatically
        return NULL;
    if (fseek (f, 0, SEEK_END)) // writes to errno too
        return NULL;

    size_t length = ftell(f);
    // Changing indicator value
    if (nRead)
        *nRead = length;

    if (errno)
        return NULL;
    else
        length /= sizeof (char);

    if (length == 0)
    {
        errno = EIO;
        return NULL;
    }
    char* buffer = (char*) calloc (length, sizeof (char));
    if (!buffer)
    {
        errno = ENOMEM;
        return NULL;
    }

    rewind (f);
    if (fread (buffer, sizeof (char), length, f) != length)
    {
        free (buffer);
        return NULL;
    }

    fclose (f);
    return buffer;
}

size_t CountLines (char chars[], size_t length)
{
    if (!chars || length == 0)
    {
        errno = EIO;
        return 0;
    }

    size_t nStrings = 0;
    for (size_t i = 0; i < length; i ++)
        if (chars[i] == '\n')
            nStrings ++;

    return nStrings;
}

char** BuildLinesMap (char chars[], size_t length, size_t* nLines = NULL)
{
    if (nLines)
        *nLines = 0;
    if (!chars || length == 0)
    {
        errno = EFAULT;
        return NULL;
    }

    size_t nStrings = CountLines(chars, length);
    if (nLines)
        *nLines = nStrings;

    char** stringsMap = (char**) calloc (nStrings, sizeof (*stringsMap));
    if (!stringsMap)
    {
        errno = ENOMEM;
        return NULL;
    }

    size_t firstLetter = 0;
    for (; chars[firstLetter] < '\n' && chars[firstLetter] > 0; firstLetter ++);
    stringsMap[0] = &chars[firstLetter];
    size_t current = 1;

    for (size_t i = firstLetter; i < length - 1; i ++)
    {
        if (chars[i] == '\n')
        {
            if (chars[i - 1] == '\r')
                chars[i - 1] = '\0';
            else
                chars[i] = '\0';

            stringsMap[current] = &chars[i + 1];
            current++;
        }
    }

    size_t lastLetter = length - 1;
    for (; chars[lastLetter] < '\n' && chars[lastLetter] > 0; lastLetter--);
    chars[lastLetter] = '\0';

    return stringsMap;
}

int PrintLinesMap (const char* linesMap[], size_t nLines)
{
    assert (linesMap);
    if (!linesMap)
    {
        errno = EFAULT;
        return 1;
    }
    for (size_t i = 0; i < nLines; i ++)
    {
        assert(linesMap[i]);
        if (!linesMap[i])
        {
            errno = EFAULT;
            return 1;
        }
        printf ("%s\n", linesMap[i]);
    }

    return 0;
}

int CompareStrings (const char* s1, const char* s2)
{
    assert (s1 && s2);
    if (!(s1 && s2))
    {
        errno = EFAULT;

        return -2;
    }
    for ( ; *s1 == *s2; s1++, s2++)
        if (*s1 == '\0')
            return 0;

    return ((*s1 < *s2) ? -1 : +1);
}
int CompareStringsBack (const char* s1, const char* s2)
{
    assert (s1 && s2);
    if (*s1 == '\0' && *s1 == '\0')
        return 0;
    if (*s1 == '\0')
        return -1;
    if (*s2 == '\0')
        return 1;
    if (!(s1 && s2))
    {
        errno = EFAULT;

        return -2;
    }
    const char* start = s1;

    for ( ; *s1 != '\0'; s1++);
    for ( ; *s2 != '\0'; s2++);
    for ( ; *s1 == *s2 && s1 != start; s1--, s2--);

    if (*s1 == *s2)
        return 0;
    return ((*s1 < *s2) ? -1 : +1);
}

template <class T>
int Swap (T* data, size_t i, size_t j)
{
    T temp;

    temp = data[i];
    data[i] = data[j];
    data[j] = temp;

    return 0;
}

int SortLinesMapBack (char* linesMap[], size_t from, size_t to)
{
    size_t i = 0, last = 0;

    if (from >= to)
        return 0;
    Swap (linesMap, from, (from + to) / 2);
        last = from;
    for (i = from + 1; i <= to; i++)
        if (CompareStringsBack(linesMap[i], linesMap[from]) < 0)
            Swap (linesMap, ++last, i);
    Swap (linesMap, from, last);
    if (last > 0)
        SortLinesMapBack (linesMap, from, last - 1);
    SortLinesMapBack (linesMap, last + 1, to);

    return 0;
}

int QSortLinesBack (char* linesMap[], size_t nLines)
{
    SortLinesMapBack(linesMap, 0, nLines - 1);

    return 0;
}

int QSort (int data[], size_t from, size_t to)
{
    size_t i = 0, last = 0;

    COMMENT("Sorting:");
    DPRINT(from);
    DPRINT(to);
    for (size_t i = from; i <= to; i++)
        printf ("%d ", data[i]);
    printf ("\n");
    if (from >= to)
    {
        COMMENT("Finished!");
        return 0;
    }
    Swap (data, from, (from + to) / 2);
        last = from;
    COMMENT("After swap:");
    for (size_t i = from; i <= to; i++)
        printf ("%d ", data[i]);
    printf ("\n");
    COMMENT("Iterating:");
    for (i = from + 1; i <= to; i++)
    {
        if (data[i] < data[from])
        {
            Swap (data, ++last, i);
            for (size_t i = from; i <= to; i++)
                printf ("%d ", data[i]);
            printf ("\n");
        }
    }
    COMMENT ("Swap:");
    Swap (data, from, last);
    for (size_t i = from; i <= to; i++)
        printf ("%d ", data[i]);
    getchar();
    if (last > 0)
        QSort (data, from, last - 1);
    QSort (data, last + 1, to);

    return 0;
}
#endif // FILES_H_INCLUDED
