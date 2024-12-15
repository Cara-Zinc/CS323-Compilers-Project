#include "../mm/program_manager.h"
#include "ast.h"

ASTNode *type_handler(program_manager *pm, char *type, size_t line) {
    return createASTLeaf("Specifier", line, type);
}

ASTNode *struct_spec_handler(program_manager *pm, ASTNode *StructSpecifier, size_t line) {
    return createASTNode("Specifier", line, 1, StructSpecifier);
}

ASTNode *struct_def_handler(program_manager *pm, char *id, ASTNode *DefList, size_t line) {
    if(DefList == NULL) {
        return createASTNode("StructSpecifier", line, 1, createASTLeaf("ID", line, id));
    }
    return createASTNode("StructSpecifier", line, 2, createASTLeaf("ID", line, id), DefList);
}

ASTNode *struct_member_handler(program_manager *pm, ASTNode *Specifier, ASTNode *DecList, ASTNode *Compst, size_t line) {
    if(Compst == NULL) {
        return createASTNode("StructDef", line, 2, Specifier, DecList);
    }
    return createASTNode("StructDef", line, 3, Specifier, DecList, Compst);
}

ASTNode *struct_fundef_handler(program_manager *pm, ASTNode *FunDef, size_t line) {
    return createASTNode("FunDef", line, 1, FunDef);
}

ASTNode *struct_member_list_handler(program_manager *pm, ASTNode *StructDef, ASTNode *StructDefList, size_t line) {
    if(StructDefList == NULL) {
        if(StructDef == NULL) {
            return NULL;
        }
        return createASTNode("StructDefList", line, 1, StructDef);
    }

    return createASTNode("StructDefList", line, 2, StructDef, StructDefList);
}
