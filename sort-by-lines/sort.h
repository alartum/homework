#ifndef SORT_H_INCLUDED
#define SORT_H_INCLUDED

#include "mystring.h"

template <class T>
int Swap (T* data, size_t i, size_t j)
{
    T temp;

    temp = data[i];
    data[i] = data[j];
    data[j] = temp;

    return 0;
}

void sort_strings (String** strings, size_t from, size_t to, int (*comp)(const String* str1, const String* str2))
{
    size_t i = 0, last = 0;

    if (from >= to)
        return;
    Swap (strings, from, (from + to) / 2);
    last = from;
    for (i = from + 1; i <= to; i++)
        if ((*comp)(strings[i], strings[from]) < 0)
            Swap (strings, ++last, i);
    Swap (strings, from, last);
    // We are using unsigned, so, it can lead us to undefined behavior.
    if (last > 0)
        sort_strings (strings, from, last - 1, comp);
    sort_strings (strings, last + 1, to, comp);
}
#endif // SORT_H_INCLUDED
