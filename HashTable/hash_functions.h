#ifndef HASH_FUNCTIONS_H_INCLUDED
#define HASH_FUNCTIONS_H_INCLUDED

#include <string.h>
size_t hash_xor (const char word[])
{
    char* word_copy = strdup (word);
    size_t hash_value = (size_t)word_copy[0];

    for (int i = 1; word_copy[i] != '\0'; i++)
    {
        *(char*)(&hash_value) ^= word_copy[i];
        hash_value = (hash_value >> 1) | (hash_value << (8*sizeof(size_t) - 1));
    }
    //printf ("XOR [%s]=%lu\n", word_copy, hash_value);
    return hash_value;
}

size_t hash_const (const char word[])
{
    //printf ("CONST=0\n");
    return 0;
}

size_t hash_char (const char word[])
{
    //printf("CHAR=%d\n", word[0]);
    return (size_t)word[0];
}

size_t hash_length (const char word[])
{
    //printf("LENGTH=%lu\n", strlen(word));
    return (size_t)strlen (word);
}

size_t hash_sum (const char word[])
{
    size_t hash_value = 0;
    for (int i =0; word[i] != '\0'; i++)
        hash_value += word[i];
    //printf ("SUM=%lu\n", hash_value);
    return hash_value;
}
#endif // HASH_FUNCTIONS_H_INCLUDED
