#include "../mm/program_manager.h"
#include "ast.h"

ASTNode *type_handler(program_manager *pm, char *type) {
    return createASTLeaf("Specifier", type);
}

ASTNode *struct_spec_handler(program_manager *pm, ASTNode *StructSpecifier) {
    return createASTNode("Specifier", 1, StructSpecifier);
}

ASTNode *struct_def_handler(program_manager *pm, char *id, ASTNode *DefList) {
    if(DefList == NULL) {
        return createASTNode("StructSpecifier", 1, createASTLeaf("ID", id));
    }
    return createASTNode("StructSpecifier", 2, createASTLeaf("ID", id), DefList);
}

ASTNode *struct_member_handler(program_manager *pm, ASTNode *Specifier, ASTNode *DecList, ASTNode *Compst) {
    if(Compst == NULL) {
        return createASTNode("StructDef", 2, Specifier, DecList);
    }
    return createASTNode("StructDef", 3, Specifier, DecList, Compst);
}

ASTNode *struct_member_list_handler(program_manager *pm, ASTNode *StructDef, ASTNode *StructDefList) {
    if(StructDefList == NULL) {
        if(StructDef == NULL) {
            return NULL;
        }
        return createASTNode("StructDefList", 1, StructDef);
    }
    
    return createASTNode("StructDefList", 2, StructDef, StructDefList);
}
