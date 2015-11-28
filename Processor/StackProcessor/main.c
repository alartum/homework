/// Author name
#define AUTHOR "Alartum"
/// Project name
#define PROJECT "StackProcessor"
/// Version
#define VERSION "2.1"

#include <stdio.h>
#include "mylib.h"
#include <string.h>
#include <limits.h>
#include "CPU.h"

int main (int argc, char* argv[])
{
    CHECK_DEFAULT_ARGS();
    char programName[NAME_MAX] = {};
    switch (argc)
    {
    case 2:
        strcpy (programName, argv[1]);
        break;
    default:
        WRITE_WRONG_USE();
    }

    Buffer program;
    buffer_construct (&program, programName);
    CPU cpu = {};
    cpu_construct(&cpu);
    cpu_set_program(&cpu, &program);
    if (!cpu_execute(&cpu))
    {
        cpu_destruct(&cpu);
        COMMENT ("Runtime error occured!");

        return WRONG_RESULT;
    }
    cpu_destruct (&cpu);
    buffer_destruct (&program);

    return NO_ERROR;
}
