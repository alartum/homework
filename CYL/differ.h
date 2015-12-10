#ifndef DIFFER_H_INCLUDED
#define DIFFER_H_INCLUDED

#include <math.h>
#include <string.h>
#include "map.h"
#include "list.h"


#define TEX_NAME "./Tex/temp.tex"
// Indicates whether simplification has occured
FILE* _TEX_FILE;


bool replace_node (TreeNode** old_node, const TreeNode* new_node)
{
    ASSERT_OK(TreeNode, *old_node);
    ASSERT_OK(TreeNode, new_node);
    //fprintf (_TEX_FILE, "Поразмыслив, поймём, что ");
    //printf (_TEX_FILE, "%s = %s.\n\n", tree_node_to_tex(*old_node, false), tree_node_to_tex(new_node, false));
    TreeNode* parent = (*old_node)->parent;
    TreeNode* new_copy = tree_node_full_copy(new_node);
    if (!new_copy)
        return false;
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
    return true;
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

bool tree_calculate__ (TreeNode** source_pointer, bool comp_fu)
{
    bool is_changed = false;
    TreeNode* source = *source_pointer;
    TreeNode* left = source->left;
    TreeNode* right = source->right;

    if (source->type == OP)
    {
        switch (source->word[0])
        {
        case '+':
            if (left && right)
            {
                if (left->type == NUM && right->type == NUM)
                {
                    float sum = left->value + right->value;
                    TreeNode* new_node = _NUM(&sum);
                    is_changed = replace_node(source_pointer, new_node);
                    tree_node_destruct(new_node);
                }
            }
            break;
        case '-':
            if (left && right)
            {
                if (left->type == NUM && right->type == NUM)
                {
                    float sub = left->value - right->value;
                    TreeNode* new_node = _NUM(&sub);
                    is_changed = replace_node(source_pointer, new_node);
                    tree_node_destruct(new_node);
                }
            }
            break;
        case '*':
            if (left && right)
            {
                if (left->type == NUM && right->type == NUM)
                {
                    float mul = left->value * right->value;
                    TreeNode* new_node = _NUM(&mul);
                    is_changed = replace_node(source_pointer, new_node);
                    tree_node_destruct(new_node);
                }
            }
            break;
        case '/':
            if (left && right)
            {
                if (left->type == NUM && right->type == NUM)
                {
                    float div = left->value / right->value;
                    TreeNode* new_node = _NUM(&div);
                    is_changed = replace_node(source_pointer, new_node);
                    tree_node_destruct(new_node);
                }
            }
            break;
        case '^':
            if (left && right)
            {
                if (left->type == NUM && right->type == NUM)
                {
                    float power = pow(left->value, right->value);
                    TreeNode* new_node = _NUM(&power);
                    is_changed = replace_node(source_pointer, new_node);
                    tree_node_destruct(new_node);
                }
            }
        default:
            break;
        }
    }
    else if (comp_fu && source->type == FU && left && left->type == NUM)
    {
        #define FUNCTION(fu_name, fu) \
            if (source->word &&!strcmp (source->word, #fu_name))\
            {\
                float x = left->value;\
                float res = fu;\
                TreeNode* new_node = _NUM(&res);\
                is_changed = replace_node(source_pointer, new_node);\
                tree_node_destruct(new_node);\
            }

        #include "fu_table.h"
        #undef FUNCTION
    }

    bool is_left_changed = false, is_right_changed = false;\
    if ((*source_pointer)->right)
        is_right_changed = tree_calculate__(&((*source_pointer)->right), comp_fu);
    if ((*source_pointer)->left)
        is_left_changed = tree_calculate__(&((*source_pointer)->left), comp_fu);
    return is_changed || is_left_changed || is_right_changed;
}

bool tree_simplify__ (TreeNode** source_pointer)
{
    bool is_changed = false;
    TreeNode* source = *source_pointer;
    TreeNode* left = source->left;
    TreeNode* right = source->right;
    switch (source->type)
    {
    case OP:
        switch (source->word[0])
        {
        case '+':
            if (left && right)
            {
                if (left->type == NUM && IS_ZERO(left->value))
                    is_changed = replace_node(source_pointer, right);
                else if (right->type == NUM && IS_ZERO(right->value))
                    is_changed = replace_node(source_pointer, left);
            }
            else
            {
                if (left)
                    is_changed = replace_node(source_pointer, left);
                else if (right)
                    is_changed = replace_node(source_pointer, right);
            }
            break;
        case '-':
            if (left && right)
            {
                if (left->type == NUM && IS_ZERO(left->value))
                {
                    float minus = -1;
                    TreeNode* _one = _NUM (&minus);
                    TreeNode* new_node = _MUL (_one, tree_node_full_copy(right));
                    is_changed = replace_node(source_pointer, new_node);
                }
                else if (right->type == NUM && IS_ZERO(right->value))
                    is_changed = replace_node(source_pointer, left);
            }
            else
            {
                if (left)
                {
                    float minus = -1;
                    TreeNode* _one = _NUM (&minus);
                    TreeNode* new_node = _MUL (_one, tree_node_full_copy(left));
                    is_changed = replace_node(source_pointer, new_node);
                }
                else if (right)
                {
                    float minus = -1;
                    TreeNode* _one = _NUM (&minus);
                    TreeNode* new_node = _MUL (_one, tree_node_full_copy(right));
                    is_changed = replace_node(source_pointer, new_node);
                }
            }
            break;
        case '*':
            if (left && right)
            {
                if ((right->type == NUM && IS_ZERO(right->value))
                        || (left->type == NUM && IS_ZERO(left->value)))
                {
                    float null = 0;
                    TreeNode* _null = _NUM (&null);
                    is_changed = replace_node(source_pointer, _null);
                }
                else if (right->type == NUM && IS_ZERO(right->value - 1))
                {
                    is_changed = replace_node(source_pointer, left);
                }
                else if (left->type == NUM && IS_ZERO(left->value - 1))
                {
                    is_changed = replace_node(source_pointer, right);
                }
            }
            break;
        case '/':
            if (!left || !right)
                break;
            if (right->type == NUM
                            && IS_ZERO(right->value - 1))
            {
                is_changed = replace_node(source_pointer, left);
            }
            else if (left->type == NUM && IS_ZERO(left->value))
                {
                    float null = 0;
                    TreeNode* _null = _NUM (&null);
                    is_changed = replace_node(source_pointer, _null);
                }
            else if (right->type == NUM
                        && IS_ZERO(round((double)right->value) - right->value)
                               && left->type == NUM
                                    && IS_ZERO(round((double)left->value) - left->value))

            {
                int l_r_gcd = gcd((int)round(left->value), (int)round(right->value));
                left->value /= l_r_gcd;
                right->value /= l_r_gcd;
            }
            break;
        case '^':
            if (left && right && right->type == NUM)
            {
                if (IS_ZERO(right->value))
                {
                    float one = 1;
                    TreeNode* _one = _NUM (&one);
                    is_changed = replace_node(source_pointer, _one);
                }
                else if (IS_ZERO(right->value - 1))
                    is_changed = replace_node(source_pointer, left);
            }
        default:
            break;
        }
        default:
            break;
    }
    bool is_left_changed = false, is_right_changed = false;
    if ((*source_pointer)->right)
        is_right_changed = tree_simplify__(&((*source_pointer)->right));
    if ((*source_pointer)->left)
        is_left_changed = tree_simplify__(&((*source_pointer)->left));

    return is_changed || is_left_changed || is_right_changed;
}

// comp_fu = do we want to compute functions
TreeNode* tree_calculate (const TreeNode* source, bool comp_fu)
{
    ASSERT_OK(TreeNode, source);
    bool is_changed = true;
    TreeNode* calculated = tree_node_full_copy(source);

    if (!calculated)
        return NULL;
    while (is_changed)
    {
        is_changed = tree_calculate__(&calculated, comp_fu);
    }

    return calculated;
}
TreeNode* tree_simplify (const TreeNode* source, bool comp_fu)
{
    ASSERT_OK(TreeNode, source);
    bool is_changed = true;
    TreeNode* simplified = tree_node_full_copy(source);

    if (!simplified)
        return NULL;
    while (is_changed)
    {
        bool is_calculated = true;
        while (is_calculated)
        {
            //tree_node_show_dot(simplified);
            is_calculated = tree_calculate__ (&simplified, comp_fu);
        }
        is_changed = tree_simplify__ (&simplified);
        //printf ("Tried to simplify:\n");
        //tree_node_show_dot(simplified);
    }

    return simplified;
}

#define _DIFF(source) build_part_derivative__(source, argument, d_map)
#define _CPY(node) tree_node_full_copy(node)
TreeNode* build_part_derivative__ (const TreeNode* source, const char argument[], const DifferMap* d_map)
{
    TreeNode* left = source->left;
    TreeNode* right = source->right;

    ASSERT_OK(TreeNode, source);
    assert (argument);
    ASSERT_OK(DifferMap, d_map);
    switch (source->type)
    {
    case VAR:
        if (!strcmp(source->word, argument))
        {
            float num = 1.0;
            return _NUM(&num);
        }
        else
        {
            float nul = 0.0;
            return _NUM(&nul);
        }
        return tree_node_construct_copy(source, NULL, NULL);
    case OP:
        switch (source->word[0])
        {
        case '+':
            return _ADD(_DIFF(left), _DIFF(right));
        case '-':
            return _SUB(_DIFF(left), _DIFF(right));
        case '*':
            return _ADD(_MUL(_DIFF(left),_CPY(right)),_MUL(_CPY(left),_DIFF(right)));
        case '/':
            ;
            float two = 2.0;
            TreeNode* power_2 = _NUM(&two);
            return _DIV(_SUB(_MUL(_DIFF(left),_CPY(right)),_MUL(_CPY(left),_DIFF(right))),_POW(_CPY(right),power_2));
        case '^':
            ;
            if (right->type == NUM)
            {
                float new_pow = right->value - 1;

                return _MUL(_NUM(&right->value),_POW(_CPY(left),_NUM(&new_pow)));
            }
            else
            {
                TreeNode* ln_node = _FU("ln", _CPY(left));
                return _MUL(_CPY(source), _ADD(_MUL(_DIFF(right),ln_node),_MUL(_DIV(_DIFF(left), _CPY(left)), _CPY(right))));
            }
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
        TreeNode* derivative = differ_map_get(d_map, key);
        free(key);
        replace_nodes(derivative, "x", tree_node_full_copy(left));

        return _MUL(derivative, _DIFF(left));
    default:
        return NULL;
    }
}
#undef _DIFF
#undef _CPY

TreeNode* build_part_derivative (const TreeNode* source, const char argument[], const char deriv_file[], bool is_silent, bool comp_fu)
{
    ASSERT_OK(TreeNode, source);
    assert (argument);
    assert (deriv_file);
    _TEX_FILE = fopen (TEX_NAME, "w");
    if (!_TEX_FILE)
    {
        perror ("Please, provide /Tex directory.");
        return NULL;
    }
    Buffer format = {};
    if (!buffer_construct(&format, "./format.tex"))
    {
        printf ("Please, provide ./format.tex file.\n");
        return false;
    }
    fprintf (_TEX_FILE, "%s\n\n\\begin{document}\n", format.chars);
    fprintf (_TEX_FILE, "Найдём производную по %s:\n%s\n", argument, tree_node_to_tex(source, true));
    fprintf (_TEX_FILE, "Дифференцируем:\n\n");
    buffer_destruct(&format);

    DifferMap d_map = {};
    if (!differ_map_construct_from_file(&d_map, deriv_file))
        return NULL;

    TreeNode* simple_source = tree_simplify(source, comp_fu);
    fprintf (_TEX_FILE, "Упростим данное выражение:\n%s\n", tree_node_to_tex(simple_source, true));
    TreeNode* part_derivative = build_part_derivative__ (simple_source, argument, &d_map);
    tree_node_destruct(simple_source);
    fprintf (_TEX_FILE, "Итак, производная равна:\n%s\n", tree_node_to_tex(part_derivative, true));
    TreeNode* simplified = tree_simplify(part_derivative, comp_fu);
    tree_node_destruct(part_derivative);

    fprintf (_TEX_FILE, "Наконец, получаем производную:\n%s", tree_node_to_tex(simplified, true));
    fprintf (_TEX_FILE, "\\end{document}\n");
    if (fclose (_TEX_FILE))
    {
        perror ("#Can't close the file");
        return NULL;
    }
    char cmd[128] = {};
    strcat (cmd, "pdflatex -output-directory=./Tex -interaction=batchmode -jobname=\"differ_by_");
    strcat (cmd, argument);
    strcat (cmd, "\" ./Tex/temp.tex");
    system (cmd);
    if (!is_silent)
    {
        cmd[0] = '\0';
        strcat (cmd, "qpdfview ./Tex/differ_by_");
        strcat (cmd, argument);
        strcat (cmd, ".pdf");
        system (cmd);
    }

    return simplified;
}

void add_variables (const TreeNode* source, ListHead* variables)
{
    ASSERT_OK(TreeNode, source);
    ASSERT_OK(ListHead, variables);

    if (source->type == VAR)
        list_head_add(variables, source->word);
    if (source->left)
        add_variables(source->left, variables);
    if (source->right)
        add_variables(source->right, variables);
}

//Last element of the array is NULL
DifferMap* build_all_derivatives (const TreeNode* source, const char deriv_file[], bool is_silent, bool comp_fu)
{
    ASSERT_OK(TreeNode, source);
    assert (deriv_file);

    ListHead variables = {};
    if (!list_head_construct(&variables))
        return NULL;
    add_variables (source, &variables);
    DifferMap* derivatives = (DifferMap*) calloc (1, sizeof(*derivatives));
    if (!derivatives)
        return NULL;
    differ_map_construct(derivatives, variables.amount);
    int i = 0;
    for (ListElement* p = variables.first; p != NULL; p = p->next, i++)
    {
        derivatives->keys[i] = p->word;
        derivatives->values[i] = build_part_derivative(source, p->word, deriv_file, is_silent, comp_fu);
        //tree_node_show_tex(derivatives[i]);
    }

    return derivatives;
}

TreeNode* tree_substitute (const TreeNode* source, const ValMap* variables)
{
    TreeNode* new_tree = tree_node_full_copy(source);
    for (size_t i = 0; i < variables->amount; i++)
    {
        TreeNode* replacement = _NUM (variables->values + i);
        replace_nodes(new_tree, variables->keys[i], replacement);
        tree_node_destruct(replacement);
    }

    return new_tree;
}

#endif // DIFFER_H_INCLUDED
