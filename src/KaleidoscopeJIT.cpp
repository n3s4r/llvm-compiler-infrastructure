//===----------------------------------------------------------------------===//
// KaleidoscopeJIT Implementation
// Based on LLVM Tutorial: https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/
//===----------------------------------------------------------------------===//

#include "KaleidoscopeJIT.h"
#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include "llvm/ExecutionEngine/Orc/ExecutionUtils.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"

namespace kaleidoscope {

KaleidoscopeJIT::KaleidoscopeJIT(
    std::unique_ptr<llvm::orc::ExecutionSession> ES,
    llvm::orc::JITTargetMachineBuilder JTMB, llvm::DataLayout DL)
    : ES(std::move(ES)), DL(std::move(DL)), Mangle(*this->ES, this->DL),
      ObjectLayer(*this->ES,
                  []() { return std::make_unique<llvm::SectionMemoryManager>(); }),
      CompileLayer(*this->ES, ObjectLayer,
                   std::make_unique<llvm::orc::ConcurrentIRCompiler>(std::move(JTMB))),
      OptimizeLayer(*this->ES, CompileLayer, optimizeModule),
      MainJD(this->ES->createBareJITDylib("<main>")) {
    MainJD.addGenerator(
        cantFail(llvm::orc::DynamicLibrarySearchGenerator::GetForCurrentProcess(
            DL.getGlobalPrefix())));
    if (JTMB.getTargetTriple().isOSBinFormatCOFF()) {
        ObjectLayer.setOverrideObjectFlagsWithResponsibilityFlags(true);
        ObjectLayer.setAutoClaimResponsibilityForObjectSymbols(true);
    }
}

KaleidoscopeJIT::~KaleidoscopeJIT() {
    if (auto Err = ES->endSession())
        ES->reportError(std::move(Err));
}

llvm::Expected<std::unique_ptr<KaleidoscopeJIT>> KaleidoscopeJIT::Create() {
    auto EPC = llvm::orc::SelfExecutorProcessControl::Create();
    if (!EPC)
        return EPC.takeError();

    auto ES = std::make_unique<llvm::orc::ExecutionSession>(std::move(*EPC));

    llvm::orc::JITTargetMachineBuilder JTMB(
        ES->getExecutorProcessControl().getTargetTriple());

    auto DL = JTMB.getDefaultDataLayoutForTarget();
    if (!DL)
        return DL.takeError();

    return std::make_unique<KaleidoscopeJIT>(std::move(ES), std::move(JTMB),
                                             std::move(*DL));
}

llvm::Error KaleidoscopeJIT::addModule(llvm::orc::ThreadSafeModule TSM,
                                       llvm::orc::ResourceTrackerSP RT) {
    if (!RT)
        RT = MainJD.getDefaultResourceTracker();
    return OptimizeLayer.add(RT, std::move(TSM));
}

llvm::Expected<llvm::orc::ExecutorSymbolDef>
KaleidoscopeJIT::lookup(llvm::StringRef Name) {
    return ES->lookup({&MainJD}, Mangle(Name.str()));
}

llvm::Expected<llvm::orc::ThreadSafeModule>
KaleidoscopeJIT::optimizeModule(llvm::orc::ThreadSafeModule TSM,
                                const llvm::orc::MaterializationResponsibility& R) {
    TSM.withModuleDo([](llvm::Module& M) {
        // Create a function pass manager.
        auto FPM = std::make_unique<llvm::legacy::FunctionPassManager>(&M);

        // Add some optimizations.
        FPM->add(llvm::createInstructionCombiningPass());
        FPM->add(llvm::createReassociatePass());
        FPM->add(llvm::createGVNPass());
        FPM->add(llvm::createCFGSimplificationPass());
        FPM->doInitialization();

        // Run the optimizations over all functions in the module being added to
        // the JIT.
        for (auto& F : M)
            FPM->run(F);
    });

    return std::move(TSM);
}

} // namespace kaleidoscope

