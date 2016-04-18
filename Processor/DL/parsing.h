#ifndef PARSING_H_INCLUDED
#define PARSING_H_INCLUDED

#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>


//The parent function to all parsers
//Gets functions until the end
char* dl_parse (const char* program);
char* dl_get_include  ();
char* dl_get_function ();
char* dl_get_body     ();
char* dl_get_expr     ();
char* dl_get_line     ();
char* dl_get_while    ();
char* dl_get_if       ();
char* dl_get_call     ();
char* dl_get_var      ();
char* dl_get_braces   ();
//CHARED::= NAME+BRACKETS | NAME
char* getCHARED();
//MIXED::= CHARED | BRACKETS | NUMBER
char* getMIXED();
//NAME::=['A'-'Z', 'a'-'z']+['0'-'9']*
char* getNAME();
//NUMBER::=['0'-'9']+['.']+['0'-'9']
char* getNUMBER();
//SUM::= MULT(['+', '-']MULT)*
char* getSUM();
//MULT::= POW(['*', '/']POW)*
char* getMULT();
//BRACKETS::= '('SUM')'
char* getBRACKETS();
//POW::= MIXED([^]MIXED)*
char* getPOW();
//Control sequence
//INPUT::=SUM
char* getCOMPAR();

// Pointer to the current symbol
char* _S = NULL;
size_t _WHILE_N = 0;
size_t _IF_N = 0;
#define MAX_WORD 256

#define CHECK_POINTER(); if (*_S == '\0') return NULL;
#define DO_BEGIN(string_1, string_2) !strncmp(string_1, string_2, strlen(string_2))
char* clear_string (const char string[])
{
    assert (string);
    char* p = (char*)string;
    char* clear_copy = (char*)calloc (strlen(string), sizeof(*clear_copy));
    char* last = clear_copy;
    //printf ("String: %s\n", string);

    for (; *p; p++)
    {
        if (*p == '/' && *(p+1) == '/')
            for (;*p && *p != '\n'; p++);
        if (!strchr (" \n\t\r", *p))
        {
            *last = *p;
            last++;
        }
    }
    *last = '\0';
    char* cleared_string = strdup (clear_copy);
    if (clear_copy)
        free (clear_copy);

    return cleared_string;
}
//Merges two strings. This means that the other string will be deleted after being appended
bool strmerge (char** source, char** other)
{
    assert (source);
    assert (other);
    size_t source_size = (*source) ? strlen(*source) : 0;
    size_t other_size  = (*other ) ? strlen(*other ) : 0;

    if (other_size == 0)
        return true;
    *source = (char*)realloc (*source, source_size + other_size + 1);
    if (source_size == 0)
        **source = '\0';
    if (!(*source))
        return false;
    if (*other)
    {
        strcat(*source, *other);
        if (*other)
            free (*other);
        *other = NULL;
    }

    return true;
}

#define APPEND(string, const_string) \
{\
    temp = strdup (const_string);\
    strmerge (&string, &temp);\
}

char* dl_parse(const char* program)
{
    char* parsed = clear_string(program);
    _S = parsed;
    char standart[] = "call main\njmp END\n\n";
    char* done = strdup (standart);

    while (*_S)
    {
        char* temp = dl_get_include();
        strmerge(&done, &temp);
        temp = dl_get_function();
        strmerge(&done, &temp);
        //printf ("Done: %s\n", done);
    }
    char* the_end = strdup ("\n\nEND:\nend\n");
    strmerge(&done, &the_end);

    return done;
}

char* dl_get_include()
{
    char include[] = "#include";
    if (!DO_BEGIN(_S, include))
        return NULL;
    _S += strlen(include);
    if (*_S == '<')
    {
        _S++;
        char file[NAME_MAX] = {};
        sscanf (_S,"%[^>]", file);
        _S += strlen(file);
        _S ++;
        Buffer code = {};
        buffer_construct(&code, file);
        char* code_string = strdup (code.chars);
        buffer_destruct(&code);

        return code_string;
    }
    else
    {
        printf ("ERROR:: #include '<' is missed\n");
        return NULL;
    }
}

char* dl_get_function()
{
    char fu_name[NAME_MAX] = {};
    char* chr = fu_name;
    //printf ("bgbgbgbg %s\n", _S);
    while (isalpha(*_S))
    {
        *chr = *_S;
        _S++;
        chr++;
    }
    while (isalnum(*_S))
    {
        *chr = *_S;
        _S++;
        chr++;
    }
    if (*_S == '(')
    {
        _S++;
        char arg[NAME_MAX] = {};
        char* code = NULL;
        sscanf (_S,"%[a-z9-0]", arg);
        _S += strlen (arg);
        if (*_S != ')')
            return NULL;
        _S++;

        ///////////////////////
        ////FUNCTION BEGINS////
        ///////////////////////
        char* fu_begin = strdup (fu_name);
        strcat (fu_begin, ":\n");
        char* temp = strdup (fu_begin);

        strmerge(&code, &temp);
        ////////////////////////
        ////POPPING ARGUMENT////
        ////////////////////////
        if (strlen(arg))
        {
            temp = strdup ("pop ");
            strcat (arg, "\n");
            char* arg_temp = strdup (arg);
            strmerge(&temp, &arg_temp);
            strmerge(&code, &temp);
        }
        temp = dl_get_braces();
        if (!temp)
            return NULL;

        strmerge(&code, &temp);

        /////////////////////
        ////FUNCTION ENDS////
        /////////////////////
        char* fu_temp = strdup (fu_name);

        strmerge(&code, &fu_temp);
        APPEND(code, "_end:\nret\n");

        return code;
    }
    else
        return NULL;
}

