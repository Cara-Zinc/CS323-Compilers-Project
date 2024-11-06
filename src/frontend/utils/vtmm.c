#include "vtmm.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

variable_t *init_vtmm()
{
    list_entry_t vtmm_head;
    variable_t *head = (variable_t *)malloc(sizeof(variable_t));
    head->name = "head";
    head->type = 4;
    head->value = 0;
    list_init(&vtmm_head);
    head->le = vtmm_head;
    return head;
}

bool add_variable(variable_t *vtmm_head, char *name, int type, void *value)
{
    list_entry_t *le = vtmm_head->le.next;
    variable_t *var = (variable_t *)malloc(sizeof(variable_t));
    var->name = name;
    var->type = type;
    var->value = value;
    list_init(&(var->le));
    list_add_after(le, &(var->le));
    vtmm_head->value = (void *)((int)vtmm_head->value + 1);
    return true;
}

bool remove_variable(variable_t *vtmm_head, char *name)
{
    list_entry_t *le = vtmm_head->le.next;
    while (le != &vtmm_head)
    {
        variable_t *var = to_struct(le, variable_t, le);
        if (strcmp(var->name, name) == 0)
        {
            list_del(le);
            free(var);
            vtmm_head->value = (void *)((int)vtmm_head->value - 1);
            return true;
        }
        le = le->next;
    }
    return false;
}

variable_t *get_variable(variable_t *vtmm_head, char *name)
{
    list_entry_t *le = vtmm_head->le.next;
    while (le != &vtmm_head)
    {
        variable_t *var = to_struct(le, variable_t, le);
        if (strcmp(var->name, name) == 0)
        {
            return var;
        }
        le = le->next;
    }
    printf("Variable %s not found\n", name);
    return (variable_t *)NULL;
}

variable_t *get_variable_by_index(variable_t *vtmm_head, int index)
{
    list_entry_t *le = vtmm_head->le.next;
    int i = 0;
    while (le != &vtmm_head)
    {
        if (i == index)
        {
            variable_t *var = to_struct(le, variable_t, le);
            return var;
        }
        le = le->next;
        i++;
    }
    printf("Index %d out of range\n", index);
    return (variable_t *)NULL;
}

int get_variable_count(variable_t *vtmm_head)
{
    return (int)vtmm_head->value;
}

void free_vtmm(variable_t *vtmm_head)
{
    list_entry_t *le = vtmm_head->le.next;
    while (le != &(vtmm_head->le))
    {
        variable_t *var = to_struct(le, variable_t, le);
        list_del(le);
        free(var);
        le = vtmm_head->le.next;
    }
    free(vtmm_head);
}

void print_vtmm(variable_t *vtmm_head)
{
    printf("====================================\n");
    printf("Variable Table has %d variables\n", (int)vtmm_head->value);
    printf("====================================\n");
    list_entry_t *le = vtmm_head->le.next;
    while (le != &(vtmm_head->le))
    {
        variable_t *var = to_struct(le, variable_t, le);
        print_variable(var, var->name);
        le = le->next;
    }
    printf("====================================\n");
}

void print_variable(variable_t *vtmm_head, char *name)
{
    variable_t *var = get_variable(vtmm_head, name);
    printf("Variable %s: ", var->name);
    switch (var->type)
    {
        case 0:
            printf("int %d\n", *((int *)var->value));
            break;
        case 1:
            printf("float %f\n", *((float *)var->value));
            break;
        case 2:
            printf("char %c\n", *((char *)var->value));
            break;
        case 3:
            printf("struct\n");
            break;
        default:
            printf("unknown type\n");
            break;
    }
}