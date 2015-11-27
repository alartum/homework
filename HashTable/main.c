//#define DEBUG
/// Author name
#define AUTHOR "Alartum"
/// Project name
#define PROJECT "Hash Table"
/// Version
#define VERSION "1"

#include <stdio.h>
#include "mylib.h"
#include "list.h"
#include "hash_functions.h"
#include <limits.h>
#include <ctype.h>
#define OK_PUNCT "-'"
#define HASH_TABLE_SIZE 499

int main (int argc, char* argv[])
{
    //^^^^^^^^^^^^^^^^^^^^^^^^^
    //Default part BEGIN
    //^^^^^^^^^^^^^^^^^^^^^^^^^
    CHECK_DEFAULT_ARGS();
    char inName[NAME_MAX] = {}, outName[NAME_MAX] = {};
    switch (argc)
    {
    case 3:
        strcpy (inName, argv[1]);
        strcpy (outName, argv[2]);
        break;
    case 2:
        strcpy (inName, argv[1]);
        strcpy (outName, "info.csv");
        break;
    default:
        WRITE_WRONG_USE();
    }

    Buffer words = {};
    buffer_construct(&words, inName);
    char *word = words.chars;

    #define LISTS(HASH_TYPE) \
        ListHead lists_ ## HASH_TYPE [HASH_TABLE_SIZE];\
        for (int i = 0; i < HASH_TABLE_SIZE; i++)\
            list_head_construct(lists_ ## HASH_TYPE + i);
    LISTS(xor);
    LISTS(const);
    LISTS(length);
    LISTS(char);
    LISTS(sum);

    int counter = 0, total = words.length;
    printf ("Progress:\n");
    while (*word)
    {
        if (counter % (int)total / 1000)
            printf ("\r%3.1f%%", 100.0*counter/total);
        counter++;
        // Locating first letter
        if (!isalnum (*word))
            word++;
        else
        {
            //Pointer to the word
            char *new_word = word;
            // Finding first non-letter
            while (isalnum (*word) || strchr(OK_PUNCT, *word))
            {
                word++;
                counter++;
            }
            //Indicating the end
            *word = '\0';
            size_t position;
            #define HASH_ADD(HASH_TYPE) \
                position = hash_ ## HASH_TYPE(new_word) % HASH_TABLE_SIZE;\
                list_head_add(&lists_ ## HASH_TYPE[position], new_word);
            HASH_ADD(xor);
            HASH_ADD(const);
            HASH_ADD(length);
            HASH_ADD(char);
            HASH_ADD(sum);
            word++;
        }
    }
    buffer_destruct(&words);

    open_file (out, outName, "w", "#Output error");
    fprintf (out, "Hash value, ");
        for (int i = 0; i < HASH_TABLE_SIZE; i++)
            fprintf (out, "%d, ", i);
    fprintf (out, "\n");
    #define PRINT_INFO(HASH_TYPE) \
        fprintf (out, "Hash type: " #HASH_TYPE ", ");\
        for (int i = 0; i < HASH_TABLE_SIZE; i++)\
            fprintf (out, "%lu, ", lists_ ## HASH_TYPE[i].amount);\
        fprintf (out, "\n");

    PRINT_INFO(xor);
    PRINT_INFO(const);
    PRINT_INFO(length);
    PRINT_INFO(char);
    PRINT_INFO(sum);
    close_file(out);
    printf ("\nThe info has been successfully written to %s.\n", outName);

    list_head_destruct(lists_xor);
    list_head_destruct(lists_const);
    list_head_destruct(lists_length);
    list_head_destruct(lists_char);
    list_head_destruct(lists_sum);

    return NO_ERROR;
}
