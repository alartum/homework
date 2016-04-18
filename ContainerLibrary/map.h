#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "mylib.h"
#include "buffer.h"
#include "tree.h"
#include "parsing.h"
#define MAX_ITEMS 1000

/// More comfortable dump
#define DifferMap_dump(This) differ_map_dump_(This, #This)
/// To be stylish
#define differ_map_dump(This) DifferMap_dump(This)
/// To be stylish
#define DifferMap_OK(This) differ_map_OK(This)

typedef struct
{
    size_t amount;
    TreeNode **values;
    char **keys;

    size_t max_amount;
    bool state;
} DifferMap;


bool differ_map_OK (const DifferMap* This)
{
    assert (This);
    return This->state;
}

void differ_map_destruct (DifferMap* This)
{
    assert (This);
    for (int i = 0; i < This->amount; i++)
    {
        if (This->keys[i])
            free(This->keys[i]);
        tree_node_destruct(&This->values[i]);
    }
    if (This->keys)
        free(This->keys);
    if (This->values)
        free(This->values);
    This->amount = 0;
    This->state = false;
}
bool differ_map_construct (DifferMap* This, size_t amount)
{
    assert (This);
    This->amount = 0;
    This->max_amount = amount;
    This->keys = (char**)calloc(amount, sizeof(char*));
    if (!This->keys)
    {
        differ_map_destruct(This);
        return false;
    }
    This->values = (TreeNode**)calloc(amount, sizeof(TreeNode*));
    if (!This->values)
    {
        differ_map_destruct(This);
        return false;
    }
    This->state = true;
    return true;
}



void differ_map_dump_ (const DifferMap* This, const char name[])
{
    assert (This);
    printf ("%s = DifferMap (", name);
    if (differ_map_OK(This))
        printf ("ok)\n");
    else
        printf ("ERROR)\n");
    printf ("{\n");
    printf ("\tamount = %lu\n", This->amount);
    printf ("\tmax_amount = %lu\n", This->max_amount);
    printf ("\tItems:\n");
    for (int i = 0; i < This->amount; i++)
    {
        printf ("\t   [%s] : <showing graph>\n", This->keys[i]);
        tree_node_show_dot(This->values[i]);
    }
    printf ("}\n");
}

bool differ_map_add (DifferMap* This, const char key[], const char value[])
{
    ASSERT_OK(DifferMap, This);
    assert (key);
    assert (value);
    if (This->amount >= This->max_amount)
    {
        differ_map_destruct(This);
        return false;
    }
    //else
    for (int i = 0; i < This->amount; i++)
        if (!strcmp(This->keys[i], key))
            return true;
    //if key is not present
    This->keys[This->amount] = strdup(key);
    This->values[This->amount] = tree_node_from_string(value);
    This->amount++;

    return true;
}

bool differ_map_add_node (DifferMap* This, const char key[], const TreeNode* value)
{
    ASSERT_OK(DifferMap, This);
    ASSERT_OK(TreeNode, value);
    assert (key);
    if (This->amount >= This->max_amount)
    {
        differ_map_destruct(This);
        return false;
    }
    //else
    for (int i = 0; i < This->amount; i++)
        if (!strcmp(This->keys[i], key))
            return true;
    //if key is not present
    This->keys[This->amount] = strdup(key);
    This->values[This->amount] = tree_node_full_copy(value);
    This->amount++;

    return true;
}

TreeNode* differ_map_get (const DifferMap* This, const char key[])
{
    ASSERT_OK(DifferMap, This);
    assert (key);
    for (int i = 0; i < This->amount; i++)
        if (!strcmp(This->keys[i], key))
            return tree_node_full_copy(This->values[i]);

    return NULL;
}

bool differ_map_construct_filename(DifferMap* This, const char filename[])
{
    assert(This);
    assert(filename);
    Buffer input = {};
    buffer_construct(&input, filename);
    size_t Nfunctions = 0;
    for (int i = 0; i < input.length; i++)
        if (input.chars[i] == '\n')
            Nfunctions++;
    if (!differ_map_construct(This, Nfunctions))
        return false;
    char* line = strtok (input.chars,"\n");
    while (line != NULL)
    {
        int Nread = 0;
        char function[NAME_MAX], derivative[NAME_MAX];
        sscanf (line, "%s%n", function, &Nread);
        sscanf (line + Nread, "%s", derivative);
        differ_map_add(This, (const char*)function, (const char*)derivative);
        line = strtok (NULL, "\n");
    }
    return true;
}

bool differ_map_construct_file (DifferMap* This, FILE* file)
{
    assert(This);
    assert(file);
    Buffer input = {};
    buffer_construct_file(&input, file);
    size_t Nfunctions = 0;
    for (int i = 0; i < input.length; i++)
        if (input.chars[i] == '\n')
            Nfunctions++;
    if (!differ_map_construct(This, Nfunctions))
        return false;
    char* line = strtok (input.chars,"\n");
    while (line != NULL)
    {
        int Nread = 0;
        char function[NAME_MAX], derivative[NAME_MAX];
        sscanf (line, "%s%n", function, &Nread);
        sscanf (line + Nread, "%s", derivative);
        differ_map_add(This, (const char*)function, (const char*)derivative);
        line = strtok (NULL, "\n");
    }
    return true;
}


