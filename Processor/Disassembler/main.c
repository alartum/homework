/// Author name
#define AUTHOR "Alartum"
/// Project name
#define PROJECT "Disassembler"
/// Version
#define VERSION "1"

#include <stdio.h>
#include "mylib.h"
#include "exit_codes.h"
#include <string.h>
#include <limits.h>

#define DEFINES_ONLY
#include "commands.h"
#undef DEFINES_ONLY

enum PARSING_STATE
{
    CMD,
    ARG,
    DONE
};

int main (int argc, char* argv[])
{
    CHECK_DEFAULT_ARGS();
    char inName[NAME_MAX] = {}, outName[NAME_MAX] = {};
    switch (argc)
    {
    case 3:
        strcpy (inName, argv[1]);
        strcpy (outName, argv[2]);
        break;
    case 2:
        strcpy (inName, argv[1]);
        strcpy (outName, "source.asm");
        break;
    default:
        WRITE_WRONG_USE();
    }

    Buffer assembled = {};
    if (!buffer_construct (&assembled, inName))
    {
        perror ("#Input error");
        return WRONG_RESULT;
    }

    Buffer source;
    buffer_construct_empty (&source, 10*assembled.length);

    char state = CMD, arg_type = -1;
    int reading_pos = 0;
    while (reading_pos <= assembled.length)
    {
        switch (state)
        {
        case CMD:
            ;// Just very strange behavior
            char code = assembled.chars[reading_pos];
            reading_pos++;
            #define CMD(name, key, shift, arguments) \
            if (!(arguments & ARG_OVL)) \
            {\
                if (state == CMD && code == key)\
                {\
                    printf ("%s ", #name);\
                    strcat (source.chars, #name " ");\
                    if (arguments & ARG_NO)\
                    {\
                        state = DONE;\
                    }\
                    else \
                    {\
                        state = ARG;\
                        arg_type = arguments;\
                    }\
                }\
                else if (arguments & ARG_REG && code == key + 1)\
                {\
                    printf (#name " ");\
                    strcat (source.chars, #name " ");\
                    state = ARG;\
                    arg_type = ARG_REG;\
                }\
                else if (arguments & ARG_MEM && code == key + 2)\
                {\
                    printf (#name " ");\
                    strcat (source.chars, #name " ");\
                    state = ARG;\
                    arg_type = ARG_MEM;\
                }\
            }
            #include "commands.h"
            #undef CMD
            if (state == CMD)
            {
                printf ("Code is corrupted\n");
                return WRONG_RESULT;
            }

            break;
        case ARG:
            if ((state != DONE) && (arg_type & ARG_NUM))
            {
                float num = *(float*)(assembled.chars+reading_pos);

                char *temp = (char*)calloc (128, sizeof(char));
                temp[0] = '\0';
                sprintf (temp, "%g", num);
                strcat (source.chars, temp);
                free (temp);

                printf ("(%g)", num);
                reading_pos += sizeof(float);
                state = DONE;
            }
            if ((state != DONE) && (arg_type & ARG_MEM))
            {
                unsigned mem = *(unsigned*)(assembled.chars + reading_pos);

                char *temp = (char*)calloc (128, sizeof(char));
                temp[0] = '\0';
                sprintf (temp, "[%u]", mem);
                strcat (source.chars, temp);
                free (temp);

                printf ("[%u]", mem);
                // Changing command identifier to mem
                reading_pos += sizeof(unsigned);
                state = DONE;
            }
            if ((state != DONE) && (arg_type & ARG_REG))
            {
                char reg = assembled.chars[reading_pos];
                reading_pos++;
                #define ADDRESS(name, address) \
                if (state != DONE && reg == address)\
                {\
                    /* Changing command identifier to reg*/\
                    char *temp = (char*)calloc (128, sizeof(char));\
                    temp[0] = '\0';\
                    sprintf (temp, #name);\
                    strcat (source.chars, temp);\
                    free (temp);\
                    printf ("%s", #name);\
                    state = DONE;\
                }
                #include "reg_address.h"
                #undef ADDRESS
            }
            if ((state != DONE) && (arg_type & ARG_POS))
            {
                int pos = *(int*)(assembled.chars+reading_pos);

                char *temp = (char*)calloc (128, sizeof(char));
                temp[0] = '\0';
                sprintf (temp, "%d", pos);
                strcat (source.chars, temp);
                free (temp);

                printf ("{%d}", pos);
                reading_pos += sizeof(int);
                state = DONE;
            }

            if (state != DONE)
            {
                printf ("\nCommand is corrupted\n");
                buffer_destruct(&assembled);
                return WRONG_RESULT;
            }
            break;
        }

        //^^^^^^^^^^^^^^^^^^^^^^^^^
        //Words parsing END
        //^^^^^^^^^^^^^^^^^^^^^^^^^
        if (state == DONE)
        {
            printf ("\n");
            strcat (source.chars, "\n");
            state = CMD;
        }
    }

    buffer_destruct(&assembled);
    open_file (out, outName, "wb", "#Output error");
    fprintf (out, "%s", source.chars);
    buffer_destruct(&source);
    close_file (out);
    printf("#Programm successfully written to %s.\n", outName);
    //^^^^^^^^^^^^^^^^^^^^^^^^^
    //Output END
    //^^^^^^^^^^^^^^^^^^^^^^^^^
    return NO_ERROR;
}
