#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/*

This file provide service for Variable Table Memory Management


*/
typedef struct variable
{
    char *name;
    int type;
    void *value;
    list_entry_t le;
} variable_t;
//TODO: refactor to generic type

// variable type is design as follows:
// 0: int
// 1: float
// 2: char
// 3: struct
// 4: header, an integer value stores the number of variables in the variable table


/*
Following functions are used to manage the variable table:
1. init_vtmm: initialize a variable table
2. add_variable: add a variable a the variable table
3. remove_variable: remove a variable from the variable table
4. get_variable: get a variable from the variable table by name
5. get_variable_by_index: get a variable from the variable table by index
6. get_variable_count: get the number of variables in the variable table
7. free_vtmm: free the variable table
8. print_vtmm: print the variable table
9. print_variable: print a variable

*/
variable_t *init_vtmm();
bool add_variable(variable_t *vtmm_head, char *name, int type, void *value);
bool remove_variable(variable_t *vtmm_head, char *name);
variable_t *get_variable(variable_t *vtmm_head, char *name);
variable_t *get_variable_by_index(variable_t *head, int index);
int get_variable_count(variable_t *vtmm_head);
void free_vtmm(variable_t *vtmm_head);
void print_vtmm(variable_t *vtmm_head);
void print_variable(variable_t *vtmm_head, char *name);
