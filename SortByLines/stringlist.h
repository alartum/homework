#ifndef STRINGLIST_H_INCLUDED
#define STRINGLIST_H_INCLUDED

#include "mystring.h"
#include "buffer.h"
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include "sort.h"

struct StringList
{
    // Array of strings (actually, pointers to them)
    String** strings;
    // Length of the list
    size_t length;
};

void stringlist_init (StringList* This)
{
    This->strings = NULL;
    This->length = 0;
}
int stringlist_construct (StringList* This, const char filename[]);
int stringlist_construct (StringList* This, const Buffer* buffer);
// Destructs the string. Memory is cleaned.
void stringlist_destruct (StringList* This);
// Copies source string to This.
// Returnes OK on success, otherwise returnes ERROR
int stringlist_construct (StringList* This, const StringList* other);
// Validating string
// True is returned if valid, otherwise false
bool stringlist_OK (const StringList* This);
// Outputs information about This string.
void stringlist_dump (const StringList* This);
int stringslist_to_file (const StringList* This, const char filename[]);

bool stringlist_OK (const StringList* This)
{
    return This && This->length && This->strings;
}
void stringlist_destruct (StringList* This)
{
    if (!This)
        return;
    if (This->strings)
    {
        for (size_t i = 0; i < This->length; i++)
            string_destruct(This->strings[i]);
        free (This->strings);
    }

    This->strings = NULL;
    This->length = 0;
}
int stringlist_construct (StringList* This, const Buffer* buffer)
{

    assert (buffer_OK(buffer));

    bool isWindows = 0;
    This->length = 0;
    for (size_t i = 0; i < buffer->length; i ++)
    {
        if (buffer->chars[i] == '\n' || buffer->chars[i] == '\0')
            This->length++;
        if (buffer->chars[i] == '\r')
            isWindows = 1;
    }

    DPRINT(isWindows);
    This->strings = (String**) calloc (This->length, sizeof (String*));
    // Very important line!
    String* new_strings = (String*) calloc (This->length, sizeof (String));
    for (size_t i = 0; i < This->length; i ++)
        This->strings[i] = new_strings + i + isWindows;

    if (!This->strings)
    {
        errno = ENOMEM;
        stringlist_destruct(This);
        return ERROR;
    }
    char* last = buffer->chars;

    for (size_t i = 0; i < This->length; i++)
    {
        if (string_construct_rn (This->strings[i], last) == ERROR)
        {
            stringlist_destruct(This);
            return ERROR;
        }
        last += This->strings[i]->length + isWindows + 1; // <------ Some magic here!
    }

    return OK;
}

int stringlist_construct (StringList* This, const StringList* other)
{
    assert (stringlist_OK(other));

    This->length = other->length;
    This->strings = (String**) calloc (This->length, sizeof (String));
    String* new_strings = (String*) calloc (This->length, sizeof (String));
    for (size_t i = 0; i < This->length; i ++)
        This->strings[i] = new_strings + i;
    if (!This->strings)
    {
        errno = ENOMEM;
        stringlist_destruct(This);
        return ERROR;
    }
    for (size_t i = 0; i < This->length; i++)
        string_construct(This->strings[i], other->strings[i]);

    return OK;
}

int stringslist_to_file (const StringList* This, const char filename[])
{
    assert (stringlist_OK(This));
    FILE *f = fopen (filename, "w");
    if (!f) // fopen() writes to errno automatically
        return ERROR;

    for (size_t i = 0; i < This->length; i ++)
        if (This->strings[i]->length != 0)
            fprintf (f, "%s\n", This->strings[i]->str);

    fclose (f);
    return 0;
}

void stringlist_sort (StringList* This)
{
    sort_strings(This->strings, 0, This->length - 1, string_compare_letters);
}

void stringlist_back_sort (StringList* This)
{
    sort_strings(This->strings, 0, This->length - 1, string_compare_back_letters);
}

int stringlist_construct (StringList* This, const char filename[])
{
    Buffer buffer;

    if (buffer_construct (&buffer, filename) == ERROR)
    {
        stringlist_destruct(This);
        return ERROR;
    }
  //  buffer_dump(&buffer);
    return stringlist_construct(This, &buffer);
}

void stringlist_dump (const StringList* This)
{
    printf ("StringList ");
    if (!This)
    {
        printf("(NULL)");
        return;
    }
    if (stringlist_OK(This))
        printf ("(ok)\n");
    else
        printf ("(ERROR)\n");
    printf ("{\n");
    printf ("\tlength = %lu\n", This->length);
    printf ("\t&strings = %p\n", This->strings);
    printf ("\tstrings = ");
    if (This->strings == NULL)
        printf ("NONE\n");
    else
    {
        printf ("\n");
        for (size_t i = 0; i < This->length; i++)
        {
            printf ("\t[%lu]", i);
            string_dump(This->strings[i]);
            printf ("\n");
        }

    }
    printf ("}\n");
}
#endif // STRINGLIST_H_INCLUDED
