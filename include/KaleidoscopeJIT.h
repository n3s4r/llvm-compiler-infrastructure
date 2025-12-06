//===----------------------------------------------------------------------===//
// KaleidoscopeJIT - JIT Compiler for Kaleidoscope
// Based on LLVM Tutorial: https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/
//===----------------------------------------------------------------------===//

#ifndef KALEIDOSCOPE_JIT_H
#define KALEIDOSCOPE_JIT_H

#include "llvm/ADT/StringRef.h"
#include "llvm/ExecutionEngine/JITSymbol.h"
#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/ExecutionUtils.h"
#include "llvm/ExecutionEngine/Orc/ExecutorProcessControl.h"
#include "llvm/ExecutionEngine/Orc/IRCompileLayer.h"
#include "llvm/ExecutionEngine/Orc/IRTransformLayer.h"
#include "llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h"
#include "llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h"
#include "llvm/ExecutionEngine/Orc/Shared/ExecutorSymbolDef.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include <memory>

namespace kaleidoscope {

class KaleidoscopeJIT {
private:
    std::unique_ptr<llvm::orc::ExecutionSession> ES;

    llvm::DataLayout DL;
    llvm::orc::MangleAndInterner Mangle;

    llvm::orc::RTDyldObjectLinkingLayer ObjectLayer;
    llvm::orc::IRCompileLayer CompileLayer;
    llvm::orc::IRTransformLayer OptimizeLayer;

    llvm::orc::JITDylib &MainJD;

public:
    KaleidoscopeJIT(std::unique_ptr<llvm::orc::ExecutionSession> ES,
                    llvm::orc::JITTargetMachineBuilder JTMB,
                    llvm::DataLayout DL);

    ~KaleidoscopeJIT();

    static llvm::Expected<std::unique_ptr<KaleidoscopeJIT>> Create();

    const llvm::DataLayout &getDataLayout() const { return DL; }

    llvm::orc::JITDylib &getMainJITDylib() { return MainJD; }

    llvm::Error addModule(llvm::orc::ThreadSafeModule TSM,
                          llvm::orc::ResourceTrackerSP RT = nullptr);

    llvm::Expected<llvm::orc::ExecutorSymbolDef> lookup(llvm::StringRef Name);

private:
    static llvm::Expected<llvm::orc::ThreadSafeModule>
    optimizeModule(llvm::orc::ThreadSafeModule TSM,
                   const llvm::orc::MaterializationResponsibility &R);
};

} // namespace kaleidoscope

#endif // KALEIDOSCOPE_JIT_H

