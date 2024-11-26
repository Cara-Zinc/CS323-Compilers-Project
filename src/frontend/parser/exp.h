#include "../mm/program_manager.h"
#include "ast.h"

ASTNode *exp_id_handler(program_manager *pm, char *name)
{
    // if (!program_manager_get_field(pm, name))
    // {
    //     //printf("Error: variable %s not declared\n", name);
    //     // exit(1);
    // }
    ASTNode *leaf = createASTLeaf("ID", name);
    ASTNode *node = createASTNode("Exp", 1, leaf);
    return node;
}

ASTNode *exp_bi_op_handler(program_manager *pm, ASTNode *left, char *op, ASTNode *right)
{
    // char *node_type = "EXP";
    // if (left == NULL || right == NULL)
    // {
    //     return NULL;
    // }

    // // check usage before declaration
    // if (!strcmp(left->nodeType, "ID"))
    // {
    //     if (!program_manager_get_field(pm, left->text))
    //     {
    //         printf("Error: variable %s not declared\n", left->text);
    //         // exit(1);
    //     }
    // }
    // if (!strcmp(right->nodeType, "ID"))
    // {
    //     if (!program_manager_get_field(pm, right->text))
    //     {
    //         printf("Error: variable %s not declared\n", right->text);
    //         // exit(1);
    //     }
    // }

    // // check type compatibility for binary operations
    // if (strcmp(op, "="))
    // {
    //     if (strcmp(left->nodeType, "ID") && strcmp(right->nodeType, "ID"))
    //     {
    //         if (strcmp(left->nodeType, right->nodeType))
    //         {
    //             printf("Error: type mismatch\n");
    //             // exit(1);
    //         }
    //     }
    // }
    // else if (strcmp(op, "+") || strcmp(op, "-") || strcmp(op, "*"))
    // {
    //     // check if both operands are INT or FLOAT
    //     if (strcmp(left->nodeType, "INT") && strcmp(left->nodeType, "FLOAT") && strcmp(left->nodeType, "CHAR"))
    //     {
    //         printf("Error: type mismatch on left expression\n");
    //     }
    //     if (strcmp(right->nodeType, "INT") && strcmp(right->nodeType, "FLOAT") && strcmp(right->nodeType, "CHAR"))
    //     {
    //         printf("Error: type mismatch on right expression\n");
    //     }

    //     // CHAR + INT = CHAR, CHAR + FLOAT = FLOAT, CHAR + CHAR = CHAR, INT + CHAR = CHAR,INT + INT = INT, INT + FLOAT = FLOAT, FLOAT + FLOAT = FLOAT
    //     // TODO: Type promotion
    //     node_type = "EXP";
    // }
    // else if (strcmp(op, "/"))
    // {
    //     // check if both operands are INT or FLOAT
    //     if (strcmp(left->nodeType, "INT") && strcmp(left->nodeType, "FLOAT") && strcmp(left->nodeType, "CHAR"))
    //     {
    //         printf("Error: type mismatch on left expression\n");
    //     }
    //     if (strcmp(right->nodeType, "INT") && strcmp(right->nodeType, "FLOAT") && strcmp(right->nodeType, "CHAR"))
    //     {
    //         printf("Error: type mismatch on right expression\n");
    //     }

    //     // check divide-by-zero, such as 0, 0.0f, .0f, 0.0, .0, 0x0
    //     if (strcmp(right->text, "0") && strcmp(right->text, "0.0f") && strcmp(right->text, ".0f") && strcmp(right->text, "0.0") && strcmp(right->text, ".0") && strcmp(right->text, "0x0"))
    //     {
    //         printf("Error: divide by zero\n");
    //         // exit(1);
    //     }
    //     node_type = "EXP";

    //     // CHAR / INT = CHAR, CHAR / FLOAT = FLOAT, CHAR / CHAR = CHAR, INT / CHAR = CHAR,INT / INT = INT, INT / FLOAT = FLOAT, FLOAT / FLOAT = FLOAT
    //     // TODO: Type promotion
    // }
    // else if (strcmp(op, "<") || strcmp(op, ">") || strcmp(op, "<=") || strcmp(op, ">="))
    // {
    //     if (strcmp(left->nodeType, "INT") && strcmp(left->nodeType, "FLOAT") && strcmp(left->nodeType, "CHAR"))
    //     {
    //         printf("Error: type mismatch on left expression\n");
    //     }
    //     if (strcmp(right->nodeType, "INT") && strcmp(right->nodeType, "FLOAT") && strcmp(right->nodeType, "CHAR"))
    //     {
    //         printf("Error: type mismatch on right expression\n");
    //     }
    //     // TODO: Type promotion
    //     node_type = "BOOL";
    // }
    // else if (strcmp(op, "==") || strcmp(op, "!="))
    // {
    //     if (strcmp(left->nodeType, right->nodeType))
    //     {
    //         printf("Error: type mismatch\n");
    //         // exit(1);
    //     }
    // }
    // else if (strcmp(op, "&&") || strcmp(op, "||"))
    // {
    //     if (strcmp(left->nodeType, "BOOL") && strcmp(right->nodeType, "BOOL"))
    //     {
    //         printf("Error: type mismatch\n");
    //         // exit(1);
    //     }
    // }

    ASTNode *node = createASTNode("Exp", 3, left, createASTLeaf("operator", op), right);
    // node.text = left->text + op + right->text; we try to merge the text of the children
    // int len = strlen(left->text) + strlen(op) + strlen(right->text) + 1;
    // node->text = (char *)malloc(len * sizeof(char));
    // if (node->text != NULL)
    // {
    //     if (left->text && right->text && op)
    //     {
    //         strcpy(node->text, left->text);
    //         strcat(node->text, op);
    //         strcat(node->text, right->text);
    //     }
    //     else
    //     {
    //         printf("Error: Invalid text in expression nodes\n");
    //         free(node->text);
    //         return NULL;
    //     }
    // }

    return node;
}

