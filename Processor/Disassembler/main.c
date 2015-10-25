#include <stdio.h>

//#define DEBUG
/// Author name
#define AUTHOR "Alartum"
/// Project name
#define PROJECT "Disassembler"
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
            COMMENT ("Disassembler converts the machine code back to source code.");
            printf ("The right way to call it:\n");
            printf ("\tDisassembler program.code [source.asm]\n");
            printf ("(1)'program.code' stands for the output.\n");
            printf ("(2)'source.asm' stands for the file with code and may not be given;\n");
            printf ("The result is written to 'source.asm' in this case.\n");
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
        strcpy (outName, "source.asm");
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
        perror ("#Program code error");
        return WRONG_RESULT;
    }
    FILE *out = fopen (outName, "w");
    if (!out)
    {
        perror ("#Output error");
        return WRONG_RESULT;
    }

    int code = 0;

    while (EOF != fscanf (in, "%d", &code))
    {
        DPRINT_int(code);
        switch (code)
        {
        case PUSH:
            fprintf (out, "PUSH ");
            fscanf (in, "%d", &code);
            fprintf (out, "%d\n", code);
            break;
        case PUSH_RX:
            fprintf (out, "PUSH ");
            fscanf (in, "%d", &code);

            switch (code)
            {
            case RAX:
                fprintf (out, "RAX\n");
                break;
            case RBX:
                fprintf (out, "RBX\n");
                break;
            case RCX:
                fprintf (out, "RCX\n");
                break;
            case RDX:
                fprintf (out, "RDX\n");
                break;
            default:
                printf ("#Error! Wrong register to PUSH: %d\n", code);
                fprintf (out, "ERROR");
                fclose (in);
                fclose (out);

                return WRONG_RESULT;
            }
            break;
        case POP:
            fprintf (out, "POP ");
            fscanf (in, "%d", &code);
            switch (code)
            {
            case RAX:
                fprintf (out, "RAX\n");
                break;
            case RBX:
                fprintf (out, "RBX\n");
                break;
            case RCX:
                fprintf (out, "RCX\n");
                break;
            case RDX:
                fprintf (out, "RDX\n");
                break;
            default:
                printf ("#Error! Wrong register to POP: %d\n", code);
                fprintf (out, "ERROR");
                fclose (in);
                fclose (out);

                return WRONG_RESULT;
            }
            break;
        case END:
            fprintf (out, "END\n");
            break;
        case ADD:
            fprintf (out, "ADD\n");
            break;
        case SUB:
            fprintf (out, "SUB\n");
            break;
        case MUL:
            fprintf (out, "MUL\n");
            break;
        case DIV:
            fprintf (out, "DIV\n");
            break;
        case POW:
            fprintf (out, "POW\n ");
            break;
        case JA:
            fprintf (out, "JA ");
            fscanf (in, "%d", &code);
            fprintf (out, "%d\n", code);
            break;
        case JMP:
            fprintf (out, "JMP ");
            fscanf (in, "%d", &code);
            fprintf (out, "%d\n", code);
            break;
        case OUT:
            fprintf (out, "OUT\n");
            break;
        case ERROR:
            printf ("#Error! The code is corrupted.");
            fprintf (out, "ERROR");

            return WRONG_RESULT;
        default:
            printf ("#Error! Wrong code %d\n", code);
            fprintf (out, "ERROR");

            return WRONG_RESULT;
        }
    }
    fclose (in);
    fclose (out);
    printf("#Code sucessfully written to %s.\n", outName);

    return NO_ERROR;
}
