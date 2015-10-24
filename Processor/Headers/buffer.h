#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED

#define OK 0
#define ERROR 1
#include <string.h>

typedef struct
{
    // Array of symbols
    char* chars;
    // Length of the array (amount of symbols)
    size_t length;
}  Buffer;

void buffer_destruct (Buffer* This);
int buffer_construct (Buffer* This, const char filename[]);
bool buffer_OK (const Buffer* This);
void buffer_dump (const Buffer* This);

void buffer_destruct (Buffer* This)
{
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
    return This && This->length && This->chars;
}

void buffer_dump (const Buffer* This)
{
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
        for (size_t i = 0; i < This->length; i++)
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

int buffer_construct (Buffer* This, const char filename[])
{
    FILE *f = fopen (filename, "rb");

    if (!f) // fopen() writes to errno automatically
    {
        buffer_destruct (This);
        perror ("HERE");
        return ERROR;
    }
    if (fseek (f, 0, SEEK_END)) // writes to errno too
    {

        buffer_destruct (This);
        fclose (f);
        return ERROR;
    }
    This->length = ftell (f);

    if (errno || This->length == 0)
    {
        errno = EIO;
        buffer_destruct (This);
        fclose (f);
        return ERROR;

    }
    This->chars = (char*) calloc (This->length, sizeof (char));
    if (!This->chars)
    {
        errno = ENOMEM;
        buffer_destruct (This);
        fclose (f);
        return ERROR;
    }

    rewind (f);
    if (fread (This->chars, sizeof (char), This->length, f) != This->length)
    {
        errno = EIO;
        buffer_destruct (This);
        fclose (f);
        return ERROR;
    }

    fclose (f);
    return OK;
}

#endif // BUFFER_H_INCLUDED