ASTNode *exp_assign_handler(program_manager *pm, ASTNode *left, ASTNode *right)
{
    // if (left == NULL || right == NULL)
    // {
    //     printf("Error: Invalid assignment, both sides are NULL\n");
    //     return NULL;
    // }

    // check usage before declaration
    // if (!strcmp(left->nodeType, "ID"))
    // {
    //     if (!program_manager_get_field(pm, left->text))
    //     {
    //         printf("Error: variable %s not declared\n", left->text);
    //         // exit(1);
    //     }
    // }
    // else
    // {
    //     printf("Error: Invalid left expression in assignment, should be ID\n");
    //     // exit(1);
    // }

    // if (!strcmp(right->nodeType, "EXP"))
    // {
    //     // TODO: check if right expression is valid
    //     // check if type of right expression is compatible with left expression
    //     if (strcmp(left->nodeType, right->nodeType))
    //     {
    //         printf("Error: type mismatch\n");
    //         // exit(1);
    //     }
    // }

    ASTNode *node = createASTNode("Exp", 3, left, createASTLeaf("op", "="), right);
    // char *text = (char *)malloc(strlen(left->text) + strlen(right->text) + 2);
    // if (text != NULL)
    // {
    //     if (left->text && right->text)
    //     {
    //         strcpy(text, left->text);
    //         strcat(text, "=");
    //         strcat(text, right->text);
    //         node->text = text;
    //     }
    //     else
    //     {
    //         printf("Error: Invalid text in expression nodes\n");
    //         free(text);
    //         return NULL;
    //     }
    // }
    // else
    // {
    //     printf("Error: Memory allocation failed\n");
    //     return NULL;
    //
    return node;
}

ASTNode *exp_neg_handler(program_manager *pm, ASTNode *child)
{
    // if (child == NULL)
    // {
    //     printf("Error: Invalid negation, child is NULL\n");
    //     return NULL;
    // }

    // // check usage before declaration
    // if (!strcmp(child->nodeType, "ID"))
    // {
    //     if (!program_manager_get_field(pm, child->text))
    //     {
    //         //printf("Error: variable %s not declared\n", child->text);
    //         // exit(1);
    //     }
    // }

    // if (strcmp(child->nodeType, "INT") && strcmp(child->nodeType, "FLOAT"))
    // {
    //     printf("Error: type mismatch, should be INT or FLOAT\n");
    //     // exit(1);
    // }

    ASTNode *node = createASTNode("Exp", 2, createASTLeaf("op", "-"), child);
    // char *text = (char *)malloc(strlen(child->text) + 2);
    // if (text != NULL)
    // {
    //     if (child->text)
    //     {
    //         strcpy(text, "-");
    //         strcat(text, child->text);
    //         node->text = text;
    //     }
    //     else
    //     {
    //         printf("Error: Invalid text in expression nodes\n");
    //         free(text);
    //         return NULL;
    //     }
    // }
    // else
    // {
    //     printf("Error: Memory allocation failed\n");
    //     return NULL;
    // }
    return node;
}

