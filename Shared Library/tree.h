#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include <assert.h>
#include <stdio.h>

/// More comfortable dump
#define TreeNode_dump(This) tree_node_dump_(This, #This)
/// To be stylish
#define tree_node_dump(This) TreeNode_dump(This)
/// To be stylish
#define TreeNode_OK(This) tree_node_OK(This)
#define tree_node_dump_r(This) tree_node_dump_r_(This, #This)

//////////////////////
//Beautiful defines///
//////////////////////

#define _ADD(left, right) tree_node_construct(OP, "+", left, right)
#define _SUB(left, right) tree_node_construct(OP, "-", left, right)
#define _MUL(left, right) tree_node_construct(OP, "*", left, right)
#define _DIV(left, right) tree_node_construct(OP, "/", left, right)
#define _POW(base, exp)   tree_node_construct(OP, "^", base, exp)
#define _FU(name, arg)    tree_node_construct(FU, name, arg, NULL)
#define _NUM(num_pointer) tree_node_construct(NUM, num_pointer, NULL, NULL)
#define _VAR(name)        tree_node_construct(VAR, name, NULL, NULL)

int _CURRENT_ID = 0;

enum NODE_TYPES
{
    VAR,
    OP,
    NUM,
    FU,
    ERR
};
/**
@brief Node of a binary tree.
The node of NODE_TYPE type that containes value.
*/

typedef struct TreeNode
{
    enum NODE_TYPES type;
    float value;
    char* word;

    struct TreeNode* parent;
    struct TreeNode* left;
    struct TreeNode* right;
    int id;
} TreeNode;

/**
*@brief Standard treconstructor.
*
*Constructs tree node and puts the given word in it.
*@param type If the node is operational, variable or numeric
*@param data Data stored in the node
*@param left Left subnode
*@param right Right subnode
*@return Pointer to allocated TreeNode if success, NULL pointer otherwise
*/
TreeNode* tree_node_construct (const enum NODE_TYPES type, const void *data, TreeNode* left, TreeNode* right);
/**
*@brief Copy constructor
*
*Constructs tree node as copy of the other
*@param other Pointer to the node to be duplicated
*@return Pointer to allocated TreeNode if success, NULL pointer otherwise
*@warning The pointers to left and right subnodes are not being copied.
*/
TreeNode* tree_node_construct_copy (const TreeNode* other, TreeNode* left, TreeNode* right);
/**
*@brief Standard tree node destructor.
*
*Destructs the given node.
*@param This Pointer to the node to be destructed
*/
void tree_node_destruct (TreeNode** This_pointer);
/**
*@brief Checks if the node is in a tree already
*
*Checks the links of the node.
*@param This Pointer to the node to be checked
*/
bool tree_node_linked (const TreeNode* This);
/**
*@brief Standard tree node varificator.
*
*Checks if the given node is valid
*@param This Pointer to the node to be checked
*/
bool tree_node_OK (const TreeNode* This);
/**
*@brief Prints node's dump.
*
*Outputs the current status of node.
*@param This Pointer to the node to be dumped.
*/
void tree_node_dump_ (const TreeNode* This, const char name[]);
/**
*@brief Adds left subnode to the given node
*
*Sets the left subnode of the node to the given node
*@param This Pointer to the node to be changed
*@param left Pointer to the node to be added
*@warning Be sure to make the copy of left before!
*/
bool tree_node_set_left (TreeNode* This, TreeNode* left);
/**
*@brief Adds right subnode to the given node
*
*Sets the right subnode of the node to the given node
*@param This Pointer to the node to be changed
*@param right Pointer to the node to be added
*@warning Be sure to make the copy of right before!
*/
bool tree_node_set_right (TreeNode* This, TreeNode* right);
/**
*@brief Converts the tree to the node format
*
*Converts this and all subnodes to the dot format and puts them in file.
*@param This Pointer to the node to be converted
*@param dot_file The file results will be written to
*@return true if success, false otherwise
*@warning You should care about the correct file status yourself.
*/
bool tree_node_to_dot (const TreeNode* This, FILE* dot_file);
/**
*@brief Shows the tree graph built by dot
*
*Convert this node and all subnodes to dot format then builds graph with dot and shows it.
*@param This Pointer to the node to start with
*@return true if success, false otherwise
*/
bool tree_node_show_dot (const TreeNode* This);
/**
*@brief Parses the input file and stroes the
*
*Convert this node and all subnodes to dot format then builds graph with dot and shows it.
*@param This Pointer to the node to start with
*@return true if success, false otherwise
*/
bool tree_node_show_dot (const TreeNode* This);


