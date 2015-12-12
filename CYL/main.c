/// Author name
#define AUTHOR "Alartum"
/// Project name
#define PROJECT "CYL(Compute Your Labs)"
/// Version
#define VERSION "1"

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

    Buffer input = {};
    buffer_construct(&input, file_name);
    TreeNode* root = tree_node_from_string(input.chars);
    buffer_destruct(&input);
    if (!root)
        return WRONG_RESULT;
    DifferMap d_map = {};
    if (!differ_map_construct_filename(&d_map, "derivatives.txt"))
        return NULL;

    open_file(tex_file, "laba.tex", "w", "Oh, mega-crap!");
    tex_init(tex_file, "format.tex");

    DifferMap* derivatives = build_all_derivatives(root, true, &d_map, tex_file);
    if (!derivatives)
        return WRONG_RESULT;

    ValMap values = {};
    val_map_construct(&values, 2);
    val_map_add(&values, "x", 0.5);
    val_map_add(&values, "y", 3);

    //TreeNode* result = tree_substitute(derivatives->values[0], &values);
    val_map_destruct(&values);

    //TreeNode* res_smpl = tree_calculate(result, true, tex_file, NULL);

    tree_node_destruct(&root);
    differ_map_destruct(derivatives);
    tex_finish(tex_file);
    close_file(tex_file);
    show_tex_file("laba.tex");
    free (derivatives);
    //send_email("./Mail/header", "./Mail/message", "./Tex/temp.pdf", "Лаба.pdf");

    return NO_ERROR;
}
