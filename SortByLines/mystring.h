#ifndef MYSTRING_H_INCLUDED
#define MYSTRING_H_INCLUDED

#include <string.h>
#include "mylib.h"
#include <errno.h>
#include "errors.h"

#define LESS -1
#define GREATER 1
#define EQUAL 0

struct String
{
    // String itself
    char* str;
    // Length of the string
    size_t length;
};

const char NOT_ALPHA[] = "\"',./?!$%#@`~-+=)(*:;&^1234567890[]{}><";
// Constructs string, treating '\n' and '\r' as '\0'
int string_construct_rn (String* This, const char* str);
// Constructing string by default char array.
// OK is returned on success, ERROR otherwise
int string_construct (String* This, const char* str);
// Destructs the string. Memory is cleaned.
void string_destruct (String* This);
// Copies source string to This.
// Returnes OK on success, otherwise returnes ERROR
int string_construct (String* This, const String* source);
// Validating string
// True is returned if valid, otherwise false
bool string_OK (const String* This);
// Outputs information about This string.
void string_dump (const String* This);
// Compares two strings and returns:
// LESS if str1 < str2
// GREATER if str1 > str2
// EQUAL if str1 == str2
int string_compare (const String* str1, const String* str2);
// Comparisson from the back. Return states equal to str_compare.
int string_compare_back (const String* str1, const String* str2);
// Equal to str_compare_back, but counts letters only
int string_compare_back_letters (const String* str1, const String* str2);
// Equal to str_compare, but counts letters only
int string_compare_letters (const String* str1, const String* str2);


bool string_OK (const String* This)
{
    return This && ((!This->length && !This->str) || (This->length && This->str));
}

int string_construct (String* This, const char* str)
{
    // Passing length to avoid overflow
    assert (str);

    if (str[0] == '\0')
    {
        This->length = 0;
        This->str = NULL;
    }
    else
    {
        This->str = strdup (str);
        This->length = strlen (str);
    }
    if (!This->str)
    {
        This->str = NULL;
        This->length = 0;
        return ERROR;
    }

    return OK;
}

int string_construct (String* This, const String* source)
{
    // Passing length to avoid overflow
    assert (string_OK(source));

    if (source->length == 0)
    {
        This->length = 0;
        This->str = NULL;
    }
    else
    {
        This->length = source->length;
        This->str = strdup (source->str);
        if (!This->str)
        {
            This->str = NULL;
            This->length = -1;
            return ERROR;
        }
    }

    return OK;
}

void string_destruct (String* This)
{
    if (!This)
        return;
    if (This->str)
        free (This->str);
    // Just to be sure
    This->str = NULL;
    This->length = 0;
}

int string_compare (const String* str1, const String* str2)
{
    assert (string_OK(str1));
    assert (string_OK(str2));

    if (str1->length == 0 && str2->length == 0)
        return EQUAL;
    if (str1->length == 0)
        return LESS;
    if (str2->length == 0)
        return GREATER;

    char* ptr1 = str1->str;
    char* ptr2 = str2->str;
    for (; *ptr1 == *ptr2; ptr1++, ptr2++)
        if (*ptr1 == '\0')
            return EQUAL;

    return ((*ptr1 < *ptr2)? LESS : GREATER);
}

int string_compare_back (const String* str1, const String* str2)
{
    assert (string_OK(str1));
    assert (string_OK(str2));

    if (str1->length == 0 && str2->length == 0)
        return EQUAL;
    if (str1->length == 0)
        return LESS;
    if (str2->length == 0)
        return GREATER;

    char* ptr1 = str1->str + str1->length - 1;
    char* ptr2 = str2->str + str2->length - 1;
    for (; *ptr1 == *ptr2; ptr1--, ptr2--)
        if (ptr1 == str1->str)
            return EQUAL;

    return ((*ptr1 < *ptr2)? LESS : GREATER);
}

int string_compare_back_letters (const String* str1, const String* str2)
{
    assert (string_OK(str1));
    assert (string_OK(str2));

    if (str1->length == 0 && str2->length == 0)
        return EQUAL;
    if (str1->length == 0)
        return LESS;
    if (str2->length == 0)
        return GREATER;

    char* ptr1 = str1->str + str1->length - 1;
    char* ptr2 = str2->str + str2->length - 1;
    for (; ptr1 != str1->str && strchr (NOT_ALPHA, *ptr1); ptr1--);
    for (; ptr2 != str2->str && strchr (NOT_ALPHA, *ptr2); ptr2--);
    for (; *ptr1 == *ptr2; ptr1--, ptr2--)
        if (ptr1 == str1->str)
            return EQUAL;

    return ((*ptr1 < *ptr2)? LESS : GREATER);
}

int string_compare_letters (const String* str1, const String* str2)
{
    assert (string_OK(str1));
    assert (string_OK(str2));

    if (str1->length == 0 && str2->length == 0)
        return EQUAL;
    if (str1->length == 0)
        return LESS;
    if (str2->length == 0)
        return GREATER;

    char* ptr1 = str1->str;
    char* ptr2 = str2->str;
    for (; strchr (NOT_ALPHA, *ptr1); ptr1++) ; // strcpn strspn srpbrk
    for (; strchr (NOT_ALPHA, *ptr1); ptr2++) ;
    for (; *ptr1 == *ptr2; ptr1++, ptr2++)
        if (*ptr1 == '\0')
            return EQUAL;

    return ((*ptr1 < *ptr2)? LESS : GREATER);
}

void string_dump (const String* This)
{
    printf ("String ");
    if (!This)
    {
        printf("(NULL)");
        return;
    }
    if (string_OK(This))
        printf ("(ok)\n");
    else
        printf ("(ERROR)\n");

    printf ("{\n");
    printf ("\tlength = %lu\n", This->length);
    printf ("\t&str = %p\n", This->str);
    printf ("\tstr = ");
    if (This->length == 0)
        printf ("NONE\n");
    else
    {
        printf ("<");
        for (int i = 0; This->str[i] != '\0'; i++)
            printf ("%c", This->str[i]);
        printf (">\n");
    }
    printf ("}\n");
}

int string_construct_rn (String* This, const char* str)
{
    assert (str);

    size_t i = 0;
    for (; str[i] != '\r' && str[i] != '\n' && str[i] != '\0'; i++);
    //DPRINT(i);
    if (i == 0)
    {
        This->length = 0;
        This->str = NULL;
        return OK;
    }
    else
    {

        This->length = i + 1;

        This->str = (char*) calloc (This->length, sizeof (char));

        if (!This->str)
        {
            errno = ENOMEM;
            string_destruct(This);
            return ERROR;
        }

        memcpy (This->str, str, i);
        This->str[i] = '\0';
        string_dump(This);
        return OK;
    }
}
#endif // MYSTRING_H_INCLUDED
