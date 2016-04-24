#ifndef TEX_H_INCLUDED
#define TEX_H_INCLUDED

#include "buffer.h"
#include "mylib.h"
#include <stdio.h>

bool tex_init (FILE* tex, const char format_file[])
{
    Buffer format = {};
    if (!buffer_construct(&format, format_file))
        return false;
    fprintf (tex, "%s\n\n \\begin{document}\n\n", format.chars);
    buffer_destruct(&format);

    return true;
}

bool tex_finish (FILE* tex)
{
    return fprintf (tex, "\\end{document}\n");
}

void show_tex_file (const char filename[])
{
    assert (filename);
    /*char* last_slash = strrchr (filename, '/');
    if (!last_slash)
        last_slash = filename;
    else
        last_slash++;
    char* file = strdup (last_slash);*/
    char cmd[128] = {};
    strcat (cmd, "pdflatex -output-directory=./Tex -interaction=batchmode -jobname=\"show_tex_file\" ");

    strcat (cmd, filename);
    system (cmd);
    system ("qpdfview ./Tex/show_tex_file.pdf");
    //free (file);
}

//Last string in null-string so we don't need to care about the amount
char** comments_get (const char filename[])
{
    #define ERROR_EXIT() \
    {\
        buffer_destruct(&text);\
        return NULL;\
    }

    Buffer text = {};
    if (!buffer_construct(&text, filename))
        return NULL;

    int n_amount = 0;
    for (char* p = text.chars; *p; p++)
        if (*p == '\n')
            n_amount++;
    if (n_amount == 0)
        ERROR_EXIT();
    char** comments = (char**)calloc (n_amount + 1, sizeof(*comments));
    if (!comments)
        ERROR_EXIT();

    char* newline = strtok (text.chars, "\n");
    int current = 0;
    for (;newline; newline = strtok(NULL, "\n"), current++)
        comments[current] = strdup (newline);
    comments[current] = NULL;
    buffer_destruct(&text);

    #undef ERROR_EXIT
    return comments;
}
#endif // TEX_H_INCLUDED
