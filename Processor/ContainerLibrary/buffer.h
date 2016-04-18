#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED

#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>

typedef struct
{
    // Array of symbols
    char* chars;
    // Length of the array (amount of symbols)
    size_t length;
} Buffer;

void buffer_destruct (Buffer* This);
int buffer_construct (Buffer* This, const char filename[]);
int buffer_construct_empty (Buffer* This, size_t size);
bool buffer_OK (const Buffer* This);
void buffer_dump (const Buffer* This);

void buffer_destruct (Buffer* This)
{
    assert (This);
    if (!This)
        return;
    if (This->chars)
        free (This->chars);
    // Just to be sure
    This->chars = NULL;
    This->length = 0;
}

bool buffer_OK (const Buffer* This)
{
    assert(This);
    return This && This->length && This->chars;
}

void buffer_dump (const Buffer* This)
{
    assert (This);
    printf ("Buffer ");
    if (!This)
    {
        printf("(NULL)");
        return;
    }
    if (buffer_OK(This))
        printf ("(ok)\n");
    else
        printf ("(ERROR)\n");
    if (!This)
    {
        printf("NULL");
        return;
    }
    printf ("{\n");
    printf ("\tlength = %lu\n", This->length);
    printf ("\t&chars = %p\n", This->chars);
    printf ("\tchars = ");
    if (This->chars == NULL)
        printf ("NONE\n");
    else
    {
        printf ("<");
        size_t i = 0;
        for (; i < This->length; i++)
        {
            if (This->chars[i] == '\0')
                printf ("#");
            else
                printf ("%c", This->chars[i]);
        }
        printf (">\n");
    }
    printf ("}\n");
}

bool buffer_construct_file (Buffer* This, FILE* file)
{
    assert (This);
    assert (file);

    #define BUF_ERROR() \
    {\
        buffer_destruct (This);\
        return false;\
    }

    if (fseek (file, 0, SEEK_END)) // writes to errno too
        BUF_ERROR();
    This->length = ftell (file);

    if (errno || This->length == 0)
    {
        errno = EIO;
        BUF_ERROR();
    }
    This->chars = (char*) calloc (This->length, sizeof (char));
    if (!This->chars)
    {
        errno = ENOMEM;
        BUF_ERROR();
    }

    rewind (file);
    if (fread (This->chars, sizeof (char), This->length, file) != This->length)
    {
        errno = EIO;
        BUF_ERROR();
    }

    #undef BUF_ERROR

    return true;
}

int buffer_construct (Buffer* This, const char filename[])
{
    assert (This);
    FILE *f = fopen (filename, "rb");

    if (!f) // fopen() writes to errno automatically
    {
        printf ("Buffer::can't open file named \"%s\"\n", filename);
        buffer_destruct (This);
        return false;
    }

    #define BUF_ERROR() \
    {\
        buffer_destruct (This);\
        fclose (f);\
        return false;\
    }

    if (fseek (f, 0, SEEK_END)) // writes to errno too
        BUF_ERROR();
    This->length = ftell (f);

    if (errno || This->length == 0)
    {
        errno = EIO;
        BUF_ERROR();
    }
    This->chars = (char*) calloc (This->length, sizeof (char));
    if (!This->chars)
    {
        errno = ENOMEM;
        BUF_ERROR();
    }

    rewind (f);
    if (fread (This->chars, sizeof (char), This->length, f) != This->length)
    {
        errno = EIO;
        BUF_ERROR();
    }

    #undef BUF_ERROR

    fclose (f);
    return true;
}

int buffer_construct_empty (Buffer* This, size_t size)
{
    assert (This);
    This->length = size;
    This->chars = (char*) calloc (This->length, sizeof (char));
    if (!This->chars)
    {
        errno = ENOMEM;
        buffer_destruct (This);
        return false;
    }
    memset (This->chars, 0, This->length);

    return true;
}

int buffer_construct_copy (Buffer* This, const Buffer* other)
{
    assert (This);
    assert (buffer_OK(other));
    This->length = other->length;
    This->chars = (char*) calloc (This->length, sizeof (char));
    if (!This->chars)
    {
        errno = ENOMEM;
        buffer_destruct (This);
        return false;
    }
    memcpy (This->chars, other->chars, This->length);

    return true;
}

#endif // BUFFER_H_INCLUDED
