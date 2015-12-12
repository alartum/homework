/// Author name
#define AUTHOR "Alartum"
/// Project name
#define PROJECT "CYL(Compute Your Labs)"
/// Version
#define VERSION "1"

//#define DEBUG

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "tex.h"
#include "mylib.h"
#include "buffer.h"
#include <limits.h>
#include "tree.h"
#include "parsing.h"
#include "differ.h"
#include "mail.h"

int main (int argc, char* argv[])
{
    //////////////////////////
    ////Default part BEGIN////
    //////////////////////////
    char file_name[NAME_MAX];
    CHECK_DEFAULT_ARGS();
    switch (argc)
    {
    case 1:
        strcpy (file_name, "formula.txt");
        break;
    case 2:
        strcpy (file_name, argv[1]);
        break;
    default:
        WRITE_WRONG_USE();
    }
    //DANGER! RANDOM. KEEP AWAY.
    srand (time(NULL));
    //DANGER! RANDOM. KEEP AWAY.

    open_file(info_file, "lab_info", "r", "Something is wrong!");
    open_file(tex_file, "laba.tex", "w", "Something is wrong!");
    write_lab(info_file, tex_file);
    close_file(tex_file);
    close_file(info_file);

    send_email("./Mail/header", "./Mail/message", "./Tex/show_tex_file.pdf", "Лаба.pdf");
    show_tex_file("laba.tex");

    return NO_ERROR;
}
