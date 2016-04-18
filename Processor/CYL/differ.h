#ifndef DIFFER_H_INCLUDED
#define DIFFER_H_INCLUDED

#include <math.h>
#include <string.h>
#include "map.h"
#include "list.h"
#include "tex.h"

// Indicates whether simplification has occured

bool replace_node (TreeNode** old_node, const TreeNode* new_node)
{
    ASSERT_OK(TreeNode, *old_node);
    ASSERT_OK(TreeNode, new_node);
    //fprintf (_TEX_FILE, "Поразмыслив, поймём, что ");
    //printf (_TEX_FILE, "%s = %s.\n\n", tree_node_to_tex(*old_node, false), tree_node_to_tex(new_node, false));
    TreeNode* parent = (*old_node)->parent;
    //printf ("OLD\n");
    //tree_node_dump(*old_node);
    //if (new_node->left)
       // tree_node_dump(new_node->left);
   // if (new_node->right)
    //    tree_node_dump(new_node->right);
    TreeNode* new_copy = tree_node_full_copy(new_node);
    //printf ("NEW\n");
    //tree_node_dump(new_node);
    if (!new_copy)
        return false;
    if (parent)
    {
        if (parent->left == *old_node)
        {
            tree_node_destruct(old_node);
            tree_node_set_left(parent, new_copy);
        }
        else
        {
            tree_node_destruct(old_node);
            tree_node_set_right(parent, new_copy);
        }
    }
    else
    {
        tree_node_destruct(old_node);
    }
    *old_node = new_copy;
    //printf ("AFTER\n");
    //tree_node_show_dot(*old_node);
    return true;
}

void replace_nodes (TreeNode* start, const char word[],
                    const TreeNode* replacement)
{
    if (start->type == VAR && !strcmp (start->word, word))
    {
       // printf ("Replacing %s\n", word);
        replace_node(&start, replacement);
    }
    else
    {
        if (start->left)
            replace_nodes (start->left, word, replacement);
        if (start->right)
            replace_nodes (start->right, word, replacement);
    }
}

bool tree_calculate__ (TreeNode** source_pointer,
                       bool comp_fu             ,
                       const FILE* tex_file     ,
                       const char** comments    )
{
    bool is_changed = false;
    TreeNode* source = *source_pointer;
    TreeNode* left = source->left;
    TreeNode* right = source->right;
    TreeNode* new_node = NULL; //the replacement for the source

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
                    new_node = _NUM(&sum);
                }
            }
            break;
        case '-':
            if (left && right)
            {
                if (left->type == NUM && right->type == NUM)
                {
                    float sub = left->value - right->value;
                    new_node = _NUM(&sub);
                }
            }
            break;
        case '*':
            if (left && right)
            {
                if (left->type == NUM && right->type == NUM)
                {
                    float mul = left->value * right->value;
                    new_node = _NUM(&mul);
                }
            }
            break;
        case '/':
            if (left && right)
            {
                if (left->type == NUM && right->type == NUM)
                {
                    float div = left->value / right->value;
                    new_node = _NUM(&div);
                }
            }
            break;
        case '^':
            if (left && right)
            {
                if (left->type == NUM && right->type == NUM)
                {
                    float power = pow(left->value, right->value);
                    new_node = _NUM(&power);
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
                new_node = _NUM(&res);\
            }

        #include "fu_table.h"
        #undef FUNCTION
    }
    //////////////////
    ////REPLACEMNET///
    //////////////////
    if (new_node)
    {
        is_changed = replace_node(source_pointer, new_node);
        if (new_node)
            tree_node_destruct(&new_node);
    }
    //////////////////
    ////RECURSION/////
    //////////////////
    bool is_left_changed = false, is_right_changed = false;\
    if ((*source_pointer)->right)
        is_right_changed = tree_calculate__(&((*source_pointer)->right), comp_fu, tex_file, comments);
    if ((*source_pointer)->left)
        is_left_changed = tree_calculate__(&((*source_pointer)->left), comp_fu, tex_file, comments);

    return is_changed || is_left_changed || is_right_changed;
}

