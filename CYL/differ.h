#ifndef DIFFER_H_INCLUDED
#define DIFFER_H_INCLUDED

#include <string.h>
#include "map.h"
#include "list.h"


#define TEX_NAME "./Tex/temp.tex"
// Indicates whether simplification has occured
bool _CHANGED = true;
FILE* _TEX_FILE;


void replace_node (TreeNode** old_node, const TreeNode* new_node)
{
    fprintf (_TEX_FILE, "Поразмыслив, поймём, что ");
    fprintf (_TEX_FILE, "%s = %s.\n\n", tree_node_to_tex(*old_node, false), tree_node_to_tex(new_node, false));
    TreeNode* parent = (*old_node)->parent;
    TreeNode* new_copy = tree_node_full_copy(new_node);
    if (parent)
    {
        if (parent->left == *old_node)
        {
            tree_node_destruct(*old_node);
            tree_node_set_left(parent, new_copy);
        }
        else
        {
            tree_node_destruct(*old_node);
            tree_node_set_right(parent, new_copy);
        }
    }
    else
    {
        tree_node_destruct(*old_node);
        *old_node = new_copy;
    }
    _CHANGED = true;
}

void replace_nodes (TreeNode* start, const char word[], const TreeNode* replacement)
{
    if (start->type == VAR && !strcmp (start->word, word))
        replace_node(&start, replacement);
    else
    {
        if (start->left)
            replace_nodes (start->left, word, replacement);
        if (start->right)
            replace_nodes (start->right, word, replacement);
    }
}

void calculate_tree__ (TreeNode** source_pointer)
{
    switch ((*source_pointer)->type)
    {
    case OP:
        switch ((*source_pointer)->word[0])
        {
        case '+':
            if ((*source_pointer)->left && (*source_pointer)->right)
            {
                if ((*source_pointer)->left->type == NUM && (*source_pointer)->right->type == NUM)
                {
                    float sum = (*source_pointer)->left->value + (*source_pointer)->right->value;
                    TreeNode* new_node = _NUM(&sum);
                    replace_node(&(*source_pointer), new_node);
                    tree_node_destruct(new_node);
                }
            }
            break;
        case '-':
            if ((*source_pointer)->left && (*source_pointer)->right)
            {
                if ((*source_pointer)->left->type == NUM && (*source_pointer)->right->type == NUM)
                {
                    float sub = (*source_pointer)->left->value - (*source_pointer)->right->value;
                    TreeNode* new_node = _NUM(&sub);
                    replace_node(&(*source_pointer), new_node);
                    tree_node_destruct(new_node);
                }
            }
            break;
        case '*':
            if ((*source_pointer)->left && (*source_pointer)->right)
            {
                if ((*source_pointer)->left->type == NUM && (*source_pointer)->right->type == NUM)
                {
                    float mul = (*source_pointer)->left->value * (*source_pointer)->right->value;
                    TreeNode* new_node = _NUM(&mul);
                    replace_node(&(*source_pointer), new_node);
                    tree_node_destruct(new_node);
                }
            }
            break;
        case '/':
            if ((*source_pointer)->left && (*source_pointer)->right)
            {
                if ((*source_pointer)->left->type == NUM && (*source_pointer)->right->type == NUM)
                {
                    float div = (*source_pointer)->left->value / (*source_pointer)->right->value;
                    TreeNode* new_node = _NUM(&div);
                    replace_node(&(*source_pointer), new_node);
                    tree_node_destruct(new_node);
                }
            }
            break;
        case '^':
            if ((*source_pointer)->left && (*source_pointer)->right)
            {
                if ((*source_pointer)->left->type == NUM && (*source_pointer)->right->type == NUM)
                {
                    float power = pow((*source_pointer)->left->value, (*source_pointer)->right->value);
                    TreeNode* new_node = _NUM(&power);
                    replace_node(&(*source_pointer), new_node);
                    tree_node_destruct(new_node);
                }
            }
        default:
            break;
        }
        default:
            break;
    }
    if ((*source_pointer)->right)
        calculate_tree__(&((*source_pointer)->right));
    if ((*source_pointer)->left)
        calculate_tree__(&((*source_pointer)->left));
}

