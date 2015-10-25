/** @file */

#ifndef MYLIB_H_INCLUDED
#define MYLIB_H_INCLUDED

#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

enum MAIN_ERRORS
{
    NO_ERROR,
    WRONG_RESULT,
    INFO_CALL,
    WRONG_USE
};

/// Definition of boolean type
#define bool int
/// Boolean true
#define true 1
/// Boolean false
#define false 0

#if !defined(AUTHOR) || !defined (PROJECT) || !defined (VERSION)
#error Please, define AUTHOR, PROJECT and VERSION.
#else
#if defined(DEBUG)
/// Standard info message (debug mode)
#define INFO(x) ( printf ("#\t\t[" PROJECT " v." VERSION "] by " AUTHOR "\n#\t\t   ("__TIMESTAMP__ ")\n#\t\t\t    [DEBUG]\n") )
#else
/// Standard info message (default mode)
#define INFO(x) ( printf ("#\t\t[" PROJECT " v." VERSION "] by " AUTHOR "\n#\t\t   ("__TIMESTAMP__ ")\n") )
#endif
#endif
/// Puts a comment in needed way
#define COMMENT(x) ( printf ("#" x "\n") )
/// Useful check for number is it 0
#define IS_ZERO(x) ( fabs (x) < DBL_EPSILON )
/// Returns the minimum of two numbers
#define MIN(x, y) (((x) < (y))? (x) : (y))
/// Returns the maximum of two numbers
#define MAX(x, y) (((x) < (y))? (y) : (x))

#if defined(DEBUG)
/**
*@brief Assertion of structures of different types.
*
*Let "type" be the structure name. For correct work of this macro you need:
*
*(1) void type_dump (type* what) | provides standard output for the structure;
*
*(2) bool type_OK (type* what) | provides validation of structure, returns true if the structure is OK, false otherwise.
*/
#define ASSERT_OK(type, what)\
    if (!type ## _OK(what))\
    {\
        printf ("(!)ERROR: " #type " is broken!\nFunction: %s\nFile: %s\nLine: %d\n\n[LOG]\n\n", __FUNCTION__, __FILE__, __LINE__);\
        type ## _dump(what);\
        abort();\
    }
#define DPRINT_int(x) ( PrintDebug_int ((x), #x) )
#define DPRINT_size_t(x) ( PrintDebug_size_t ((x), #x) )
#define DPRINT_float(x) ( PrintDebug_float((x), #x) )
#define DPRINT_double(x) ( PrintDebug_double ((x), #x) )
#define DPRINT_char(x) ( PrintDebug_char ((x), #x) )
#else
#define ASSERT_OK(type, what) ;
#define DPRINT_int(x) ;
#define DPRINT_size_t(x) ;
#define DPRINT_float(x) ;
#define DPRINT_double(x) ;
#define DPRINT_char(x) ;

#endif // DEBUG

/// Amount of tries are given to input a value
const int TRY_AMOUNT = 3;

/// Macro for automated input function generation
#define GENERATE_INPUT(TYPE, SPEC) int DoInput_ ## TYPE (TYPE *var, const char name[])\
    {\
        assert (var);\
        printf ("#%s>", name);\
        \
        if (scanf (SPEC, var) == 1)\
            return 1;\
        else\
        {\
            printf ("#Invalid input\n");\
            errno = EIO;\
        }\
        \
        for (int i = 0; i < TRY_AMOUNT - 1; ++i)\
        {\
            printf ("#%s>", name);\
            if (scanf ("%*s" SPEC, var) == 1)\
                return 1;\
            else\
            {\
                printf ("#Invalid input\n");\
                errno = EIO;\
            }\
        }\
        \
        perror ("#Input error");\
        return 0;\
    }

/// Generated input functions
GENERATE_INPUT(int, "%d")
GENERATE_INPUT(size_t, "%lu")
GENERATE_INPUT(float, "%f")
GENERATE_INPUT(double, "%lf")
GENERATE_INPUT(char, "%c")

#define INPUT_int(x)   ( DoInput_int (&(x), #x) )
#define INPUT_size_t(x)   ( DoInput_size_t (&(x), #x) )
#define INPUT_float(x)   ( DoInput_float (&(x), #x) )
#define INPUT_double(x)   ( DoInput_double(&(x), #x) )
#define INPUT_char(x)   ( DoInput_char(&(x), #x) )

/// Macro for automated debug print function generation
#define GENERATE_PRINT_DEBUG(TYPE, SPEC) void PrintDebug_ ## TYPE (TYPE var, const char name[])\
    {\
        printf ("(?)%s = " SPEC "\n", name, var);\
    }

GENERATE_PRINT_DEBUG(int, "%d")
GENERATE_PRINT_DEBUG(size_t, "%lu")
GENERATE_PRINT_DEBUG(float, "%f")
GENERATE_PRINT_DEBUG(double, "%lf")
GENERATE_PRINT_DEBUG(char, "%c")

#endif // MYLIB_H_INCLUDED
