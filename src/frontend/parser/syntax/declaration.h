#include "../mm/program_manager.h"
#include "ast.h"

/*
 * Handles the following rules:
 * VarDec : ID
 *        | VarDec LB INT RB
 *        ;
 */

// For VarDec -> ID
ASTNode *VarDec_ID_handler(program_manager *pm, char *id, size_t line)
{
    ASTNode *leaf = createASTLeaf("ID", line, id);
    ASTNode *node = createASTNode("Vardec", line, 1, leaf);
    return node;
}

// For VarDec -> VarDec LB INT RB
ASTNode *VarDec_Array_handler(program_manager *pm, ASTNode *VarDec, int int_val, size_t line)
{
    // convert INT to string
    char *INT_str = malloc(12);
    sprintf(INT_str, "%d", int_val);
    return createASTNode("VarDec", line, 4, VarDec, createASTLeaf("LB", line, "["), createASTLeaf("INT", line, INT_str), createASTLeaf("RB", line, "]"));
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
ASTNode *VarList_ParamDec_handler(program_manager *pm, ASTNode *ParamDec, size_t line)
{
    return createASTNode("VarList", line, 1, ParamDec);
}

// For VarList -> ParamDec COMMA VarList
// Example: int a, int b
ASTNode *VarList_ParamDec_Comma_VarList_handler(program_manager *pm, ASTNode *ParamDec, ASTNode *VarList, size_t line)
{
    return createASTNode("VarList", line, 2, ParamDec, VarList);
}

// For ParamDec -> Specifier VarDec
ASTNode *ParamDec_handler(program_manager *pm, ASTNode *Specifier, ASTNode *VarDec, size_t line)
{
    return createASTNode("ParamDec", line, 2, Specifier, VarDec);
}

// For FunDec -> ID LP VarList RP | ID LP RP
ASTNode *FunDec_handler(program_manager *pm, char *id, ASTNode *VarList, size_t line)
{
    if (VarList == NULL)
    {
        return createASTNode("FunDec", line, 1, createASTLeaf("ID", line, id));
    }
    return createASTNode("FunDec", line, 2, createASTLeaf("ID", line, id), VarList);
}

// For DecList -> Dec | Dec COMMA DecList
ASTNode *DecList_handler(program_manager *pm, ASTNode *Dec, ASTNode *DecList, size_t line)
{
    if (DecList == NULL)
    {
        return createASTNode("DecList", line, 1, Dec);
    }
    return createASTNode("DecList", line, 2, Dec, DecList);
}

// For Dec -> VarDec | VarDec ASSIGN Exp
ASTNode *Dec_handler(program_manager *pm, ASTNode *varDec, ASTNode *exp, size_t line)
{
    if (exp == NULL)
    {
        return createASTNode("Dec", line, 1, varDec);
    }
    return createASTNode("Dec", line, 3, varDec, createASTLeaf("ASSIGN", line, "="), exp);
}
