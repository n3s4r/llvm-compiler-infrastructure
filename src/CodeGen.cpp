//===----------------------------------------------------------------------===//
// Code Generation - LLVM IR Code Generation for Kaleidoscope
// Based on LLVM Tutorial: https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/
//===----------------------------------------------------------------------===//

#include "CodeGen.h"
#include "KaleidoscopeJIT.h"
#include "Parser.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include <iostream>

namespace kaleidoscope {

//===----------------------------------------------------------------------===//
// Global Variables for Code Generation
//===----------------------------------------------------------------------===//

std::unique_ptr<llvm::LLVMContext> TheContext;
std::unique_ptr<llvm::Module> TheModule;
std::unique_ptr<llvm::IRBuilder<>> Builder;
std::map<std::string, llvm::AllocaInst*> NamedValues;
std::unique_ptr<KaleidoscopeJIT> TheJIT;
std::map<std::string, std::unique_ptr<PrototypeAST>> FunctionProtos;

//===----------------------------------------------------------------------===//
// Code Generation Helper Functions
//===----------------------------------------------------------------------===//

llvm::Value* LogErrorV(const char* Str) {
    LogError(Str);
    return nullptr;
}

llvm::Function* getFunction(const std::string& Name) {
    // First, see if the function has already been added to the current module.
    if (auto* F = TheModule->getFunction(Name))
        return F;

    // If not, check whether we can codegen the declaration from some existing
    // prototype.
    auto FI = FunctionProtos.find(Name);
    if (FI != FunctionProtos.end())
        return FI->second->codegen();

    // If no existing prototype exists, return null.
    return nullptr;
}

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
/// the function. This is used for mutable variables etc.
llvm::AllocaInst* CreateEntryBlockAlloca(llvm::Function* TheFunction,
                                         const std::string& VarName) {
    llvm::IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
                           TheFunction->getEntryBlock().begin());
    return TmpB.CreateAlloca(llvm::Type::getDoubleTy(*TheContext), nullptr,
                             VarName);
}

void InitializeModuleAndManagers() {
    // Open a new context and module.
    TheContext = std::make_unique<llvm::LLVMContext>();
    TheModule = std::make_unique<llvm::Module>("KaleidoscopeJIT", *TheContext);
    
    if (TheJIT)
        TheModule->setDataLayout(TheJIT->getDataLayout());

    // Create a new builder for the module.
    Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
}

} // namespace kaleidoscope