char* dl_get_braces ()
{
    if (*_S != '{')
        return NULL;
    else
    {
        char* body = NULL;
        char* temp = NULL;

        _S++;
        while (*_S && *_S != '}')
        {
            temp = dl_get_line();
            if (!temp)
                return NULL;
            strmerge(&body, &temp);
           // _S++;
        }
        if (*_S != '}')
        {
            printf ("ERROR:: '}' was expected\n");
            if (body)
                free (body);
            return NULL;
        }
        _S++;

        return body;
    }
}

char* dl_get_body     ()
{
    if (*_S == '\0')
        return NULL;

    char* body = NULL;
    char* temp = NULL;
    if (*_S != '{')
        temp = dl_get_line();
    else
        temp = dl_get_braces();

    if (!temp)
        return NULL;
    strmerge(&body, &temp);

    return body;
}

char* dl_get_line ()
{
    char* line = NULL;
    char* saved = _S;
    printf ("Parsing: %s\n", _S);
    line = dl_get_while();
    if (_S != saved)
        return line;

    line = dl_get_if();
    if (_S != saved)
        return line;

    line = dl_get_var ();
    if (_S != saved)
        return line;

    line = dl_get_call();
    if (_S != saved)
        return line;

    return NULL;
}

char* dl_get_while ()
{
    char* temp = NULL;
    char* body = NULL;

//printf ("W_Parsing: %s\n", _S);
    if (DO_BEGIN(_S, "while("))
    {
        _S += strlen("while(");
        size_t while_n = _WHILE_N++;
        char other[NAME_MAX] = {};
        sprintf (other, "WHILE_%lu:\n", while_n);
        APPEND(body, other);

        temp = getCOMPAR();
        if (!temp || *_S != ')')
            return NULL;
        _S++;
        strmerge(&body, &temp);

        sprintf (other, "push 0\nje WHILE_END_%lu\n", while_n);
        APPEND(body, other);
        temp = dl_get_body ();
        if (!temp)
            return NULL;
        strmerge(&body, &temp);
        sprintf (other, "\njmp WHILE_%lu\nWHILE_END_%lu:\n", while_n, while_n);
        APPEND(body, other);
    }

    //printf ("WHILE returns: %s\n", body);
    return body;
}

char* dl_get_if()
{
    char* temp = NULL;
    char* body = NULL;

    //printf ("W_Parsing: %s\n", _S);
    if (DO_BEGIN(_S, "if("))
    {
        _S += strlen("if(");
        size_t if_n = _IF_N++;
        char other[NAME_MAX] = {};
        sprintf (other, "IF_%lu:\n", if_n);
        APPEND(body, other);

        temp = getCOMPAR();
        if (!temp || *_S != ')')
            return NULL;
        _S++;
        strmerge(&body, &temp);

        sprintf (other, "push 0\nje IF_END_%lu\n", if_n);
        APPEND(body, other);
        temp = dl_get_body ();
        if (!temp)
            return NULL;
        strmerge(&body, &temp);
        sprintf (other, "\nIF_END_%lu:\n", if_n);
        APPEND(body, other);
    }

    return body;
}

char* dl_get_call()
{
    //printf ("C_Parsing: %s\n", _S);
    char* code = NULL;
    char* temp = NULL;

    char* left_end = _S;
    int pos = 0;
    for (; isalpha(*left_end); left_end++, pos++);
    if (*left_end != '(')
        return NULL;

    char left_part[NAME_MAX] = {};
    strncat (left_part, _S, pos);
    _S += pos;
    _S++;
    temp = getCOMPAR();
    if (!temp)
        return NULL;
    if (*_S != ')')
    {
        printf ("ERROR:: ')' is missed\n");
        if (temp)
            free(temp);
        return NULL;
    }
    strmerge(&code, &temp);
    _S++;
    if (*_S != ';')
    {
        printf ("ERROR:: ';' is needed\n");
        if (temp)
            free(temp);
        return NULL;
    }
    _S++;
    if (!strcmp(left_part, "return"))
    {
        APPEND(code, "\nret\n");
    }
    else
    {
        APPEND(code, "\ncall ");
        APPEND(code, left_part);
        APPEND(code, "\n");
    }

    //printf ("CALL returns: %s\n", code);
    return code;
}

