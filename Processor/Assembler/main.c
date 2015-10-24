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
    int number = 0, nSuccess = 0;
    while (EOF != (nSuccess = fscanf (in, "%d", &number)))
    {
        if (nSuccess != 1)
        {
            char command[COMMAND_LENGTH] = {};
            fscanf (in, "%s", command);
            if (!strcmp (command, "PUSH"))
            {
                char address[COMMAND_LENGTH] = {};
                fscanf (in, "%s", address);
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
                    number = 0;
                    sscanf (address, "%d", &number);
                    fprintf (out, "%d %d ", PUSH, number);
                }
            }
            else if (!strcmp (command, "POP"))
            {
                fprintf (out, "%d ", POP);
                char address[COMMAND_LENGTH] = {};

                fscanf (in, "%s", address);
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
                    printf ("#Error! Wrong register to POP: %s\n", address);
                    fprintf (out, "%d", ERROR);
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
                fprintf (out, "%d ", JA);
            else if (!strcmp (command, "JMP"))
                fprintf (out, "%d ", JMP);
            else if (!strcmp (command, "OUT"))
                fprintf (out, "%d ", OUT);
            else
            {
                printf ("#Error! Wrong command: %s\n", command);
                fprintf (out, "ERROR");
                fclose (in);
                fclose (out);

                return WRONG_RESULT;
            }
        }
        else
            fprintf (out, "%d ", number);
    }
    fclose (in);
    fclose (out);
    printf("#Programm successfully written to %s.\n", outName);

    return NO_ERROR;
}
