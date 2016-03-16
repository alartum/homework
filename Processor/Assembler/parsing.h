#ifndef PARSING_H_INCLUDED
#define PARSING_H_INCLUDED

#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <tree.h>

// Pointer to the current symbol
char* _S = NULL;
#define MAX_WORD 256

#define CHECK_POINTER(); if (*_S == '\0') return NULL;
//CHARED::= NAME+BRACKETS | NAME
TreeNode* getCHARED();
//MIXED::= CHARED | BRACKETS | NUMBER
TreeNode* getMIXED();
//NAME::=['A'-'Z', 'a'-'z']+['0'-'9']*
char* getNAME();
//NUMBER::=['0'-'9']+['.']+['0'-'9']
TreeNode* getNUMBER();
//SUM::= MULT(['+', '-']MULT)*
TreeNode* getSUM();
//MULT::= POW(['*', '/']POW)*
TreeNode* getMULT();
//BRACKETS::= '('SUM')'
TreeNode* getBRACKETS();
//POW::= MIXED([^]MIXED)*
TreeNode* getPOW();
//Control sequence
//INPUT::=SUM
TreeNode* getINPUT(const char* str);

char* clear_string (const char string[])
{
    assert (string);
    char* p = (char*)string;
    char* clear_copy = (char*)calloc (strlen(string), sizeof(*clear_copy));
    char* last = clear_copy;

    for (; *p; p++)
        if (!strchr (" \n\t\r", *p))
        {
            *last = *p;
            last++;
        }
    *last = '\0';
    char* cleared_string = strdup (clear_copy);
    free (clear_copy);

    return cleared_string;
}


//Interface function
TreeNode* tree_node_from_string (const char* string)
{
    assert (string);
    char* cleared_string = clear_string(string);
    TreeNode* result = getINPUT(cleared_string);
    //tree_node_show_dot(result);
    free (cleared_string);
    if (!result)
        printf ("ERROR::Can't parse the input.\n");
    return result;
}

TreeNode* getNUMBER()
{
    float val = 0, mul = 1;
    char* saved = _S;
    if (*_S == '-' && isdigit(*(_S + 1)))
    {
        _S++;
        mul = -1;
    }
    while (isdigit(*_S))
    {
        CHECK_POINTER();
        val = 10*val + *_S - '0';
        _S++;
    }
    if (*_S == '.')
    {
        int exp = -1;
        _S++;
        while (isdigit(*_S))
        {
            CHECK_POINTER();
            val += (*_S - '0') * pow ((double)10, (double)exp);
            exp--;
            _S++;
        }
    }
    val *= mul;
    if (saved != _S)
    {
       // printf ("NUM <%f>\n", val);
        return _NUM(&val);
    }
    else
        return NULL;
}

//E::= T(['+', '-']T)*
TreeNode* getSUM()
{
    TreeNode* parent_node = NULL;
    TreeNode* left_node = getMULT();
    TreeNode* right_node = NULL;
    while (*_S == '-' || *_S == '+')
    {
        char op = *_S++;
        CHECK_POINTER();
        right_node = getMULT();
        if (op == '+')
            parent_node = _ADD (left_node, right_node);
        else if (op == '-')
            parent_node = _SUB (left_node, right_node);
        left_node = parent_node;
    }

    return left_node;
}

//T::= P(['*', '/']P)*
TreeNode* getMULT()
{
    TreeNode* parent_node = NULL;
    TreeNode* left_node = getPOW();
    TreeNode* right_node = NULL;

    while (*_S == '*' || *_S == '/')
    {
        char op = *(_S++);
        CHECK_POINTER();
        right_node = getPOW();
        if (op == '*')
            parent_node = _MUL (left_node, right_node);
        else if (op == '/')
            parent_node = _DIV (left_node, right_node);
        left_node = parent_node;
    }

    return left_node;
}

//Control sequence
TreeNode* getINPUT(const char* str)
{
    _S = (char*)str;
    TreeNode* root_node = getSUM();
    assert (*_S == 0);

    return root_node;
}

//P::= N | B | F
TreeNode* getMIXED()
{
    char* saved = _S;
    CHECK_POINTER();
    TreeNode* new_node = getNUMBER();
    if (saved != _S)
        return new_node;
    //else
    new_node = getBRACKETS();
    if (saved != _S)
    {
        return new_node;
    }
    //else
    new_node = getCHARED();
    //BADABOOM("fu");
    //tree_node_show_dot(new_node);
    return new_node;
}

char* getNAME()
{
    CHECK_POINTER();
    unsigned current = 0;
    char word[MAX_WORD] = {};
    while (isalpha(*_S) || *_S== '_')
    {
        word[current] = *_S;
        _S++;
        current++;
    }
    while (isdigit(*_S))
    {
        word[current] = *_S;
        _S++;
        current++;
    }
    word[current] = '\0';
    if (word[0])
    {
        //printf ("NAME \"%s\"\n", word);
        return strdup(word);
    }
    return NULL;
}

TreeNode* getCHARED()
{
    char* name = getNAME();

    if (!name)
        return NULL;
    TreeNode* new_node = getBRACKETS();
    if (new_node)
    {
        //BADABOOM("Here");
        //printf ("name: %s\n", name);
        //tree_node_dump(new_node);
        TreeNode* parent = _FU(name, new_node);
        //tree_node_dump(parent);
        free (name);
        return parent;
    }
    //else
    //printf ("VAR <%s>\n", name);
    new_node = _VAR(name);
    free(name);
    return new_node;
}

TreeNode* getBRACKETS()
{
    if (*_S == '(')
    {
        _S++;
        TreeNode* new_node = getSUM();
        assert (*_S == ')');
        _S++;
        //tree_node_dump(new_node);
        return new_node;
    }
    else
        return NULL;
}

TreeNode* getPOW()
{
    TreeNode* parent_node = NULL;
    TreeNode* base_node = getMIXED();

    while (*_S == '^')
    {
        _S++;
        TreeNode* exp_node = getMIXED();
        parent_node = _POW(base_node, exp_node);
        base_node = parent_node;
    }
    parent_node = base_node;
    //printf ("POW ENDED\n");
   // tree_node_show_dot(parent_node);

    return parent_node;
}

#endif // PARSING_H_INCLUDED