TreeNode* tree_node_construct (const enum NODE_TYPES type, const void *data, TreeNode* left, TreeNode* right)
{
    if (type >= ERR || type < VAR)
        return NULL;
    TreeNode* new_node = (TreeNode*)calloc(1, sizeof(TreeNode));

    if (!new_node)
        return NULL;
    //printf ("Type is %d\n", new_node->type);
    new_node->type = type;
    if (type == NUM)
    {
        new_node->value = *(float*)data;
        //printf ("COn value %f", new_node->value);
        new_node->word = NULL;
    }
    else
    {
        new_node->value = 0;
        new_node->word = strdup ((char*)data);
        if (!new_node)
            return NULL;
    }
    new_node->parent = NULL;
    if (left && !tree_node_set_left(new_node, left))
        return NULL;
    if (right && !tree_node_set_right(new_node, right))
        return NULL;

    new_node->id = _CURRENT_ID;
    _CURRENT_ID++;


    return new_node;
}

TreeNode* tree_node_construct_copy (const TreeNode* other, TreeNode* left, TreeNode* right)
{
    ASSERT_OK(TreeNode, other);
    if (other->type == NUM)
        return tree_node_construct(NUM, &(other->value), left, right);
    else
        return tree_node_construct(other->type, other->word, left, right);
}

void tree_node_destruct (TreeNode** This_pointer)
{
    assert (This_pointer);
    assert (*This_pointer);
    TreeNode* This = *This_pointer;
    if (This->left)
    {
        tree_node_destruct(&This->left);
        This->left = NULL;
    }
    if (This->right)
    {
        tree_node_destruct(&This->right);
        This->right = NULL;
    }
    This->type = ERR;
    This->value = 0;
    if (!This->word)
        free(This->word);
    if (This->parent)
    {
        if (This->parent->left == This)
            This->parent->left = NULL;
        else if (This->parent->right == This)
            This->parent->right = NULL;
    }
    This->word = NULL;
    This->parent = NULL;
    free (*This_pointer);
    *This_pointer = NULL;
}

bool tree_node_linked (const TreeNode* This)
{
    assert (This);
    return This->left || This->right || This->parent;
}

bool tree_node_OK (const TreeNode* This)
{
    assert (This);
    bool is_ok = This->type < ERR && This->type >= VAR;
    bool is_legal_child = true;
    if (This->parent)
        is_legal_child = This->parent->left == This || This->parent->right == This;

    return is_ok && is_legal_child;
}

void tree_node_dump_ (const TreeNode* This, const char name[])
{
    assert (This);
    printf ("%s = TreeNode (", name);
    if (tree_node_OK(This))
        printf ("ok)\n");
    else
        printf ("ERROR)\n");
    printf ("{\n");
    printf ("\taddress = %p\n\n", This);
    printf ("\tid = %d\n", This->id);
    printf ("\ttype = %d\n", This->type);
    printf ("\tvalue = %g\n", This->value);
    printf ("\tword = <%s>\n", This->word);
    printf ("\tparent = %p\n", This->parent);
    printf ("\tleft = %p\n", This->left);
    printf ("\tright = %p\n", This->right);
    printf ("}\n");
    if (!tree_node_OK(This))
        getchar();
}

void tree_node_dump_r_ (const TreeNode* This, const char name[])
{
    assert (This);
    printf ("%s = TreeNode (", name);
    if (tree_node_OK(This))
        printf ("ok)\n");
    else
        printf ("ERROR)\n");
    printf ("{\n");
    printf ("\taddress = %p\n\n", This);
    printf ("\tid = %d\n", This->id);
    printf ("\ttype = %d\n", This->type);
    printf ("\tvalue = %g\n", This->value);
    printf ("\tword = <%s>\n", This->word);
    printf ("\tparent = %p\n", This->parent);
    printf ("\tleft = %p\n", This->left);
    printf ("\tright = %p\n", This->right);
    printf ("}\n");
    if (!tree_node_OK(This))
        getchar();
    if (This->left)
        tree_node_dump_r(This->left);
    if (This->right)
        tree_node_dump_r(This->right);
}