ASTNode *exp_not_handler(program_manager *pm, ASTNode *child)
{
    // if (child == NULL)
    // {
    //     printf("Error: Invalid negation, child is NULL\n");
    //     return NULL;
    // }

    // // check usage before declaration
    // if (!strcmp(child->nodeType, "ID"))
    // {
    //     if (!program_manager_get_field(pm, child->text))
    //     {
    //         printf("Error: variable %s not declared\n", child->text);
    //         // exit(1);
    //     }
    // }

    // if (strcmp(child->nodeType, "BOOL"))
    // {
    //     printf("Error: type mismatch, should be BOOL\n");
    //     // exit(1);
    // }

    ASTNode *node = createASTNode("Exp", 2, createASTLeaf("op", "!"), child);
    // char *text = (char *)malloc(strlen(child->text) + 2);
    // if (text != NULL)
    // {
    //     if (child->text)
    //     {
    //         strcpy(text, "!");
    //         strcat(text, child->text);
    //         node->text = text;
    //     }
    //     else
    //     {
    //         printf("Error: Invalid text in expression nodes\n");
    //         free(text);
    //         return NULL;
    //     }
    // }
    // else
    // {
    //     printf("Error: Memory allocation failed\n");
    //     return NULL;
    // }
    return node;
}

ASTNode *exp_unary_op_handler(program_manager *pm, char *op, ASTNode *child)
{
    if(!strcmp(op, "MINUS"))
    {
        return exp_neg_handler(pm, child);
    }
    else if(!strcmp(op, "NOT"))
    {
        return exp_not_handler(pm, child);
    }
    else if(!strcmp(op, "PLUS"))
    {
        return createASTNode("Exp", 2, createASTLeaf("op","PLUS"), child);
    }
    else
    {
        printf("Error: Invalid unary operator\n");
        return NULL;
    }
}

ASTNode *exp_primitive_handler(program_manager *pm, char *type, char *text)
{
    ASTNode *node = createASTLeaf(type, text);
    return node;
}

ASTNode *exp_func_handler(program_manager *pm, char *id, ASTNode *Args)
{
    ASTNode *node = createASTNode("Exp", 2, createASTLeaf("ID", id), Args);
    return node;
}

ASTNode *exp_array_handler(program_manager *pm, ASTNode *Exp1, ASTNode *Exp2)
{
    if(Exp2 == NULL)
    {
        return createASTNode("Exp", 3, Exp1, createASTLeaf("LB", "["), createASTLeaf("RB", "]"));
    }
    ASTNode *node = createASTNode("Exp", 4, Exp1, createASTLeaf("LB", "["), Exp2, createASTLeaf("RB", "]"));
    return node;
}


ASTNode *exp_struct_handler(program_manager *pm, ASTNode *Exp, char *id)
{
    // if(Exp == NULL)
    // {
    //     return createASTNode("Exp", 1, createASTLeaf("ID", id));
    // }
    ASTNode *node = createASTNode("Exp", 2, Exp, createASTLeaf("ID", id));
    return node;
}

ASTNode *exp_int_handler(program_manager *pm, int text)
{
    // convert int to string
    char *str = (char *)malloc(12 * sizeof(char));
    if (str != NULL)
    {
        sprintf(str, "%d", text);
    }
    else
    {
        printf("Error: Memory allocation failed\n");
        return NULL;
    }
    ASTNode *leaf = createASTLeaf("INT", str);
    ASTNode *node = createASTNode("Exp", 1, leaf);
    return node;
}

ASTNode *exp_float_handler(program_manager *pm, float text)
{
    // convert float to string
    char *str = (char *)malloc(12 * sizeof(char));
    if (str != NULL)
    {
        sprintf(str, "%f", text);
    }
    else
    {
        printf("Error: Memory allocation failed\n");
        return NULL;
    }
    ASTNode *leaf = createASTLeaf("FLOAT", str);
    ASTNode *node = createASTNode("Exp", 1, leaf);
    return node;
}

ASTNode *exp_char_handler(program_manager *pm, char text)
{
    // convert char to string
    char *str = (char *)malloc(2 * sizeof(char));
    if (str != NULL)
    {
        str[0] = text;
        str[1] = '\0';
    }
    else
    {
        printf("Error: Memory allocation failed\n");
        return NULL;
    }
    ASTNode *leaf = createASTLeaf("CHAR", str);
    ASTNode *node = createASTNode("Exp", 1, leaf);
    return node;
}