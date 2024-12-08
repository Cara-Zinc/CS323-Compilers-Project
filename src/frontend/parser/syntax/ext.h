#include "../mm/program_manager.h"
#include "ast.h"

ASTNode *ext_def_list_handler(program_manager *pm, ASTNode *ExtDef, ASTNode *ExtDefList, size_t line) {
    return createASTNode("ExtDefList", line, 2, ExtDef, ExtDefList);
}

ASTNode *ext_def_dec_handler(program_manager *pm, ASTNode *Specifier, ASTNode *ExtDecList, size_t line) {
    return createASTNode("ExtDef", line, 2, Specifier, ExtDecList);
}

ASTNode *ext_def_struct_handler(program_manager *pm, ASTNode *StructSpecifier, size_t line) {
    return createASTNode("ExtDef", line, 1, StructSpecifier);
}

ASTNode *ext_def_func_handler(program_manager *pm, ASTNode *Specifier, ASTNode *FunDec, ASTNode *CompSt, size_t line) {
    ASTNode *node = createASTNode("ExtDef", line, 3, Specifier, FunDec, CompSt);
    return node;
}

ASTNode *ext_dec_list_handler(program_manager *pm, ASTNode *VarDec, ASTNode *ExtDecList, size_t line) {
    if (ExtDecList == NULL) {
        return createASTNode("ExtDecList", line, 1, VarDec);
    }
    return createASTNode("ExtDecList", line, 2, VarDec, ExtDecList);
}

ASTNode *program_handler(program_manager *pm, ASTNode *ExtDefList, size_t line) {
    if(ExtDefList == NULL) {
        return createASTLeaf("Program", line, NULL);
    }
    return createASTNode("Program", line, 1, ExtDefList);
}

