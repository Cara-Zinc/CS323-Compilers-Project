#include "extdef.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include <cstring>
#include "program_manager.h" // for symbol and type management

extern llvm::Module *TheModule;
extern llvm::LLVMContext TheContext;
extern program_manager *pm; // Assume you have a global or passed-in program_manager

namespace extdef {

// Forward declarations of helper functions
static llvm::Type *llvm_type_from_type_def(type_def *tdef);
static type_def *ast_specifier_to_type_def(program_manager *pm, ASTNode *specifierNode);
static void handle_global_var_decl(llvm::IRBuilder<> &Builder, type_def *tdef, ASTNode *extDecList);
static void handle_function_def(llvm::IRBuilder<> &Builder, type_def *returnTypeDef, ASTNode *funDec, ASTNode *compSt);

void handle_extdef(llvm::IRBuilder<> &Builder, ASTNode *node) {
    // ExtDef → Specifier ExtDecList SEMI
    //        | Specifier SEMI
    //        | Specifier FunDec CompSt
    //
    //  node->children[0] = Specifier
    //  next children vary depending on production

    if (!node || node->childCount == 0) return;
    ASTNode *specifierNode = node->children[0];

    // Convert AST specifier into a type_def*
    type_def *tdef = ast_specifier_to_type_def(pm, specifierNode);
    if (!tdef) {
        // Handle error: unknown type
        return;
    }

    if (node->childCount >= 3 &&
        strcmp(node->children[1]->nodeType, "ExtDecList") == 0 &&
        strcmp(node->children[2]->nodeType, "SEMI") == 0) {
        // ExtDef → Specifier ExtDecList SEMI
        ASTNode *extDecList = node->children[1];
        handle_global_var_decl(Builder, tdef, extDecList);

    } else if (node->childCount == 2 &&
               strcmp(node->children[1]->nodeType, "SEMI") == 0) {
        // ExtDef → Specifier SEMI
        // This might be a type definition (e.g., "struct Foo;") or a no-op.
        // If it's a struct or typedef definition, you might record it in pm.
        // If it's just a redundant specifier, you may not need to do anything.
        // Example:
        // if tdef is a struct type without fields defined yet, this might be a forward declaration of a struct.
        // In that case, program_manager may have already recorded it when ast_specifier_to_type_def was called.

    } else if (node->childCount == 3 &&
               strcmp(node->children[1]->nodeType, "FunDec") == 0 &&
               strcmp(node->children[2]->nodeType, "CompSt") == 0) {
        // ExtDef → Specifier FunDec CompSt
        ASTNode *funDec = node->children[1];
        ASTNode *compSt = node->children[2];
        handle_function_def(Builder, tdef, funDec, compSt);

    } else {
        // Unrecognized production, handle error.
    }
}

static type_def *ast_specifier_to_type_def(program_manager *pm, ASTNode *specifierNode) {
    // This function should look at the specifierNode->text or children to determine the type.
    // For example:
    if (strcmp(specifierNode->text, "int") == 0) {
        // Create or retrieve a type_def for int
        type_def *int_type = find_or_create_int_type(pm); // You must implement find_or_create_int_type
        return int_type;
    } else if (strcmp(specifierNode->text, "float") == 0) {
        type_def *float_type = find_or_create_float_type(pm);
        return float_type;
    }
    // Handle structs, user-defined types, etc.
    // If it's a struct definition: program_manager_create_struct(...) or program_manager_get_struct(...)
    // If it's a user-defined type: look it up in pm.
    return nullptr;
}

static llvm::Type *llvm_type_from_type_def(type_def *tdef) {
    // Convert a type_def* to an LLVM Type.
    // For instance, if tdef represents int, return llvm::Type::getInt32Ty(TheContext).
    // This depends on how your type_def is structured.
    // For simplicity:
    if (/* tdef is int */) {
        return llvm::Type::getInt32Ty(TheContext);
    } else if (/* tdef is float */) {
        return llvm::Type::getFloatTy(TheContext);
    }
    // Handle structs and others.
    return llvm::Type::getVoidTy(TheContext);
}

static void handle_global_var_decl(llvm::IRBuilder<> &Builder, type_def *tdef, ASTNode *extDecList) {
    // extDecList might be a comma-separated list of variables.
    // For each variable:
    //   1. Extract the variable name from the AST.
    //   2. Create it in program_manager using program_manager_create_field(pm, name, tdef).
    //   3. Generate a global variable in LLVM IR.

    llvm::Type *varType = llvm_type_from_type_def(tdef);
    for (int i = 0; i < extDecList->childCount; i++) {
        ASTNode *varNode = extDecList->children[i];
        const char *varName = varNode->text;

        // Record in symbol table
        field_def *field = program_manager_create_field(pm, (char*)varName, tdef);

        // Create a global variable in LLVM
        new llvm::GlobalVariable(
            *TheModule,
            varType,
            false,
            llvm::GlobalValue::ExternalLinkage,
            llvm::Constant::getNullValue(varType),
            varName
        );
    }
}

static void handle_function_def(llvm::IRBuilder<> &Builder, type_def *returnTypeDef, ASTNode *funDec, ASTNode *compSt) {
    // funDec contains function name and parameters
    const char *funcName = funDec->children[0]->text;

    // For parameters:
    // Extract each parameter's type_def and name, record in program_manager, and collect their LLVM types.
    std::vector<llvm::Type*> paramTypes;
    // You’ll also want to record the parameters in pm:
    // func_def *fdef = program_manager_create_func(pm, (char*)funcName, returnTypeDef);

    func_def *fdef = program_manager_create_func(pm, (char*)funcName, returnTypeDef);

    // If the function has parameters, you'd do something like:
    // for each parameter:
    //   type_def *paramType = ...
    //   field_def *paramField = program_manager_create_field(pm, paramName, paramType);
    //   paramTypes.push_back(llvm_type_from_type_def(paramType));

    llvm::Type *retType = llvm_type_from_type_def(returnTypeDef);
    llvm::FunctionType *funcType = llvm::FunctionType::get(retType, paramTypes, false);
    llvm::Function *function = llvm::Function::Create(
        funcType,
        llvm::GlobalValue::ExternalLinkage,
        funcName,
        TheModule
    );

    // Create a new basic block for IR generation
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "entry", function);
    Builder.SetInsertPoint(BB);

    // Set up arguments (if any) - record them in pm as local fields, etc.

    // Generate IR for CompSt (the function body).
    // This would involve recursively processing child nodes and generating IR for statements.
    // As you process statements that declare variables, you’d use program_manager_create_field to add them.
    // Then create alloca instructions for them in LLVM to reserve stack space.

    // If no explicit return is found and returnType is not void, you might need a default return:
    if (retType->isVoidTy()) {
        Builder.CreateRetVoid();
    } else {
        Builder.CreateRet(llvm::Constant::getNullValue(retType));
    }

    // Optionally verify the function:
    // llvm::verifyFunction(*function);

    // Once done with generating IR for the function body, pop the scope from pm if needed.
}

} // namespace extdef
