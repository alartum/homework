#include <stdio.h>


//#define DEBUG
/// Author name
#define AUTHOR "Alartum"
/// Project name
#define PROJECT "StackProcessor"
/// Version
#define VERSION "2.1"

#include "mylib.h"
#include "CPU.h"
#include <string.h>

int main (int argc, char* argv[])
{
    char programName[64] = {};
    switch (argc)
    {
    case 2:
        if (!strcmp (argv[1], "--help"))
        {
            INFO();
            COMMENT ("This processor executes the program from the file");
            COMMENT ("The right way to call it:");
            COMMENT ("StackProcessor filename.prog");
            COMMENT ("Where 'filename.prog' stands for the file with program.");
            return INFO_CALL;
        }
        if (!strcmp (argv[1], "--version"))
        {
            COMMENT("Version: " VERSION);
            return INFO_CALL;
        }
        strcpy (programName, argv[1]);
        break;
    default:
        COMMENT ("Wrong use. Use --help specifier to get help.");
        return WRONG_USE;
    }

    FILE *fProgram = fopen (programName, "r");
    if (!fProgram)
    {
        perror ("#Program error");
        return WRONG_RESULT;
    }

    int program[MAX_PROGRAM_LENGTH] = {};
    int success = 0;
    for (int i = 0; EOF != (success = fscanf (fProgram, "%d", &program[i])); i++)
        if (!success)
        {
            COMMENT ("Error! The file is not executable as it containes something except code.");
            fclose (fProgram);

            return WRONG_RESULT;
        }

    fclose (fProgram);
    CPU cpu = {};
    cpu_construct(&cpu);
    if (!cpu_execute(&cpu, program))
    {
        cpu_destruct(&cpu);
        COMMENT ("Runtime error occured!");

        return WRONG_RESULT;
    }
    cpu_destruct(&cpu);

    return NO_ERROR;
}
