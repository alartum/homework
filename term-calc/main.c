#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

// Pointer to the current symbol
char* s = NULL;
char state = 1;
#define MAX_WORD 256

//CHARED::= NAME+BRACKETS | NAME
int getCHARED();
//MIXED::= CHARED | BRACKETS | NUMBER
int getMIXED();
//NAME::=['A'-'Z', 'a'-'z']+['0'-'9']*
int getNAME();
//NUMBER::=['0'-'9']
int getNUMBER();
//SUM::= MULT(['+', '-']MULT)*
int getSUM();
//MULT::= POW(['*', '/']POW)*
int getMULT();
//BRACKETS::= '('SUM')'
int getBRACKETS();
//POW::= MIXED([^]MIXED)*
int getPOW();
//Control sequence
int getG0(const char* str);

int main (int argc, char* argv[])
{
    if (argc != 2)
    {
        printf ("Too many arguments\n");
        return -1;
    }
    const char* sequence = argv[1];
    printf ("%s = %d\n", sequence, getG0 (sequence));

    return 0;
}

int getNUMBER()
{
    int val = 0;
    char* saved = s;
    while (isdigit(*s))
    {
        val = 10*val + *s - '0';
        s++;
    }
    if (saved != s)
        printf ("NUMBER  (%d)\n", val);
    return val;
}

//E::= T(['+', '-']T)*
int getSUM()
{
    int val = getMULT();
    while (*s == '-' || *s == '+')
    {
        char op = *s++;
        int val2 = getMULT();
        if (op == '+')
        {
            printf ("SUM  (%d)+(%d)\n", val, val2);
            val += val2;
        }
        else if (op == '-')
        {
            printf ("SUM  (%d)-(%d)\n", val, val2);
            val -= val2;
        }
    }

    return val;
}

//T::= P(['*', '/']P)*
int getMULT()
{
    int val = getPOW();
    while (*s == '*' || *s == '/')
    {
        char op = *s++;
        int val2 = getPOW();
        if (op == '*')
        {
            printf ("MULT  (%d)*(%d)\n", val, val2);
            val *= val2;
        }
        else if (op == '/')
        {
            printf ("MULT  (%d)/(%d)\n", val, val2);
            val /= val2;
        }
    }

    return val;
}

//Control sequence
int getG0(const char* str)
{
    s = (char*)str;
    int val = getSUM();
    assert (*s == 0);

    return val;
}

//P::= N | B | F
int getMIXED()
{
    char* saved = s;

    int val = getNUMBER();
    if (saved != s)
        return val;

    val = getBRACKETS();
    if (saved != s)
        return val;

    return getCHARED();
}

int getNAME()
{
    unsigned current = 0;
    char word[MAX_WORD] = {};
    while (isalpha(*s) || *s == '_')
    {
        word[current] = *s;
        s++;
        current++;
    }
    while (isdigit(*s))
    {
        word[current] = *s;
        s++;
        current++;
    }
    word[current] = '\0';
    printf ("NAME  \"%s\"\n", word);

    return 0;
}

int getCHARED()
{
    char* saved = s;

    getNAME();
    if (saved == s)
        return 0;

    saved = s;
    int value = getBRACKETS();
    if (saved != s)
    {
        printf ("CHARED function\n");

        return value;
    }

    printf ("CHARED variable\n");

    return 0;
}

int getBRACKETS()
{
    if (*s == '(')
    {
        s++;
        int val = getSUM();
        assert (*s == ')');
        s++;

        return val;
    }
    else
        return 0;
}

int getPOW()
{
    int val = getMIXED();
    while (*s == '^')
    {
        s++;
        int val2 = getMIXED();
        printf ("POW  (%d)^(%d)\n", val, val2);
        val = (int)pow ((double)val, (double)val2);
    }

    return val;
}
