#include "../mm/program_manager.h"
#include "ast.h"

/*
 * Handles the following rules:
 * VarDec : ID
 *        | VarDec LB INT RB
 *        ;
 */

// For VarDec -> ID
ASTNode *VarDec_ID_handler(program_manager *pm, char *id)
{
    ASTNode *leaf = createASTLeaf("ID", id);
    ASTNode *node = createASTNode("Vardec", 1, leaf);
    return node;
}

// For VarDec -> VarDec LB INT RB
ASTNode *VarDec_Array_handler(program_manager *pm, ASTNode *VarDec, int int_val)
{
    // convert INT to string
    char *INT_str = malloc(12);
    sprintf(INT_str, "%d", int_val);
    return createASTNode("VarDec", 4, VarDec, createASTLeaf("LB", "["), createASTLeaf("INT", INT_str), createASTLeaf("RB", "]"));
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

// For ParamDec -> Specifier VarDec
ASTNode *ParamDec_handler(program_manager *pm, ASTNode *Specifier, ASTNode *VarDec)
{
    return createASTNode("ParamDec", 2, Specifier, VarDec);
}

// For FunDec -> ID LP VarList RP | ID LP RP
ASTNode *FunDec_handler(program_manager *pm, char *id, ASTNode *VarList)
{
    if (VarList == NULL)
    {
        return createASTNode("FunDec", 3, createASTLeaf("LP", "LP"), createASTLeaf("ID", id), createASTLeaf("RP", "RP"));
    }
    return createASTNode("FunDec", 4, createASTLeaf("ID", id), createASTLeaf("LP", "LP"), VarList, createASTLeaf("RP", "RP"));
}

// For DecList -> Dec | Dec COMMA DecList
ASTNode *DecList_handler(program_manager *pm, ASTNode *Dec, ASTNode *DecList)
{
    if (DecList == NULL)
    {
        return createASTNode("DecList", 1, Dec);
    }
    return createASTNode("DecList", 2, Dec, DecList);
}

// For Dec -> VarDec | VarDec ASSIGN Exp
ASTNode *Dec_handler(program_manager *pm, ASTNode *varDec, ASTNode *exp)
{
    if (exp == NULL)
    {
        return createASTNode("Dec", 1, varDec);
    }
    return createASTNode("Dec", 3, varDec, createASTLeaf("ASSIGN", "="), exp);
}
