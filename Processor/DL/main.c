/// Author name
#define AUTHOR "Alartum"
/// Project name
#define PROJECT "DumbLang"
/// Version
#define VERSION "1"

#include <stdio.h>
#include "mylib.h"
#include <string.h>
#include <limits.h>
#include "parsing.h"

int main (int argc, char* argv[])
{
    CHECK_DEFAULT_ARGS();
    char in_name[NAME_MAX] = {}, out_name[NAME_MAX] = {};
    switch (argc)
    {
    case 3:
        strcpy (in_name, argv[1]);
        strcpy (out_name, argv[2]);
        break;
    case 2:
        strcpy (in_name, argv[1]);
        strcpy (out_name, "program.asm");
        break;
    default:
        WRITE_WRONG_USE();
    }

    Buffer program;
    buffer_construct (&program, in_name);
    if (program.length == 0)
    {
        printf ("#ERROR::The program is empty!\n");
        return WRONG_USE;
    }
    char* compiled = dl_parse(program.chars);
    open_file(compiled_file, out_name, "w", "I'm too tired, can't do this");
    fprintf (compiled_file, "%s", compiled);
    close_file(compiled_file);
    buffer_destruct (&program);

    return NO_ERROR;
}
