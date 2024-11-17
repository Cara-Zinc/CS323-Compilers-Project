#include "../mm/program_manager.h"
#include "ast.h"

ASTNode *ext_def_list_handler(program_manager *pm, ASTNode *ExtDef, ASTNode *ExtDefList) {
    return createASTNode("ExtDefList", 2, ExtDef, ExtDefList);
}

ASTNode *ext_def_dec_handler(program_manager *pm, ASTNode *Specifier, ASTNode *ExtDecList) {
    return createASTNode("ExtDef", 2, Specifier, ExtDecList);
}

ASTNode *ext_def_struct_handler(program_manager *pm, ASTNode *StructSpecifier) {
    return createASTNode("ExtDef", 1, StructSpecifier);
}

ASTNode *ext_def_func_handler(program_manager *pm, ASTNode *Specifier, ASTNode *FunDec, ASTNode *CompSt) {
    ASTNode *node = createASTNode("ExtDef", 3, Specifier, FunDec, CompSt);
    return node;
}

ASTNode *ext_dec_list_handler(program_manager *pm, ASTNode *VarDec, ASTNode *ExtDecList) {
    if (ExtDecList == NULL) {
        return createASTNode("ExtDecList", 1, VarDec);
    }
    return createASTNode("ExtDecList", 2, VarDec, ExtDecList);
}