////////////////////////
////Just Typical Map////
////////////////////////

/// More comfortable dump
#define Map_dump(This) map_dump_(This, #This)
/// To be stylish
#define map_dump(This) Map_dump(This)
/// To be stylish
#define Map_OK(This) map_OK(This)

typedef struct
{
    size_t amount;
    char **values;
    char **keys;

    size_t max_amount;
    bool state;
} Map;


bool map_OK (const Map* This)
{
    assert (This);
    return This->state;
}

void map_destruct (Map* This)
{
    assert (This);
    for (int i = 0; i < This->amount; i++)
    {
        if (This->keys[i])
            free(This->keys[i]);
        if (This->values[i])
            free(This->values[i]);
    }
    if (This->keys)
        free(This->keys);
    if (This->values)
        free(This->values);
    This->amount = 0;
    This->state = false;
}
bool map_construct (Map* This, size_t amount)
{
    assert (This);
    This->amount = 0;
    This->max_amount = amount;
    This->keys = (char**)calloc(amount, sizeof(char*));
    if (!This->keys)
    {
        map_destruct(This);
        return false;
    }
    This->values = (char**)calloc(amount, sizeof(char*));
    if (!This->values)
    {
        map_destruct(This);
        return false;
    }
    This->state = true;
    return true;
}



void map_dump_ (const Map* This, const char name[])
{
    assert (This);
    printf ("%s = Map (", name);
    if (map_OK(This))
        printf ("ok)\n");
    else
        printf ("ERROR)\n");
    printf ("{\n");
    printf ("\tamount = %lu\n", This->amount);
    printf ("\tmax_amount = %lu\n", This->max_amount);
    printf ("\tItems:\n");
    for (int i = 0; i < This->amount; i++)
        printf ("\t   [%s] : \"%s\"\n", This->keys[i], This->values[i]);
    printf ("}\n");
}

bool map_add (Map* This, const char key[], const char value[])
{
    ASSERT_OK(Map, This);
    assert (key);
    assert (value);
    if (This->amount >= This->max_amount)
    {
        printf ("amount");
        map_destruct(This);
        return false;
    }
    //else
    for (int i = 0; i < This->amount; i++)
        if (!strcmp(This->keys[i], key))
            return true;
    //if key is not present
    This->keys[This->amount] = strdup(key);
    This->values[This->amount] = strdup(value);
    This->amount++;

    return true;
}

char* map_get (Map* This, const char key[])
{
    ASSERT_OK(Map, This);
    assert (key);
    for (int i = 0; i < This->amount; i++)
        if (!strcmp(This->keys[i], key))
            return strdup(This->values[i]);

    return NULL;
}

/// More comfortable dump
#define ValMap_dump(This) val_map_dump_(This, #This)
/// To be stylish
#define val_map_dump(This) ValMap_dump(This)
/// To be stylish
#define ValMap_OK(This) val_map_OK(This)

typedef struct
{
    char **keys;
    float *values;
    size_t amount;

    size_t max_amount;
    bool state;
} ValMap;


bool val_map_OK (const ValMap* This)
{
    assert (This);
    return This->state;
}

void val_map_destruct (ValMap* This)
{
    assert (This);
    for (int i = 0; i < This->amount; i++)
    {
        if (This->keys[i])
            free(This->keys[i]);
    }
    if (This->keys)
        free(This->keys);
    if (This->values)
        free(This->values);
    This->amount = 0;
    This->state = false;
}
bool val_map_construct (ValMap* This, size_t amount)
{
    assert (This);
    This->amount = 0;
    This->max_amount = amount;
    This->keys = (char**)calloc(amount, sizeof(char*));
    if (!This->keys)
    {
        val_map_destruct(This);
        return false;
    }
    This->values = (float*)calloc(amount, sizeof(float));
    if (!This->values)
    {
        val_map_destruct(This);
        return false;
    }
    This->state = true;
    return true;
}


void val_map_dump_ (const ValMap* This, const char name[])
{
    assert (This);
    printf ("%s = Map (", name);
    if (val_map_OK(This))
        printf ("ok)\n");
    else
        printf ("ERROR)\n");
    printf ("{\n");
    printf ("\tamount = %lu\n", This->amount);
    printf ("\tmax_amount = %lu\n", This->max_amount);
    printf ("\tItems:\n");
    for (int i = 0; i < This->amount; i++)
        printf ("\t   [%s] : \"%g\"\n", This->keys[i], This->values[i]);
    printf ("}\n");
}

bool val_map_add (ValMap* This, const char key[], const float value)
{
    ASSERT_OK(ValMap, This);
    assert (key);
    if (This->amount >= This->max_amount)
    {
        val_map_destruct(This);
        return false;
    }
    //else
    for (int i = 0; i < This->amount; i++)
        if (!strcmp(This->keys[i], key))
            return true;
    //if key is not present
    This->keys[This->amount] = strdup(key);
    This->values[This->amount] = value;
    This->amount++;

    return true;
}

float val_map_get (ValMap* This, const char key[])
{
    ASSERT_OK(ValMap, This);
    assert (key);
    for (int i = 0; i < This->amount; i++)
        if (!strcmp(This->keys[i], key))
            return This->values[i];

    return 0;
}
#endif // MAP_H_INCLUDED
