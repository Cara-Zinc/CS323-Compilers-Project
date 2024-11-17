#include "../mm/program_manager.h"
#include "ast.h"

ASTNode *type_handler(program_manager *pm, char *type) {
    return createASTLeaf("Specifier", type);
}

ASTNode *struct_spec_handler(program_manager *pm, ASTNode *StructSpecifier) {
    return createASTNode("Specifier", 1, StructSpecifier);
}

ASTNode *struct_def_handler(program_manager *pm, ASTNode *id, ASTNode *DefList) {
    if(DefList == NULL) {
        return createASTNode("StructSpecifier", 1, id);
    }
    return createASTNode("StructSpecifier", 2, id, DefList);
}