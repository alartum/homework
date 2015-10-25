#include <stdio.h>

//#define DEBUG
/// Author name
#define AUTHOR "Alartum"
/// Project name
#define PROJECT "Assembler"
/// Version
#define VERSION "1"

#define COMMAND_LENGTH 5

#include "mylib.h"
#include "CPU.h"
#include <string.h>

int main (int argc, char* argv[])
{
    char inName[64] = {}, outName[64] = {};
    switch (argc)
    {
    case 2:
        if (!strcmp (argv[1], "--help"))
        {
            INFO();
            COMMENT ("Assembler converts the source code into machine code.");
            printf ("The right way to call it:\n");
            printf ("\tAssembler source.asm [program.code]\n");
            printf ("(1)'source.asm' stands for the file with code;\n");
            printf ("(2)'program.code' stands for the output and may not be given.");
            printf ("The result is written to 'program.code' in this case.\n");
            printf ("Other possible keys:\n");
            printf ("\t --help to get help\n");
            printf ("\t --version to get version\n");

            return INFO_CALL;
        }
        if (!strcmp (argv[1], "--version"))
        {
            COMMENT("Version: " VERSION);
            return INFO_CALL;
        }
        strcpy (inName, argv[1]);
        strcpy (outName, "program.code");
        break;
    case 3:
        strcpy (inName, argv[1]);
        strcpy (outName, argv[2]);
        break;
    default:
        COMMENT ("Wrong use. Use --help specifier to get help.");
        return WRONG_USE;
    }

    FILE *in = fopen (inName, "r");
    if (!in)
    {
        perror ("#Source code error");
        return WRONG_RESULT;
    }
    FILE *out = fopen (outName, "w");
    if (!out)
    {
        perror ("#Output error");
        return WRONG_RESULT;
    }

    char command[COMMAND_LENGTH] = {}; // String for each command
    int counter = -1; // Commands counter

    while (EOF != fscanf (in, "%s", command))
    {
        counter ++;
        if (!strcmp (command, "PUSH"))
        {
            char address[COMMAND_LENGTH] = {};
            if (!fscanf (in, "%s", address))
            {
                printf ("#[ERROR] Command #%d: PUSH has wrong argument.\n", counter);
                fprintf (out, "%d ERROR", ERROR);
                fclose (in);
                fclose (out);

                return WRONG_RESULT;
            }
            if (!strcmp (address, "RAX"))
                fprintf (out, "%d %d ", PUSH_RX, RAX);
            else if (!strcmp (address, "RBX"))
                fprintf (out, "%d %d ", PUSH_RX, RBX);
            else if (!strcmp (address, "RCX"))
                fprintf (out, "%d %d ", PUSH_RX, RCX);
            else if (!strcmp (address, "RDX"))
                fprintf (out, "%d %d ", PUSH_RX, RDX);
            else
            {
                int argument = 0;
                if (!sscanf (address, "%d", &argument))
                {
                    printf ("#[ERROR] Command #%d: PUSH has wrong register.\n", counter);
                    fprintf (out, "%d ERROR", ERROR);
                    fclose (in);
                    fclose (out);

                    return WRONG_RESULT;
                }
                fprintf (out, "%d %d ", PUSH, argument);
            }
        }
        else if (!strcmp (command, "POP"))
        {
            fprintf (out, "%d ", POP);
            char address[COMMAND_LENGTH] = {};

            if (!fscanf (in, "%s", address))
            {
                printf ("#[ERROR] Command #%d: POP has wrong argument.\n", counter);
                fprintf (out, "%d ERROR", ERROR);
                fclose (in);
                fclose (out);

                return WRONG_RESULT;
            }
            if (!strcmp (address, "RAX"))
                fprintf (out, "%d ", RAX);
            else if (!strcmp (address, "RBX"))
                fprintf (out, "%d ", RBX);
            else if (!strcmp (address, "RCX"))
                fprintf (out, "%d ", RCX);
            else if (!strcmp (address, "RDX"))
                fprintf (out, "%d ", RDX);
            else
            {
                printf ("#[ERROR] Command #%d: PUSH has wrong register.\n", counter);
                fprintf (out, "%d ERROR", ERROR);
                fclose (in);
                fclose (out);

                return WRONG_RESULT;
            }
        }
        else if (!strcmp (command, "END"))
            fprintf (out, "%d ", END);
        else if (!strcmp (command, "ADD"))
            fprintf (out, "%d ", ADD);
        else if (!strcmp (command, "SUB"))
            fprintf (out, "%d ", SUB);
        else if (!strcmp (command, "MUL"))
            fprintf (out, "%d ", MUL);
        else if (!strcmp (command, "DIV"))
            fprintf (out, "%d ", DIV);
        else if (!strcmp (command, "POW"))
            fprintf (out, "%d ", POW);
        else if (!strcmp (command, "JA"))
        {
            fprintf (out, "%d ", JA);

            int argument = 0;
            if (!fscanf (in, "%d", &argument))
            {
                printf ("#[ERROR] Command #%d: JA has wrong argument.\n", counter);
                fprintf (out, "%d ERROR", ERROR);
                fclose (in);
                fclose (out);

                return WRONG_RESULT;
            }
            fprintf (out, "%d ", argument);
        }
        else if (!strcmp (command, "JMP"))
        {
            fprintf (out, "%d ", JMP);

            int argument = 0;
            if (!fscanf (in, "%d", &argument))
            {
                printf ("#[ERROR] Command #%d: JMP has wrong argument.\n", counter);
                fprintf (out, "%d ERROR", ERROR);
                fclose (in);
                fclose (out);

                return WRONG_RESULT;
            }
            fprintf (out, "%d ", argument);
        }
        else if (!strcmp (command, "OUT"))
            fprintf (out, "%d ", OUT);
        else
        {
            printf ("#[ERROR] Command #%d: unknown command.\n", counter);
            fprintf (out, "%d ERROR", ERROR);
            fclose (in);
            fclose (out);

            return WRONG_RESULT;
        }
    }
    fclose (in);
    fclose (out);
    printf("#Programm successfully written to %s.\n", outName);

    return NO_ERROR;
}
