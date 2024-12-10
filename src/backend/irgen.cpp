#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

LLVMContext TheContext;
Module *TheModule = new Module("MyModule", TheContext);
IRBuilder<> Builder(TheContext);

void generateIR(ASTNode *node) {
    if (strcmp(node->nodeType, "Function") == 0) {
        // Create function
        FunctionType *FT = FunctionType::get(Builder.getInt32Ty(), false);
        Function *F = Function::Create(FT, Function::ExternalLinkage, node->text, TheModule);

        // Create entry basic block
        BasicBlock *BB = BasicBlock::Create(TheContext, "entry", F);
        Builder.SetInsertPoint(BB);

        // Recurse into function body
        for (int i = 0; i < node->numChildren; ++i) {
            generateIR(node->children->data[i]);
        }

        // Add return statement
        Builder.CreateRet(Builder.getInt32(0));
    } else if (strcmp(node->nodeType, "Return") == 0) {
        // Handle return statement
        int value = atoi(node->text);  // Simplification for demo
        Builder.CreateRet(Builder.getInt32(value));
    }
    // Add more cases for other node types...
}

int main() {
    // Assuming you already have an AST built
    ASTNode *root = buildAST();

    // Generate LLVM IR
    generateIR(root);

    // Print LLVM IR to stdout
    TheModule->print(llvm::outs(), nullptr);

    // Cleanup
    delete TheModule;
    freeAST(root);

    return 0;
}
