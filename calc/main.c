#include <stdio.h>
#include <assert.h>

// Pointer to the current symbol
char* s = NULL;
char state = 1;

//P::= '('E')' | N
int getP();
//N::=['0'-'9']*
int getN();
//E::= T(['+', '-']T)*
int getE();
//T::= P(['*', '/']P)*
int getT();
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



//N::=['0'-'9']*
int getN()
{
    int val = 0;
    while ('0' <= *s && *s <= '9')
    {
        val = 10*val + *s - '0';
        s++;
    }

    return val;
}

//E::= T(['+', '-']T)*
int getE()
{
    int val = getT();
    while (*s == '-' || *s == '+')
    {
        char op = *s++;
        int val2 = getT();
        if (op == '+')
            val += val2;
        else if (op == '-')
            val -= val2;
    }

    return val;
}

//T::= P(['*', '/']P)*
int getT()
{
    int val = getP();
    while (*s == '*' || *s == '/')
    {
        char op = *s++;
        int val2 = getP();
        if (op == '*')
            val *= val2;
        else if (op == '/')
            val /= val2;
    }

    return val;
}

//Control sequence
int getG0(const char* str)
{
    s = (char*)str;
    int val = getE();
    assert (*s == 0);

    return val;
}

//P::= '('E')' | N
int getP()
{
    if (*s == '(')
    {
        s++;
        int val = getE();
        assert (*s == ')');
        s++;

        return val;
    }
    else
        return getN();
}