bool tree_simplify__ (TreeNode** source_pointer,
                      FILE* tex_file     ,
                      const char** comments    )
{
    //printf ("Simplifying\n");
    //tree_node_dump(*source_pointer);
    bool is_changed = false;
    TreeNode* source = *source_pointer;
    TreeNode* left = source->left;
    TreeNode* right = source->right;
    TreeNode* new_node = NULL; //the replacement for the source

    switch (source->type)
    {
    case OP:
        switch (source->word[0])
        {
        case '+':
            if (left && right)
            {
                if (left->type == NUM && IS_ZERO(left->value))
                    new_node = right;
                else if (right->type == NUM && IS_ZERO(right->value))
                    new_node = left;
            }
            else
            {
                if (left)
                    new_node = left;
                else if (right)
                    new_node = right;
            }
            break;
        case '-':
            if (left && right)
            {
                if (left->type == NUM && IS_ZERO(left->value))
                {
                    float minus = -1;
                    TreeNode* _one = _NUM (&minus);
                    new_node = _MUL (_one, tree_node_full_copy(right));
                }
                else if (right->type == NUM && IS_ZERO(right->value))
                    new_node = left;
            }
            else
            {
                if (left)
                {
                    float minus = -1;
                    TreeNode* _one = _NUM (&minus);
                    new_node = _MUL (_one, tree_node_full_copy(left));
                }
                else if (right)
                {
                    float minus = -1;
                    TreeNode* _one = _NUM (&minus);
                    new_node = _MUL (_one, tree_node_full_copy(right));
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
                    new_node = _NUM (&null);
                }
                else if (right->type == NUM && IS_ZERO(right->value - 1))
                    new_node = left;
                else if (left->type == NUM && IS_ZERO(left->value - 1))
                    new_node = right;
            }
            break;
        case '/':
            if (!left || !right)
                break;
            if (right->type == NUM
                            && IS_ZERO(right->value - 1))
            {
                new_node = left;
            }
            else if (left->type == NUM && IS_ZERO(left->value))
                {
                    float null = 0;
                    new_node = _NUM (&null);
                }
            else if (right->type == NUM
                        && IS_ZERO(round((double)right->value) - right->value)
                               && left->type == NUM
                                    && IS_ZERO(round((double)left->value) - left->value))

            {
                int l_r_gcd = gcd((int)round(left->value), (int)round(right->value));
                left->value /= l_r_gcd;
                right->value /= l_r_gcd;
                is_changed = true;
            }
            break;
        case '^':
            if (left && right && right->type == NUM)
            {
                if (IS_ZERO(right->value))
                {
                    float one = 1;
                    new_node = _NUM (&one);
                }
                else if (IS_ZERO(right->value - 1))
                    new_node = left;
            }
        default:
            break;
        }
        default:
            break;
    }
    //////////////////
    ////REPLACEMNET///
    //////////////////
    if (new_node)
    {
        //tree_node_show_dot(*source_pointer);
        //tree_node_show_dot(new_node);
        //tree_node_dump (new_node);

        //printf ("Replacing\n");
        if (tex_file && comments)
        {
            int amount = 0;
            for (; comments[amount]; amount++);
            const char* rand_comment = comments[rand() % amount];
            fprintf (tex_file, "%s\\begin{dmath*}\n%s = %s\n\\end{dmath*}",
                   rand_comment                                           ,
                   tree_node_to_tex(source, false)                        ,
                   tree_node_to_tex(new_node, false)                      );
        }
        is_changed = replace_node(source_pointer, new_node);
        //tree_node_dump(*source_pointer);
        tree_node_destruct(&new_node);
    }
    //////////////////
    ////RECURSION/////
    //////////////////
    bool is_left_changed = false, is_right_changed = false;
    if ((*source_pointer)->right)
        is_right_changed = tree_simplify__(&((*source_pointer)->right), tex_file, comments);
    if ((*source_pointer)->left)
        is_left_changed = tree_simplify__(&((*source_pointer)->left), tex_file, comments);
    return is_changed || is_left_changed || is_right_changed;
}

// comp_fu = do we want to compute functions
TreeNode* tree_calculate (const TreeNode* source,
                          bool comp_fu          ,
                          const FILE* tex_file  ,
                          const char** comments )
{
    ASSERT_OK(TreeNode, source);
    bool is_changed = true;
    TreeNode* calculated = tree_node_full_copy(source);

    if (!calculated)
        return NULL;
    while (is_changed)
    {
        //printf ("Iterating\n");
        //tree_node_show_dot(calculated);
        is_changed = tree_calculate__(&calculated, comp_fu, tex_file, comments);
    }

    return calculated;
}
TreeNode* tree_simplify (const TreeNode* source,
                         bool comp_fu          ,
                        FILE* tex_file  ,
                         const char** comments )
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
            is_calculated = tree_calculate__ (&simplified, comp_fu, tex_file, comments);
        }
        is_changed = tree_simplify__ (&simplified, tex_file, comments);
        //printf ("Tried to simplify:\n");
        //tree_node_show_dot(simplified);
    }

    //tree_node_dump_r(simplified);
    //tree_node_show_dot(simplified);

    return simplified;
}