void simplify_tree__ (TreeNode** source_pointer)
{
    switch ((*source_pointer)->type)
    {
    case OP:
        switch ((*source_pointer)->word[0])
        {
        case '+':
            if ((*source_pointer)->left && (*source_pointer)->right)
            {
                if ((*source_pointer)->left->type == NUM && IS_ZERO((*source_pointer)->left->value))
                    replace_node(&(*source_pointer), (*source_pointer)->right);
                else if ((*source_pointer)->right->type == NUM && IS_ZERO((*source_pointer)->right->value))
                    replace_node(source_pointer, (*source_pointer)->left);
            }
            else
            {
                if ((*source_pointer)->left)
                    replace_node(source_pointer, (*source_pointer)->left);
                else if ((*source_pointer)->right)
                    replace_node(source_pointer, (*source_pointer)->right);
            }
            break;
        case '-':
            if ((*source_pointer)->left && (*source_pointer)->right)
            {
                if ((*source_pointer)->left->type == NUM && IS_ZERO((*source_pointer)->left->value))
                {
                    float minus = -1;
                    TreeNode* _one = _NUM (&minus);
                    TreeNode* new_node = _MUL (_one, tree_node_full_copy((*source_pointer)->right));
                    replace_node(source_pointer, new_node);
                }
                else if ((*source_pointer)->right->type == NUM && IS_ZERO((*source_pointer)->right->value))
                    replace_node(source_pointer, (*source_pointer)->left);
            }
            else
            {
                if ((*source_pointer)->left)
                {
                    float minus = -1;
                    TreeNode* _one = _NUM (&minus);
                    TreeNode* new_node = _MUL (_one, tree_node_full_copy((*source_pointer)->left));
                    replace_node(source_pointer, new_node);
                }
                else if ((*source_pointer)->right)
                {
                    float minus = -1;
                    TreeNode* _one = _NUM (&minus);
                    TreeNode* new_node = _MUL (_one, tree_node_full_copy((*source_pointer)->right));
                    replace_node(source_pointer, new_node);
                }
            }
            break;
        case '*':
            if ((*source_pointer)->left && (*source_pointer)->right)
            {
                if (((*source_pointer)->right->type == NUM && IS_ZERO((*source_pointer)->right->value))
                        || ((*source_pointer)->left->type == NUM && IS_ZERO((*source_pointer)->left->value)))
                {
                    float null = 0;
                    TreeNode* _null = _NUM (&null);
                    replace_node(source_pointer, _null);
                }
                else if ((*source_pointer)->right->type == NUM && IS_ZERO((*source_pointer)->right->value - 1))
                {
                    replace_node(source_pointer, (*source_pointer)->right);
                }
                else if ((*source_pointer)->left->type == NUM && IS_ZERO((*source_pointer)->left->value - 1))
                {
                    replace_node(source_pointer, (*source_pointer)->left);
                }
            }
            break;
        case '/':
            if ((*source_pointer)->left && (*source_pointer)->right
                && (*source_pointer)->right->type == NUM
                && IS_ZERO((*source_pointer)->right->value - 1))
                replace_node(source_pointer, (*source_pointer)->left);
            break;
        case '^':
            if ((*source_pointer)->left && (*source_pointer)->right && (*source_pointer)->right->type == NUM)
            {
                if (IS_ZERO((*source_pointer)->right->value))
                {
                    float one = 1;
                    TreeNode* _one = _NUM (&one);
                    replace_node(source_pointer, _one);
                }
                else if (IS_ZERO((*source_pointer)->right->value - 1))
                    replace_node(source_pointer, (*source_pointer)->left);
            }
        default:
            break;
        }
        default:
            break;
    }
    if ((*source_pointer)->right)
        simplify_tree__(&((*source_pointer)->right));
    if ((*source_pointer)->left)
        simplify_tree__(&((*source_pointer)->left));
}

