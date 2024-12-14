#include "spec.h"

type_def *specifier_semantic(program_manager *pm, ASTNode *node)
{
    if (!node->children)
    {
        // fetch the type from the text
        const char *typename = node->text;
        if (!strcmp(typename, "int"))
        {
            return type_def_new(TYPE_INT, false);
        }
        else if (!strcmp(typename, "float"))
        {
            return type_def_new(TYPE_FLOAT, false);
        }
        else if (!strcmp(typename, "void"))
        {
            return type_def_new(TYPE_VOID, false);
        }
        else if (!strcmp(typename, "char"))
        {
            return type_def_new(TYPE_CHAR, false);
        }
        else
        {
            fprintf(stderr, "Error at line %zu: unknown type %s\n", node->line, typename);
            return NULL;
        }
    }

    if (alist_count(node->children) == 1)
    {
        if (alist_get(node->children, 0) && !strcmp(alist_get(node->children, 0)->nodeType, "StructSpecifier"))
        {
            return struct_specifier_semantic(pm, alist_get(node->children, 0));
        }
        else
        {
            fprintf(stderr, "Error at line %zu: unknown specifier\n", node->line);
            return NULL;
        }
    }

    return NULL;
}

type_def *struct_specifier_semantic(program_manager *pm, ASTNode *node)
{
    if (alist_count(node->children) == 1)
    {
        if (alist_get(node->children, 0) && !strcmp(alist_get(node->children, 0)->nodeType, "ID"))
        {
            // @TODO return the type of the struct
            type_def *type = program_manager_get_field(pm, alist_get(node->children, 0)->text)->type_spec;
            if (!type)
            {
                // return NULL to state that the struct is not defined yet
                // BUT we do not know whether this specifier is a definition or a reference
                return NULL;
            }
            return type;
        }
    }
    else if (alist_count(node->children) == 2)
    {
        ASTNode *id = alist_get(node->children, 0);
        if (id && !strcmp(id->nodeType, "ID"))
        {
            // @TODO return the type of the struct
            // @bug: segfault here
            type_def *type = program_manager_get_field(pm, id->text)->type_spec;
            if (type)
            {
                fprintf(stderr, "Error at line %zu: redefinition of struct %s\n", node->line, id->text);
                struct_def *s = program_manager_create_struct_invalid(pm);
                struct_def_list_semantic(pm, alist_get(node->children, 1), s->id);

                return NULL;
            }
            else
            {
                struct_def *s = program_manager_create_struct(pm, id->text);
                struct_def_list_semantic(pm, alist_get(node->children, 1), s->id);
            }
        }
    }

    return NULL;
}

void struct_def_list_semantic(program_manager *pm, ASTNode *struct_def_list, type_id struct_id)
{
    // @TODO

    ASTNode *struct_def = alist_get(struct_def_list->children, 0);

    if (!struct_def)
    {
        // this is the end of the list
        return;
    }
    else
    {
        ASTNode *struct_def_list_1 = alist_get(struct_def_list->children, 1);
        struct_def_semantic(pm, struct_def);
        struct_def_list_semantic(pm, struct_def_list_1, struct_id);
    }
}

void struct_def_semantic(program_manager *pm, ASTNode *struct_def)
{
    // @TODO
    program_manager_current_scope(pm);
    if (alist_count(struct_def->children) == 2)
    {
        ASTNode *specifier = alist_get(struct_def->children, 0);
        type_def *type = specifier_semantic(pm, specifier);
        ASTNode *dec_list = alist_get(struct_def->children, 1);
        declist_semantic(pm, dec_list, type);
    }
    else if (alist_count(struct_def->children) == 1)
    {
        ASTNode *fundef = alist_get(struct_def->children, 0);
        func_def *func = fundef_semantic(pm, fundef);
        func_def_free(func);
    }
}