bool tree_node_set_left (TreeNode* This, TreeNode* left)
{
    ASSERT_OK(TreeNode, This);
    ASSERT_OK(TreeNode, left);
    //printf ("Adding to\n");
    //tree_node_dump(This);
    //printf ("Setting left to\n");
    //tree_node_dump(left);
    if (This->left || left->parent)
    {
        return false;
    }
    This->left = left;
    left->parent = This;
    return true;
}

bool tree_node_set_right (TreeNode* This, TreeNode* right)
{
    ASSERT_OK(TreeNode, This);
    ASSERT_OK(TreeNode, right);
    //printf ("Adding to\n");
    //tree_node_dump(This);
    //printf ("Setting right to\n");
   // tree_node_dump(right);
    if (This->right || right->parent)
        return false;
    This->right = right;
    right->parent = This;
    return true;
}

bool tree_node_to_dot (const TreeNode* This, FILE* dot_file)
{
    assert (dot_file);
    ASSERT_OK(TreeNode, This);
   // printf ("Tree_to_dot\n");
   // tree_node_dump(This);
    fprintf (dot_file, "\t%d ", This->id);
    switch (This->type)
    {
    case NUM:
        fprintf (dot_file, "[label = \"%g\", shape = circle, fillcolor = \"#E6DF44\", fontcolor = \"#FFFFFF\", style = filled]\n", This->value);
        break;
    case VAR:
        fprintf (dot_file, "[label = \"%s\", shape = circle, fillcolor = \"#F0810F\", fontcolor = \"#FFFFFF\", style = filled]\n", This->word);
        break;
    case OP:
        fprintf (dot_file, "[label = \"%s\", shape = doublecircle, fillcolor = \"#011A27\", fontcolor = \"#FFFFFF\", style = filled]\n", This->word);
        break;
    case FU:
        fprintf (dot_file, "[label = \"%s\", shape = doublecircle, fillcolor = \"#063852\", fontcolor = \"#FFFFFF\", style = filled]\n", This->word);
        break;
    default:
        break;
    }
    if (This->left)
    {
        fprintf (dot_file, "\t%d -> %d\n", This->id, This->left->id);
        tree_node_to_dot(This->left, dot_file);
    }
    if (This->right)
    {
        fprintf (dot_file, "\t%d -> %d\n", This->id, This->right->id);
        tree_node_to_dot(This->right, dot_file);
    }
    return true;
}

bool tree_node_show_dot (const TreeNode* This)
{
    if (!This)
        return false;
    open_file(dot_file, "dump.dot", "w", "Something went wrong, sir!");
    fprintf (dot_file, "digraph G {\n\tbgcolor = \"#FCFDFE\"\n");
    bool is_ok = tree_node_to_dot(This, dot_file);
    fprintf (dot_file, "}\n");
    close_file(dot_file);
    system("xdot dump.dot");
    system("rm dump.dot");
    return is_ok;
}

TreeNode* tree_node_full_copy (const TreeNode* source)
{
    ASSERT_OK(TreeNode, source);
    TreeNode* new_node = tree_node_construct_copy(source, NULL, NULL);
    if (source->left)
        tree_node_set_left(new_node, tree_node_full_copy(source->left));
    if (source->right)
        tree_node_set_right(new_node, tree_node_full_copy(source->right));
    return new_node;
}

enum PRIOR
{
    num,
    var,
    mul,
    sum,
    pew, //I don't care about pow
};

typedef struct
{
    enum PRIOR type;
    char* word;
} Expression;

