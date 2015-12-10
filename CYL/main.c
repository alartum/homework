/// Author name
#define AUTHOR "Alartum"
/// Project name
#define PROJECT "CYL(Compute Your Labs)"
/// Version
#define VERSION "1"

#include <stdio.h>
#include <string.h>
#include "mylib.h"
#include "buffer.h"
#include <limits.h>
#include "tree.h"
#include "parsing.h"
#include "differ.h"
#include "mail.h"

int main (int argc, char* argv[])
{
    //^^^^^^^^^^^^^^^^^^^^^^^^^
    //Default part BEGIN
    //^^^^^^^^^^^^^^^^^^^^^^^^^
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
    //float num = 1.5;
    //TreeNode* root = _ADD (_DIV(_VAR("y", NULL, NULL),_VAR("x", NULL, NULL)), _FU("arctg",_NUM(&num, NULL, NULL), NULL));
    Buffer input = {};
    buffer_construct(&input, file_name);
    //const char input[] = "sin(4/x)-tg(2.5*x+3^(4+abs(y)))";
    TreeNode* root = tree_node_from_string(input.chars);
    buffer_destruct(&input);
    if (!root)
        return WRONG_RESULT;
    //tree_node_show_dot(root);
    DifferMap* derivatives = build_all_derivatives(root, "derivatives.txt", false, false);
    if (!derivatives)
        return WRONG_RESULT;
    ValMap values = {};
    val_map_construct(&values, 2);
    val_map_add(&values, "x", 0.5);
    val_map_add(&values, "y", 3);
    TreeNode* result = tree_substitute(derivatives->values[0], &values);
    tree_node_show_dot(result);
    TreeNode* res_smpl = tree_calculate(result, true);
    tree_node_show_tex(result);
    tree_node_show_dot(res_smpl);

    //tree_node_show_dot(derivative);
    tree_node_destruct(root);
    differ_map_destruct(derivatives);
    val_map_destruct(&values);

    free (derivatives);
    //send_email("./Mail/header", "./Mail/message", "./Tex/temp.pdf", "Лаба.pdf");

    return NO_ERROR;
}
