//===----------------------------------------------------------------------===//
// Kaleidoscope Main - Entry point and REPL
// Based on LLVM Tutorial: https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/
//
// This file implements the main entry point for the Kaleidoscope interpreter,
// which provides a REPL (Read-Eval-Print Loop) for the language.
//===----------------------------------------------------------------------===//

#include "AST.h"
#include "CodeGen.h"
#include "KaleidoscopeJIT.h"
#include "Lexer.h"
#include "Parser.h"

#include "llvm/Support/TargetSelect.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"

#include <iostream>

using namespace kaleidoscope;

//===----------------------------------------------------------------------===//
// External Library Functions (for use in Kaleidoscope)
//===----------------------------------------------------------------------===//

#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

/// putchard - putchar that takes a double and returns 0.
extern "C" DLLEXPORT double putchard(double X) {
    fputc((char)X, stderr);
    return 0;
}

/// printd - printf that takes a double prints it as "%f\n", returning 0.
extern "C" DLLEXPORT double printd(double X) {
    fprintf(stderr, "%f\n", X);
    return 0;
}

//===----------------------------------------------------------------------===//
// Top-Level Handlers
//===----------------------------------------------------------------------===//

static Parser TheParser;

static void HandleDefinition() {
    if (auto FnAST = TheParser.ParseDefinition()) {
        if (auto* FnIR = FnAST->codegen()) {
            fprintf(stderr, "Read function definition:\n");
            FnIR->print(llvm::errs());
            fprintf(stderr, "\n");
            auto RT = TheJIT->getMainJITDylib().createResourceTracker();
            auto TSM = llvm::orc::ThreadSafeModule(std::move(TheModule),
                                                   std::move(TheContext));
            if (auto Err = TheJIT->addModule(std::move(TSM), RT)) {
                fprintf(stderr, "Error adding module to JIT\n");
            }
            InitializeModuleAndManagers();
        }
    } else {
        // Skip token for error recovery.
        TheParser.getNextToken();
    }
}

static void HandleExtern() {
    if (auto ProtoAST = TheParser.ParseExtern()) {
        if (auto* FnIR = ProtoAST->codegen()) {
            fprintf(stderr, "Read extern:\n");
            FnIR->print(llvm::errs());
            fprintf(stderr, "\n");
            FunctionProtos[ProtoAST->getName()] = std::move(ProtoAST);
        }
    } else {
        // Skip token for error recovery.
        TheParser.getNextToken();
    }
}

static void HandleTopLevelExpression() {
    // Evaluate a top-level expression into an anonymous function.
    if (auto FnAST = TheParser.ParseTopLevelExpr()) {
        if (auto* FnIR = FnAST->codegen()) {
            fprintf(stderr, "Read top-level expression:\n");
            FnIR->print(llvm::errs());
            fprintf(stderr, "\n");

            // Create a ResourceTracker to track JIT'd memory allocated to our
            // anonymous expression -- that way we can free it after executing.
            auto RT = TheJIT->getMainJITDylib().createResourceTracker();

            auto TSM = llvm::orc::ThreadSafeModule(std::move(TheModule),
                                                   std::move(TheContext));
            if (auto Err = TheJIT->addModule(std::move(TSM), RT)) {
                fprintf(stderr, "Error adding module to JIT\n");
                return;
            }
            InitializeModuleAndManagers();

            // Search the JIT for the __anon_expr symbol.
            auto ExprSymbol = TheJIT->lookup("__anon_expr");
            if (!ExprSymbol) {
                fprintf(stderr, "Error: could not find __anon_expr in JIT\n");
                return;
            }

            // Get the symbol's address and cast it to the right type (takes no
            // arguments, returns a double) so we can call it as a native function.
            double (*FP)() = ExprSymbol->getAddress().toPtr<double (*)()>();
            fprintf(stderr, "Evaluated to %f\n", FP());

            // Delete the anonymous expression module from the JIT.
            if (auto Err = RT->remove()) {
                fprintf(stderr, "Error removing anonymous expression from JIT\n");
            }
        }
    } else {
        // Skip token for error recovery.
        TheParser.getNextToken();
    }
}

/// top ::= definition | external | expression | ';'
static void MainLoop() {
    while (true) {
        fprintf(stderr, "ready> ");
        switch (TheParser.getCurTok()) {
        case tok_eof:
            return;
        case ';': // ignore top-level semicolons.
            TheParser.getNextToken();
            break;
        case tok_def:
            HandleDefinition();
            break;
        case tok_extern:
            HandleExtern();
            break;
        default:
            HandleTopLevelExpression();
            break;
        }
    }
}

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//

int main() {
    // Initialize native target for JIT compilation
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    fprintf(stderr, "Kaleidoscope - LLVM Tutorial Language\n");
    fprintf(stderr, "=====================================\n");
    fprintf(stderr, "Based on: https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/\n\n");
    fprintf(stderr, "Commands:\n");
    fprintf(stderr, "  def <name>(<args>) <expr>  - Define a function\n");
    fprintf(stderr, "  extern <name>(<args>)      - Declare an external function\n");
    fprintf(stderr, "  <expr>                     - Evaluate an expression\n");
    fprintf(stderr, "  Ctrl+Z (Windows) or Ctrl+D (Unix) to exit\n\n");

    // Create the JIT
    auto JITExpected = KaleidoscopeJIT::Create();
    if (!JITExpected) {
        fprintf(stderr, "Error creating JIT: %s\n",
                toString(JITExpected.takeError()).c_str());
        return 1;
    }
    TheJIT = std::move(*JITExpected);

    // Initialize the module and pass managers
    InitializeModuleAndManagers();

    // Prime the first token.
    fprintf(stderr, "ready> ");
    TheParser.getNextToken();

    // Run the main "interpreter loop" now.
    MainLoop();

    return 0;
}