#define _DIFF(source) build_part_derivative__(source, argument, tex_file, d_map)
#define _CPY(node) tree_node_full_copy(node)
TreeNode* build_part_derivative__ (const TreeNode* source,
                                   const char argument[] ,
                                   FILE* tex_file  ,
                                   const DifferMap* d_map)
{
    TreeNode* left = source->left;
    TreeNode* right = source->right;
    TreeNode* new_node = NULL;
    char comment[128] = {};

    ASSERT_OK(TreeNode, source);
    assert (argument);
    ASSERT_OK(DifferMap, d_map);
    switch (source->type)
    {
    case VAR:
        if (!strcmp(source->word, argument))
        {
            if (tex_file)
                strcat (comment, "Производная переменной дифференцирования:\n");
            float num = 1.0;
            new_node = _NUM(&num);
        }
        else
        {
            if (tex_file)
                strcat (comment, "Рассматриваем переменную как константу:\n");
            float nul = 0.0;
            new_node = _NUM(&nul);
        }
        break;
    case OP:
        switch (source->word[0])
        {
        case '+':
            if (tex_file)
                strcat (comment, "По формуле производной суммы:\n");
            new_node = _ADD(_DIFF(left), _DIFF(right));
            break;
        case '-':
            if (tex_file)
                strcat (comment, "По формуле производной разности:\n");
            new_node = _SUB(_DIFF(left), _DIFF(right));
            break;
        case '*':
            if (tex_file)
                strcat (comment, "Из выражения для производной произведения:\n");
            new_node = _ADD(_MUL(_DIFF(left),_CPY(right)),_MUL(_CPY(left),_DIFF(right)));
            break;
        case '/':
            ;
            if (tex_file)
                strcat (comment, "Находим производную частного:\n");
            float two = 2.0;
            TreeNode* power_2 = _NUM(&two);
            new_node = _DIV(_SUB(_MUL(_DIFF(left),_CPY(right)),_MUL(_CPY(left),_DIFF(right))),_POW(_CPY(right),power_2));
            break;
        case '^':
            ;
            if (tex_file)
                strcat (comment, "Вычисляя производную степенной функции:\n");
            if (right->type == NUM)
            {
                float new_pow = right->value - 1;

                new_node = _MUL(_NUM(&right->value),_POW(_CPY(left),_NUM(&new_pow)));
            }
            else
            {
                TreeNode* ln_node = _FU("ln", _CPY(left));
                new_node = _MUL(_CPY(source), _ADD(_MUL(_DIFF(right),ln_node),_MUL(_DIV(_DIFF(left), _CPY(left)), _CPY(right))));
            }
            break;
        }
        break;
    case NUM:
        ;
        if (tex_file)
                strcat (comment, "Производная константы равна 0:\n");
        float nul = 0.0;
        new_node = _NUM(&nul);
        break;
    case FU:
        ;
        if (tex_file)
                strcat (comment, "По теореме о производной сложной функции:\n");
        char* key = (char*)calloc(strlen(source->word) + 5, sizeof(char));
        strcat (key, source->word);
        strcat (key, "(x)");
        TreeNode* derivative = differ_map_get(d_map, key);
        free(key);
        replace_nodes(derivative, "x", tree_node_full_copy(left));

        new_node = _MUL(derivative, _DIFF(left));
        break;
    default:
        new_node = NULL;
    }

    if (new_node)
        fprintf (tex_file, "%s\\begin{dmath*}\n\\left(%s\\right)_{%s}^{\\prime} = %s\n\\end{dmath*}",
                           comment                                    ,
                           tree_node_to_tex(source, false)            ,
                           argument                                   ,
                           tree_node_to_tex(new_node, false)          );
   return new_node;
}
#undef _DIFF
#undef _CPY

