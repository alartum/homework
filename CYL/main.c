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

int send_email (const char header_file[], const char message_file[], const char attach_file[], const char attach_name[])//const char whom[], const char attachment[])
{
    Buffer header_text = {};
    buffer_construct(&header_text, header_file);
    open_file (temp_message, "./Mail/temp_message", "w", "Can't open file!");
    char to_whom[NAME_MAX];
    sscanf (header_text.chars, "To:%s", to_whom);
    fprintf (temp_message, "%sMime-Version: 1.0\nContent-Type: multipart/mixed; boundary=\"newpart\"\n--newpart\n", header_text.chars);
    fprintf (temp_message, "Content-Type: text/plain; charset=utf-8\n\n");
    Buffer message_text = {};
    buffer_construct(&message_text, message_file);
    if (attach_file)
    {
        fprintf (temp_message, "%s\n\n--newpart\n", message_text.chars);
        fprintf (temp_message, "Content-Type: application/octet-stream\n");
        fprintf (temp_message, "Content-Transfer-Encoding: base64\n");
        fprintf (temp_message, "Content-Disposition: attachment; filename=\"%s\"\n", attach_name);
    }
    close_file(temp_message);
    char command[100];
    strcat (command, "base64 ");
    strcat (command, attach_file);
    strcat (command, " >> ./Mail/temp_message");
    system (command);
    command[0] = '\0';
    strcat (command, "ssmtp ");
    strcat (command, to_whom);
    strcat (command, " < ./Mail/temp_message");
    system (command);
    system ("rm ./Mail/temp_message");

    return 0;
}

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
    TreeNode* derivative = build_part_derivative(root, "x", "derivatives.txt");
    if (!derivative)
        return WRONG_RESULT;
    tree_node_show_dot(derivative);
    tree_node_destruct(root);
    tree_node_destruct(derivative);
 //   send_email("./Mail/header", "./Mail/message", "./Tex/temp.pdf", "Лаба.pdf");

    return NO_ERROR;
}