TreeNode* simplify_tree (const TreeNode* source)
{
    _CHANGED = true;
    TreeNode* simplified = tree_node_full_copy(source);
    if (!simplified)
        return NULL;
    while (_CHANGED)
    {
        _CHANGED = false;
        simplify_tree__ (&simplified);
        //printf ("Tried to simplify:\n");
        //tree_node_show_dot(simplified);
        if (_CHANGED)
        {
            while (_CHANGED)
            {
                _CHANGED = false;
                //tree_node_show_dot(simplified);
                calculate_tree__ (&simplified);
            }
            _CHANGED = true;
        }
    }

    return simplified;
}

DifferMap _DMAP = {};
#define _DIFF(source) build_part_derivative__(source, argument)
#define _L source->left
#define _R source->right
#define _CPY(node) tree_node_full_copy(node)
TreeNode* build_part_derivative__ (const TreeNode* source, const char argument[])
{
    switch (source->type)
    {
    case VAR:
        if (!strcmp(source->word, argument))
        {
            float num = 1.0;
            return _NUM(&num);
        }
        // else
        return tree_node_construct_copy(source, NULL, NULL);
    case OP:
        switch (source->word[0])
        {
        case '+':
            return _ADD(_DIFF(_L), _DIFF(_R));
        case '-':
            return _SUB(_DIFF(_L), _DIFF(_R));
        case '*':
            return _ADD(_MUL(_DIFF(_L),_CPY(_R)),_MUL(_CPY(_L),_DIFF(_R)));
        case '/':
            ;
            float two = 2.0;
            TreeNode* power_2 = _NUM(&two);
            return _DIV(_SUB(_MUL(_DIFF(_L),_CPY(_R)),_MUL(_CPY(_L),_DIFF(_R))),_POW(_CPY(_R),power_2));
        case '^':
            ;
            TreeNode* ln_node = _FU("ln", _CPY(_L));
            return _MUL(_CPY(source), _ADD(_MUL(_DIFF(_R),ln_node),_MUL(_DIV(_DIFF(_L), _CPY(_L)), _CPY(_R))));
        }
    case NUM:
        ;
        float nul = 0.0;
        return _NUM(&nul);
    case FU:
        ;
        char* key = (char*)calloc(strlen(source->word) + 5, sizeof(char));
        strcat (key, source->word);
        strcat (key, "(x)");
        TreeNode* derivative = differ_map_get(&_DMAP, key);
        free(key);
        replace_nodes(derivative, "x", tree_node_full_copy(_L));

        return _MUL(derivative, _DIFF(_L));
    default:
        return NULL;
    }
}
#undef _DIFF
#undef _L
#undef _R
#undef _CPY

TreeNode* build_part_derivative (const TreeNode* source, const char argument[], const char filename[])
{
    ASSERT_OK(TreeNode, source);
    assert (argument);
    _TEX_FILE = fopen (TEX_NAME, "w");
    if (!_TEX_FILE)
    {
        perror ("Please, provide /Tex directory.");
        return NULL;
    }
    Buffer format = {};
    if (!buffer_construct(&format, "./Tex/format.tex"))
    {
        printf ("Please, provide ./Tex/format.tex file.\n");
        return false;
    }
    fprintf (_TEX_FILE, "%s\n\n\\begin{document}\n", format.chars);
    fprintf (_TEX_FILE, "Найдём производную по %s:\n%s\n", argument, tree_node_to_tex(source, true));
    fprintf (_TEX_FILE, "Дифференцируем:\n\n");
    buffer_destruct(&format);

    differ_map_construct_from_file(&_DMAP, filename);
    TreeNode* part_derivative = build_part_derivative__ (source, argument);
    fprintf (_TEX_FILE, "Итак, производная равна:\n%s\n", tree_node_to_tex(part_derivative, true));
    TreeNode* simplified = simplify_tree(part_derivative);
    tree_node_destruct(part_derivative);

    fprintf (_TEX_FILE, "Наконец, получаем производную:\n%s", tree_node_to_tex(simplified, true));
    fprintf (_TEX_FILE, "\\end{document}\n");
    if (fclose (_TEX_FILE))
    {
        perror ("#Can't close the file");
        return NULL;
    }
    system ("pdflatex -output-directory=./Tex ./Tex/temp.tex");
    system ("rm ./Tex/temp.tex");
    system ("qpdfview ./Tex/temp.pdf");

    return simplified;
}



#endif // DIFFER_H_INCLUDED