TreeNode* build_part_derivative (const TreeNode* source ,
                                 const char argument[]  ,
                                 bool comp_fu           ,
                                 const DifferMap* deriv ,
                                 FILE* tex_file         )
{
    ASSERT_OK(TreeNode, source);
    assert (argument);
    ASSERT_OK(DifferMap, deriv);
    if (tex_file)
    {
        char line[] = "$\\blacktriangleright$";
        fprintf (tex_file, "%sНайдём частную производную следующего выражения по %s:\n \
                            \\begin{dmath*}\n \
                            \\left(%s\\right)_{%s}^{\\prime}\n \
                            \\end{dmath*}\n\n", line, argument, tree_node_to_tex(source, false), argument);
    }
    char** comments = comments_get("comments.txt");
    TreeNode* simple_source = tree_simplify(source, comp_fu, tex_file, (const char**)comments);
    if (tex_file)
    {
        char line[] = "$\\vartriangleright$";
        fprintf (tex_file, "%sУпростив, получим: \n \
                            \\begin{dmath*}\n \
                            \\left(%s\\right)_{%s}^{\\prime}\n \
                            \\end{dmath*}\n\n", line, tree_node_to_tex(source, false), argument);
        fprintf (tex_file, "%sПродифференцируем.\n\n", line);
    }

    TreeNode* part_derivative = build_part_derivative__ (simple_source, argument, tex_file, deriv);
    if (tex_file)
    {
        char line[] = "$\\vartriangleright$";
        /*fprintf (tex_file, "%sИтак, производная равна:\n \
                            \\begin{dmath*}\n \
                            \\left(%s\\right)_{%s}^{\\prime} = %s\n \
                            \\end{dmath*}\n\n", line, tree_node_to_tex(simple_source, false), argument, tree_node_to_tex(part_derivative, false));
        */
        fprintf (tex_file, "%sУпростим полученную производную.\n\n", line);
    }
    tree_node_destruct(&simple_source);
        //tree_node_show_dot(part_derivative);
    TreeNode* simplified = tree_simplify(part_derivative, comp_fu, tex_file, (const char**)comments);
    //tree_node_dump_r(simplified);
    //printf ("DONE\n");
    tree_node_destruct(&part_derivative);
    for (int i = 0; comments[i]; i++)
    {
        free(comments[i]);
    }
    free (comments);

    if (tex_file)
    {
        char line[] = "$\\vartriangleright$";
        fprintf (tex_file, "%sНаконец, получаем окончательный ответ:\n \
                            \\begin{dmath*}\n \
                            \\left(%s\\right)_{%s}^{\\prime} = %s\n \
                            \\end{dmath*}\n\n", line, tree_node_to_tex(source, false), argument, tree_node_to_tex(simplified, false));
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

//Last element of the array is NULL.
//tex_file should be NULL for silent mode
DifferMap* build_all_derivatives (const TreeNode* source ,
                                  bool comp_fu           ,
                                  DifferMap* deriv       ,
                                  FILE* tex_file         )
{
    ASSERT_OK(TreeNode, source);
    ASSERT_OK(DifferMap, deriv);
    if (tex_file)
        fprintf (tex_file, "Найдём частные производные по каждой из переменных.\n\n");
    ListHead variables = {};
    //tree_node_dump(source);
    if (!list_head_construct(&variables))
        return NULL;
    add_variables (source, &variables);
    DifferMap* derivatives = (DifferMap*)malloc(sizeof(*derivatives));
    if (!derivatives)
        return NULL;
    differ_map_construct(derivatives, variables.amount);
    int i = 0;
    for (ListElement* p = variables.first; p != NULL; p = p->next, i++)
    {
        //printf ("Addind derivative by <%s>\n", p->word);
        differ_map_add_node(derivatives, p->word, build_part_derivative(source, p->word, comp_fu, deriv, tex_file));
    }

    return derivatives;
}

TreeNode* tree_substitute (const TreeNode* source, const ValMap* variables)
{
    ASSERT_OK(TreeNode, source);
    ASSERT_OK(ValMap, variables);
    TreeNode* new_tree = tree_node_full_copy(source);
    for (size_t i = 0; i < variables->amount; i++)
    {
        TreeNode* replacement = _NUM (variables->values + i);
        //printf ("Replacing <%s> with %g\n", variables->keys[i], variables->values[i]);
        replace_nodes(new_tree, variables->keys[i], replacement);
        tree_node_destruct(&replacement);
    }

    return new_tree;
}

TreeNode* tree_build_error (const DifferMap* part_deriv)
{
    ASSERT_OK(DifferMap, part_deriv);
    if (part_deriv->amount < 1)
        return NULL;

    TreeNode* top_node = NULL;
    for (int i = 0; i < part_deriv->amount; i++)
    {
        #define _CPY(node) tree_node_full_copy(node)
        float two = 2;
        char* var_der = (char*)calloc(strlen(part_deriv->keys[i]) + 3, sizeof(*var_der));
        strcat (var_der, "d_");
        strcat (var_der, part_deriv->keys[i]);
        TreeNode* summant = _POW(_MUL(_CPY(part_deriv->values[i]), _VAR(var_der)), _NUM(&two));
        free (var_der);
        #undef _CPY
        if (i == 0)
            top_node = summant;
        else
            top_node = _ADD(top_node, summant);

    }
    float half = 0.5;
    top_node = _POW (top_node, _NUM(&half));
    //tree_node_show_dot(top_node);
    top_node = tree_simplify(top_node, true, NULL, NULL);
    return top_node;
}

void write_lab (FILE* info_file, FILE* tex_file)
{
    assert (tex_file);
    assert (info_file);
    Buffer info = {};
    buffer_construct_file(&info, info_file);

    char* eq = strchr (info.chars, '=');
    *eq = '\0';
    eq++;

    char* to_find = info.chars;
    char* formula = strtok (eq, "\n");
    //printf ("<%s> = <%s>\n", to_find, formula);

    ListHead vars_data = {};
    list_head_construct(&vars_data);
    char* var_info = strtok (NULL, "\n");
    while (var_info)
    {
        list_head_add(&vars_data, var_info);
        var_info = strtok (NULL, "\n");
    }
    ValMap vars = {};
    val_map_construct(&vars, vars_data.amount);
    for (ListElement* current = vars_data.first; current; current = current->next)
    {
        char* var_name = current->word;
        char* val_char = strrchr (current->word, '=');
        *val_char = '\0';
        val_char++;
        float value = 0;
        sscanf (val_char, "%f", &value);
        val_map_add(&vars, var_name, value);
    }
    list_head_destruct(&vars_data);

    tex_init(tex_file, "format.tex");
    TreeNode* root = tree_node_from_string(formula);
    tree_node_show_dot(root);
    fprintf (tex_file, "Вычислим значение %s по следующей формуле:\n%s", to_find, tree_node_to_tex(root, true));
    fprintf (tex_file, "Запишем данные в таблицу:\n\n");
    fprintf (tex_file, "\\begin{tabular}{|c|c|}\n \\hline\n Величина&Значение\\\\\n \\hline\n");
    for (int i = 0; i < vars.amount; i++)
        fprintf (tex_file, "$%s$&$%g$\\\\\n\\hline\n", vars.keys[i], vars.values[i]);
    fprintf (tex_file, "\\end{tabular}\n\n");
    fprintf (tex_file, "Найдём погрешность. Для этого построим частные производные.\n\n");

    DifferMap d_map = {};
    if (!differ_map_construct_filename(&d_map, "derivatives.txt"))
        return;

    DifferMap* derivatives = build_all_derivatives(root, true, &d_map, tex_file);
    if (!derivatives)
        return;

    TreeNode* root_calc = tree_substitute(root, &vars);
    TreeNode* root_calc_done = tree_calculate(root_calc, true, NULL, NULL);
    tree_node_destruct(&root);
    TreeNode* error_node = tree_build_error(derivatives);
    fprintf (tex_file, "Тогда погрешность может быть вычислена по следующей формуле:\n\n");
    fprintf (tex_file, "\\begin{dmath*}\n \
                        \\sigma_%s = %s\n \
                        \\end{dmath*}\n\n", to_find, tree_node_to_tex(error_node, false));

    TreeNode* result = tree_substitute(error_node, &vars);
    val_map_destruct(&vars);
    tree_node_destruct(&error_node);

    TreeNode* res_calc = tree_calculate(result, true, tex_file, NULL);
    fprintf (tex_file, "$\\rhd$Вычисляя, находим: ");
    fprintf (tex_file, "$\\sigma_%s = %g$.\n\n", to_find, res_calc->value);
    differ_map_destruct(derivatives);
    //tree_node_show_dot(res_calc);
    fprintf (tex_file, "$\\blacktriangleright$Таким образом получаем, что $%s = %.2g \\pm %.2g$\n\n", to_find, root_calc_done->value, res_calc->value);
    tex_finish(tex_file);
    tree_node_destruct(&root_calc);
    tree_node_destruct(&root_calc_done);
    tree_node_destruct(&res_calc);
    tree_node_destruct(&result);
    buffer_destruct(&info);
    free (derivatives);
}
#endif // DIFFER_H_INCLUDED
