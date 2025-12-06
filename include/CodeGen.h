//===----------------------------------------------------------------------===//
// CodeGen - LLVM IR Code Generation for Kaleidoscope
// Based on LLVM Tutorial: https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/
//===----------------------------------------------------------------------===//

#ifndef KALEIDOSCOPE_CODEGEN_H
#define KALEIDOSCOPE_CODEGEN_H

#include "AST.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LegacyPassManager.h"
#include <map>
#include <memory>

namespace kaleidoscope {

// Forward declaration
class KaleidoscopeJIT;

//===----------------------------------------------------------------------===//
// Code Generation Globals
//===----------------------------------------------------------------------===//

/// TheContext - The LLVM context. Owns core LLVM data structures.
extern std::unique_ptr<llvm::LLVMContext> TheContext;

/// TheModule - Module contains all the functions and global variables.
extern std::unique_ptr<llvm::Module> TheModule;

/// Builder - Helper object for generating LLVM instructions.
extern std::unique_ptr<llvm::IRBuilder<>> Builder;

/// NamedValues - Symbol table for function scope variables.
extern std::map<std::string, llvm::AllocaInst*> NamedValues;

/// TheJIT - JIT compiler instance.
extern std::unique_ptr<KaleidoscopeJIT> TheJIT;

/// FunctionProtos - Keep a mapping of each function prototype.
extern std::map<std::string, std::unique_ptr<PrototypeAST>> FunctionProtos;

//===----------------------------------------------------------------------===//
// Code Generation Functions
//===----------------------------------------------------------------------===//

/// InitializeModule - Initialize module and pass managers.
void InitializeModuleAndManagers();

/// GetFunction - Get a function from the current module, potentially
/// generating a declaration from a prototype.
llvm::Function* getFunction(const std::string& Name);

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block
/// of the function. Used for mutable variables.
llvm::AllocaInst* CreateEntryBlockAlloca(llvm::Function* TheFunction,
                                         const std::string& VarName);

/// LogErrorV - Error logging for Value* returns
llvm::Value* LogErrorV(const char* Str);

} // namespace kaleidoscope

#endif // KALEIDOSCOPE_CODEGEN_H

