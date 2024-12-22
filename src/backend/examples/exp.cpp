#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace exp {
    Value *binary_op(IRBuilder<> &Builder, ASTNode *node) {
        Value *L = generateIR(Builder, node->children->data[0]);
        Value *R = generateIR(Builder, node->children->data[1]);

        Value *Result;
        if (strcmp(node->text, "+") == 0) {
            Result = Builder.CreateAdd(L, R, "addtmp");
        } else if (strcmp(node->text, "-") == 0) {
            Result = Builder.CreateSub(L, R, "subtmp");
        } else if (strcmp(node->text, "*") == 0) {
            Result = Builder.CreateMul(L, R, "multmp");
        } else if (strcmp(node->text, "/") == 0) {
            Result = Builder.CreateSDiv(L, R, "divtmp");
        } else {
            // Handle other binary operations or throw an error
            Result = nullptr;
        }

        return Result;
    }
}