char* dl_get_var()
{
    //printf ("V_Parsing: %s\n", _S);
    char* code = NULL;
    char* temp = NULL;

    char* line_end = strchr (_S, ';');
    char* p = _S;
    size_t pos = 0;
    for (; p != line_end && *p != '='; p ++, pos++);
    if (pos == 0 || p == line_end)
        return NULL;
    char left_part[NAME_MAX] = {};
    strncat (left_part, _S, pos);
    //printf ("left_part is %s\n", left_part);
    _S += pos;
    _S++;
    temp = getCOMPAR();
    if (!temp)
        return NULL;
    if (*_S != ';')
    {
        printf ("ERROR:: ';' was expected\n");
        if (temp)
            free(temp);
        return NULL;
    }
    strmerge(&code, &temp);
    APPEND(code, "\npop ");
    APPEND(code, left_part);
    APPEND(code, "\n");
    _S++;
    //printf ("VAR returns: %s\n", code);
    //printf ("AND _S= %c\n", *_S);
    return code;
}

#define CHECK_POINTER(); if (*_S == '\0') return NULL;

char* getNUMBER()
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
        printf ("NUM <%g>\n", val);
        char* code = NULL;
        char num[NAME_MAX] = {};
        sprintf (num, "push %g\n", val);
        char* temp = NULL;
        APPEND(code, num);
        return code;
    }
    else
        return NULL;
}

//CP::= E(['>', '<']T)*
char* getCOMPAR()
{
    char* code = NULL;
    char* left = getSUM();
    char* right = NULL;
    //printf ("LEFT: %s\n", left);
    //printf ("_S: %c\n", *_S);
    while (strchr ("><=", *_S) && *_S != ')' && *_S != ';')
    {
        char op[3] = {};
        op[0] = *_S++;
        //printf ("_S: %c\n", *_S);
        CHECK_POINTER();
        if (strchr ("><=", *_S))
        {
            op[1] = *_S++;
            op[2] = '\0';
        }
        else
        {
            op[1] = '\0';
        }

        right = getSUM();
        strmerge(&code, &left);
        strmerge(&code, &right);
        char* temp;
        #define COMPAR(cmd, compar) \
            if (!strcmp(op, #compar))\
            {\
                APPEND(code, #cmd "\n");\
            }
        #include "compar.h"
        #undef COMPAR
        strmerge (&left, &code);
    }
    //printf ("COMPAR returnes: <%s>\n", left);
    return left;
}

//E::= T(['+', '-']T)*
char* getSUM()
{
    char* code = NULL;
    char* left = getMULT();
    char* right = NULL;
    while (*_S == '-' || *_S == '+')
    {
        char op = *_S++;
        CHECK_POINTER();
        right = getMULT();
        strmerge(&right, &left);
        left = right;
        char* temp = NULL;
        if (op == '+')
        {
            APPEND(code, "add\n");
        }
        else if (op == '-')
        {
            APPEND(code, "sub\n");
        }
        strmerge (&left, &code);
    }

    return left;
}

//T::= P(['*', '/']P)*
char* getMULT()
{
    char* code = NULL;
    char* left = getMIXED();
    char* right = NULL;

    while (*_S == '*' || *_S == '/')
    {
        char op = *(_S++);
        CHECK_POINTER();
        right = getMIXED();
        strmerge(&left, &right);
        char* temp = NULL;
        if (op == '*')
        {
            APPEND(code, "mul\n");
        }
        else if (op == '/')
        {
            APPEND(code, "div\n");
        }
        strmerge (&left, &code);
    }

    return left;
}


//P::= N | B | F
char* getMIXED()
{
    char* saved = _S;
    CHECK_POINTER();
    char* code = getNUMBER();
    if (saved != _S)
        return code;
    //else
    code = getBRACKETS();
    if (saved != _S)
    {
        return code;
    }
    //else
    code = getCHARED();
    //BADABOOM("fu");
    return code;
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

char* getCHARED()
{
    char* name = getNAME();

    if (!name)
        return NULL;
    char* code = getBRACKETS();
    if (code)
    {
        char* temp = NULL;
        //BADABOOM("Here");
        //printf ("name: %s\n", name);
        //tree_node_dump(new_node);
        APPEND(code, "call ");
        strmerge(&code, &name);

        return code;
    }
    //else
    //printf ("VAR <%s>\n", name);
    char* temp = NULL;
    APPEND(code, "push ");
    strmerge(&code, &name);
    APPEND(code, "\n");
    return code;
}

char* getBRACKETS()
{
    if (*_S == '(')
    {
        _S++;
        char* code = getCOMPAR();
        assert (*_S == ')');
        _S++;
        //tree_node_dump(new_node);
        return code;
    }
    else
        return NULL;
}

#endif // PARSING_H_INCLUDED
