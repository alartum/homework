//#define DEBUG
/// Author name
#define AUTHOR "Alartum"
/// Project name
#define PROJECT "Assembler"
/// Version
#define VERSION "1"

#include <stdio.h>
#include "mylib.h"
#include "commands_enum.h"
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

typedef struct
{
    char* name;
    int position; //Position in bytes
    char counter;
} Label;

size_t assemble (const Buffer* source, Buffer* assembled, Label* labels);

int main (int argc, char* argv[])
{
    //^^^^^^^^^^^^^^^^^^^^^^^^^
    //Default part BEGIN
    //^^^^^^^^^^^^^^^^^^^^^^^^^
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
        strcpy (outName, "program.bin");
        break;
    default:
        WRITE_WRONG_USE();
    }

    //^^^^^^^^^^^^^^^^^^^^^^^^^
    //Default part END
    //Buffer BEGIN
    //^^^^^^^^^^^^^^^^^^^^^^^^^
    Buffer buffer = {};
    if (!buffer_construct (&buffer, inName))
    {
        perror ("#Input error");
        return WRONG_RESULT;
    }
    Buffer assembled;
    buffer_construct_empty (&assembled, buffer.length);


    int charcount = 1; // 1 because we need a stop label, so we need one more of them
    for(int m=0; buffer.chars[m]; m++)
        if(buffer.chars[m] == ':')
            charcount ++;
    Label* labels = (Label*) calloc (charcount, sizeof (*labels));
    for (int i = 0; i < charcount; i ++)
    {
        labels[i].counter = 0;
        labels[i].position = -1;
        labels[i].name = NULL;
    }

    printf ("FIRST RUN:\n");
    assemble(&buffer, &assembled, labels);
    printf ("SECOND RUN:\n");
    size_t writing_pos = assemble(&buffer, &assembled, labels);
    if (!writing_pos)
    {
        printf ("Assemble error\n");
        open_file (out, outName, "wb", "#Output error");
        fwrite (assembled.chars, sizeof (char), assembled.length, out);
        buffer_destruct(&assembled);
        close_file (out);
        return WRONG_RESULT;
    }

    printf ("Labels:\n");
    for (int i = 0; labels[i].position != -1; i ++)
    {
        printf ("[%s] = %d;\n", labels[i].name, labels[i].position);
        free(labels[i].name);
    }
    //^^^^^^^^^^^^^^^^^^^^^^^^^
    //Parse END
    //Output BEGIN
    //^^^^^^^^^^^^^^^^^^^^^^^^^
    free (labels);
    buffer_destruct(&buffer);
    printf ("AMOUNT: %lu\n", writing_pos);
    for (size_t i = 0; i < writing_pos; i++)
    {
        printf ("%d ", assembled.chars[i]);
    }

    open_file (out, outName, "wb", "#Output error");
    fwrite (assembled.chars, sizeof (char), writing_pos, out);
    buffer_destruct(&assembled);
    close_file (out);
    printf("\n#Programm successfully written to %s.\n", outName);
    //^^^^^^^^^^^^^^^^^^^^^^^^^
    //Output END
    //^^^^^^^^^^^^^^^^^^^^^^^^^
    return NO_ERROR;
}




