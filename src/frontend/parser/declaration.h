#include "../mm/program_manager.h"
#include "ast.h"

/* 
 * Handles the following rules:
 * VarDec : ID
 *        | VarDec LB INT RB
 *        ;
 */

// For VarDec -> ID
ASTNode *VarDec_ID_handler(program_manager *pm, char *ID)
{
    return createASTLeaf("ID", ID);
}

// For VarDec -> VarDec LB INT RB
ASTNode *VarDec_Array_handler(program_manager *pm, ASTNode *VarDec, int INT)
{
    return createASTNode("VarDec", 2, VarDec, createASTLeaf("INT", INT));
}

/*
 * Handles the following rules when declaring a function: 
 * VarList : ParamDec COMMA VarList
 *         | ParamDec
 *         ;
 *
 * ParamDec : Specifier VarDec
 *          ;
 */

// For VarList -> ParamDec
// Example: int a
ASTNode *VarList_ParamDec_handler(program_manager *pm, ASTNode *ParamDec)
{
    return createASTNode("VarList", 1, ParamDec);
}

// For VarList -> ParamDec COMMA VarList
// Example: int a, int b
ASTNode *VarList_ParamDec_Comma_VarList_handler(program_manager *pm, ASTNode *ParamDec, ASTNode *VarList)
{
    return createASTNode("VarList", 2, ParamDec, VarList);
}