Expression tree_node_to_tex__ (const TreeNode* This)
{
    ASSERT_OK(TreeNode, This);
    char word[NAME_MAX];
    Expression result = {};
    switch (This->type)
    {
    case NUM:
        ;
        if (This->value < 0)
            sprintf (word, "(%g)", This->value);
        else
            sprintf (word, "%g", This->value);
        result.type = num;
        result.word = strdup(word);
        break;
    case VAR:
        ;
        sprintf (word, "%s", This->word);
        result.type = var;
        result.word = strdup(word);
        break;
    case OP:
        if (!This->left || !This->right)
            return result;
        Expression left = tree_node_to_tex__(This->left);
        Expression right = tree_node_to_tex__(This->right);
        if (!left.word || !right.word)
            return result;

        char* new_string = (char*)calloc (strlen(left.word) + strlen(right.word) + 50, sizeof(char));
        switch (This->word[0])
        {
        case '+':
            strcat (new_string, left.word);
            strcat (new_string, " + ");
            strcat (new_string, right.word);
            result.type = sum;
            result.word = new_string;
            break;
        case '-':
            strcat (new_string, left.word);
            strcat (new_string, " - ");
            if (right.type == sum)
            {
                strcat (new_string, " \\left(");
                strcat (new_string, right.word);
                strcat (new_string, " \\right)");
            }
            else
                strcat (new_string, right.word);
            result.type = sum;
            result.word = new_string;
            break;
        case '*':
            if (!strcmp(left.word, "(-1)"))
                strcat (new_string, "-");
            else if ((right.type == var || right.type == pew) && (left.type != pew))//don't put cdot
                strcat (new_string, left.word);
            else
            {
                if (left.type == sum)
                {
                    strcat (new_string, "\\left(");
                    strcat (new_string, left.word);
                    strcat (new_string, "\\right)");
                }
                else
                    strcat (new_string, left.word);

                strcat (new_string, " \\cdot ");
            }

            if (right.type == sum)
            {
                strcat (new_string, "\\left(");
                strcat (new_string, right.word);
                strcat (new_string, "\\right)");
            }
            else
                strcat (new_string, right.word);

            result.type = mul;
            result.word = new_string;
            break;
        case '/':
            strcat (new_string, " \\frac {");
            strcat (new_string, left.word);
            strcat (new_string, "} {");
            strcat (new_string, right.word);
            strcat (new_string, "} ");
            result.type = mul;
            result.word = new_string;
            break;
        case '^':
            if (left.type == var)
                strcat (new_string, left.word);
            else
            {
                strcat (new_string, "\\left(");
                strcat (new_string, left.word);
                strcat (new_string, "\\right)");
            }
            if (strcmp (right.word, "1"))
            {
                strcat (new_string, "^{");
                strcat (new_string, right.word);
                strcat (new_string, "}");
            }
            result.type = pew;
            result.word = new_string;
            break;
        default:
            break;
        }
        free(left.word);
        free(right.word);
        break;
    case FU:
        ;
        Expression fu_arg = tree_node_to_tex__(This->left);
        char* new_string_ = (char*)calloc (strlen(fu_arg.word) + 50, sizeof(char));

        strcat (new_string_, "\\");
        strcat (new_string_, This->word);
        if (fu_arg.type != var)
        {
            strcat (new_string_, "\\left(");
            strcat (new_string_, fu_arg.word);
            strcat (new_string_, "\\right)");
        }
        else
        {
            strcat (new_string_, " ");
            strcat (new_string_, fu_arg.word);
        }

        free(fu_arg.word);
        result.type = pew;
        result.word = new_string_;
        break;
    default:
        break;
    }

    return result;
}

// Be sure to use with new_line = false only for really short formulas
char* tree_node_to_tex (const TreeNode* This, bool new_line)
{
    Expression tex_exp = tree_node_to_tex__(This);
    char* tex_string = (char*)calloc(strlen(tex_exp.word) + 10, sizeof(char*));
    if (new_line)
    {
        //align*~dmath and \nonumber at the end
        strcat (tex_string, "\\begin{align*}\n");
        strcat (tex_string, tex_exp.word);
        strcat (tex_string, "\\end{align*}\n");
    }
    else
    {
        //strcat (tex_string, "$");
        strcat (tex_string, tex_exp.word);
        //strcat (tex_string, "$");
    }

    return tex_string;
}

bool tree_node_show_tex (const TreeNode* This, FILE* format_file)
{
    if (!This)
        return false;
    Buffer format = {};
    if (!buffer_construct_file(&format, format_file))
        return false;
    open_file(tex_file, "./temp_show_tex.tex", "w", "Something is wrong, sir!");
    fprintf (tex_file, "%s\n\n\\begin{document}\n", format.chars);
    buffer_destruct(&format);
    fprintf (tex_file, "%s", tree_node_to_tex(This, true));
    fprintf (tex_file, "\\end{document}\n");
    close_file(tex_file);
    system ("pdflatex -output-directory=./Tex -interaction=batchmode ./Tex/temp_show_tex.tex");
    system ("rm ./Tex/temp_show_tex.tex");
    system ("qpdfview ./Tex/temp_show_tex.pdf");
    return true;
}

#endif // TREE_H_INCLUDED