size_t assemble(const Buffer* source, Buffer* assembled, Label* labels)
{
    Buffer buffer = {};
    buffer_construct_copy (&buffer, source);

    char* nextLinePtr = buffer.chars;
    // Current line number and writing byte
    size_t lineN = 0, writing_pos = 0;

    //^^^^^^^^^^^^^^^^^^^^^^^^^
    //Buffer END
    //Parse BEGIN
    //^^^^^^^^^^^^^^^^^^^^^^^^^

    //Reading state
    char state = DONE;
    bool labels_success = true;
    while (nextLinePtr != NULL && state == DONE)
    {
        //^^^^^^^^^^^^^^^^^^^^^^^^^
        //Lines marking BEGIN
        //^^^^^^^^^^^^^^^^^^^^^^^^^

        printf ("#%lu# ", lineN);
        lineN++;

        while (strchr(" \t", *nextLinePtr))
            nextLinePtr++;
        if (strchr("\r\n", *nextLinePtr))
        {
            printf ("(skipped)\n");
            nextLinePtr++;
            continue;
        }

        char* line = nextLinePtr;
        nextLinePtr = strpbrk (nextLinePtr, "\n\r");
        if (nextLinePtr)
        {
            // Need to put \0 for strtok to work correctly
            *nextLinePtr = '\0';
            nextLinePtr++;
        }

        //^^^^^^^^^^^^^^^^^^^^^^^^^
        //Lines marking END
        //Words parsing BEGIN
        //^^^^^^^^^^^^^^^^^^^^^^^^^

        char* word = strtok (line, " ");
        // State is what we currently expect to read
        int arg_type = -1;
        state = CMD;

        while (word != NULL)
        {
            switch (state)
            {
            case DONE:
                if (sscanf(word, ";%*s"))
                {
                    printf (" $comment$ ");
                    state = DONE;
                    word = NULL;
                    continue;
                }
                else
                {
                    printf ("Parsing error\n");
                    return 0;
                }
                break;
            case CMD:
                if (state != DONE && sscanf(word, ";%*s"))
                {
                    printf (" $comment$ ");
                    state = DONE;
                    word = NULL;
                    continue;
                }
                if ((state != DONE) && (!strcmp (word, "end")))
                {
                    printf ("end\n");
                    assembled->chars[writing_pos] = (char)cmd_end;
                    writing_pos ++;
                    assembled->chars[writing_pos] = 0;
                    buffer_destruct(&buffer);

                    return writing_pos;
                }
                #define CMD(name, key, shift, arguments) \
                if (!(arguments & ARG_OVL)) \
                {\
                    if ((state == CMD) && (!strcmp (word, #name)))\
                    {\
                        printf ("%s ", #name);\
                        assembled->chars[writing_pos] = (char)key;\
                        writing_pos ++;\
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
                }
                #include "commands.h"
                #undef CMD
                char* dots_ptr = strchr (word, ':');
                if (dots_ptr)
                {
                    *dots_ptr = '\0';
                    state = DONE;
                    int i = 0;
                    for (; labels[i].position != -1; i++)
                    {
                        if (!strcmp (labels[i].name, word))
                        {
                            if (labels[i].counter < 2)
                            {
                                labels[i].counter ++;
                                break;
                            }
                            else
                            {
                                printf ("Multiple label definition: %s\n", word);
                                buffer_destruct(&buffer);
                                return 0;
                            }
                        }
                    }
                    if (labels[i].position == -1)
                    {
                        labels[i].name = strdup (word);
                        labels[i].counter ++;
                        labels[i].position = writing_pos;
                        printf (" ::%s:: (%lu)", word, writing_pos);
                    }
                    state = DONE;
                }
                if (state == CMD)
                {
                    printf ("\nUnknown command in line #%lu: %s\n", lineN, word);
                    buffer_destruct(&buffer);
                    assembled->chars[writing_pos] = (char)cmd_err;
                    return 0;
                }

                break;
            case ARG:
                //printf ("Parsing: <%s> ", word);
                if ((state != DONE) && (arg_type & ARG_POS))
                {
                    int pos = 0;
                    if (sscanf (word, "%d", &pos))
                    {
                        printf ("{%d}", pos);
                        *(int*)(assembled->chars+writing_pos) = pos;
                        writing_pos += sizeof(int);
                        state = DONE;
                    }
                }
                if ((state != DONE) && (arg_type & ARG_NUM))
                {
                    float num = 0;
                    if (sscanf (word, "%f", &num))
                    {
                        printf ("(%f)", num);
                        *(float*)(assembled->chars+writing_pos) = num;
                        writing_pos += sizeof(float);
                        state = DONE;
                    }
                }
                if ((state != DONE) && (arg_type & ARG_MEM))
                {
                    unsigned mem = 0;
                    if (sscanf (word, "[%u]", &mem))
                    {
                        printf ("[%u]", mem);
                        // Changing command identifier to mem
                        assembled->chars[writing_pos - 1] += 2;
                        *(unsigned*)(assembled->chars + writing_pos) = mem;
                        writing_pos += sizeof(unsigned);
                        state = DONE;
                    }
                }
                if ((state != DONE) && (arg_type & ARG_REG))
                {
                    //printf ("[Wr_pos: %s]", word);
                    #define ADDRESS(name, address) \
                    if ((state != DONE) && (!strcmp (word, #name)))\
                    {\
                        /* Changing command identifier to reg*/\
                        printf ("[%s]|%d|", word, address);\
                        assembled->chars[writing_pos - 1] += 1 ;\
                        assembled->chars[writing_pos] = (char)address;\
                        writing_pos ++;\
                        state = DONE;\
                    }
                    #include "reg_address.h"
                    #undef ADDRESS
                }
                if ((state != DONE) && (arg_type & ARG_LBL))
                {
                    for (int i = 0; labels[i].position != -1 && state != DONE; i++)
                    {
                        if (!strcmp (labels[i].name, word))
                        {
                            printf ("[%s]{%d}", word, labels[i].position);
                            *(int*)(assembled->chars+writing_pos) = labels[i].position;
                            writing_pos += sizeof(int);
                            state = DONE;
                        }
                    }
                    if (state != DONE)
                    {
                        *(int*)(assembled->chars+writing_pos) = -1;
                        writing_pos += sizeof(int);
                    }
                    labels_success = false;
                    state = DONE;// Just to be sure

                }
                if (state != DONE)
                {
                    printf ("\nUnknown command in line #%lu: %s\n", lineN, word);
                    buffer_destruct(&buffer);
                    assembled->chars[writing_pos] = (char)cmd_err;
                    return 0;
                }
                break;
            }

            // Take next word
            word = strtok (NULL, " ");
        }

        //^^^^^^^^^^^^^^^^^^^^^^^^^
        //Words parsing END
        //^^^^^^^^^^^^^^^^^^^^^^^^^
        printf ("\n");
    }
    buffer_destruct(&buffer);
    return labels_success && writing_pos;
}
