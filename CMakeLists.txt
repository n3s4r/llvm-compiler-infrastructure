# Kaleidoscope - LLVM Tutorial Implementation
# Based on https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/index.html

cmake_minimum_required(VERSION 3.13.4)
project(Kaleidoscope)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find LLVM
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Include LLVM headers
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Add our source files
add_executable(kaleidoscope
    src/main.cpp
    src/Lexer.cpp
    src/Parser.cpp
    src/AST.cpp
    src/CodeGen.cpp
    src/KaleidoscopeJIT.cpp
)

# Link LLVM libraries
llvm_map_components_to_libnames(llvm_libs 
    Core 
    ExecutionEngine 
    InstCombine 
    MC
    MCJIT
    Object
    OrcJIT
    RuntimeDyld
    ScalarOpts 
    Support 
    TransformUtils
    native
    nativecodegen
)

target_link_libraries(kaleidoscope ${llvm_libs})

# Include directories
target_include_directories(kaleidoscope PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Set output directory
set_target_properties(kaleidoscope PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

