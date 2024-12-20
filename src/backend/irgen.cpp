#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"

#include "exp.cpp"

using namespace llvm;

LLVMContext TheContext;
Module *TheModule = new Module("MyModule", TheContext);
IRBuilder<> Builder(TheContext);

void generateIR(ASTNode *node) {
    if (strcmp(node->nodeType, "ExtDef") == 0) {
        return extdef::handle_extdef(Builder, node)
    }
    else if (strcmp(node->nodeType, "BinaryOp") == 0) {
        return exp::binary_op(Builder, node);
    }
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
