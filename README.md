# Kaleidoscope - LLVM Tutorial Implementation

A complete implementation of the **Kaleidoscope** language from the [LLVM Tutorial: My First Language Frontend](https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/index.html).

## Overview

Kaleidoscope is a simple procedural language that demonstrates LLVM's capabilities:

- **Variables**: 64-bit floating-point only (all values are doubles)
- **Functions**: User-defined functions with recursion support
- **Operators**: Arithmetic (`+`, `-`, `*`, `/`), comparison (`<`, `>`), and user-defined operators
- **Control Flow**: `if/then/else` expressions and `for` loops
- **JIT Compilation**: Expressions are compiled and executed on the fly
- **Optimization**: LLVM optimization passes are applied automatically

## Features Implemented

| Chapter | Feature | Status |
|---------|---------|--------|
| 1 | Lexer | ✅ |
| 2 | Parser & AST | ✅ |
| 3 | LLVM IR Code Generation | ✅ |
| 4 | JIT Compilation & Optimization | ✅ |
| 5 | Control Flow (if/then/else, for) | ✅ |
| 6 | User-defined Operators | ✅ |
| 7 | Mutable Variables | ✅ |

## Prerequisites

- **LLVM** (version 14 or later recommended)
- **CMake** (version 3.13.4 or later)
- **C++ Compiler** with C++17 support (GCC, Clang, or MSVC)

### Installing LLVM

**Windows (with vcpkg):**
```powershell
vcpkg install llvm:x64-windows
```

**Windows (with Chocolatey):**
```powershell
choco install llvm
```

**Ubuntu/Debian:**
```bash
sudo apt-get install llvm-dev
```

**macOS:**
```bash
brew install llvm
```

## Building

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build
cmake --build .
```

If LLVM is not in a standard location, specify the path:
```bash
cmake -DLLVM_DIR=/path/to/llvm/lib/cmake/llvm ..
```

## Running

```bash
./bin/kaleidoscope
```

This starts an interactive REPL (Read-Eval-Print Loop).

## Language Syntax

### Basic Expressions

```
ready> 2 + 3
Evaluated to 5.000000

ready> 4 * (2 + 3)
Evaluated to 20.000000
```

### Function Definitions

```
ready> def add(x y) x + y
Read function definition

ready> add(3, 4)
Evaluated to 7.000000
```

### External Functions

```
ready> extern sin(x)
ready> sin(1.0)
Evaluated to 0.841471
```

### Fibonacci Example

```
ready> def fib(x)
         if x < 3 then
           1
         else
           fib(x-1) + fib(x-2)

ready> fib(10)
Evaluated to 55.000000
```

### Control Flow

**If/Then/Else:**
```
ready> def max(a b) if a > b then a else b
ready> max(10, 20)
Evaluated to 20.000000
```

**For Loops:**
```
ready> def printstar(n)
         for i = 1, i < n, 1.0 in
           putchard(42)  # 42 is ASCII for '*'

ready> printstar(10)
**********Evaluated to 0.000000
```

### User-Defined Operators

```
ready> def unary!(v)
         if v then
           0
         else
           1

ready> !0
Evaluated to 1.000000

ready> !1
Evaluated to 0.000000
```

Binary operators with precedence:
```
ready> def binary| 5 (LHS RHS)
         if LHS then
           1
         else if RHS then
           1
         else
           0

ready> 0|1
Evaluated to 1.000000
```

### Mutable Variables

```
ready> def fib(x)
         var a = 1, b = 1, c in
         (for i = 3, i < x in
            c = a + b :
            a = b :
            b = c) :
         b

ready> fib(10)
Evaluated to 55.000000
```

## Built-in Functions

The following C library functions are available:

- `putchard(x)` - Prints a character (ASCII value) and returns 0
- `printd(x)` - Prints a double value and returns 0
- `sin(x)`, `cos(x)`, etc. - Standard math functions (via extern)

## Project Structure

```
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── include/
│   ├── AST.h               # Abstract Syntax Tree definitions
│   ├── CodeGen.h           # LLVM IR code generation
│   ├── KaleidoscopeJIT.h   # JIT compiler
│   ├── Lexer.h             # Tokenizer
│   └── Parser.h            # Recursive descent parser
├── src/
│   ├── AST.cpp             # AST code generation implementation
│   ├── CodeGen.cpp         # Code generation utilities
│   ├── KaleidoscopeJIT.cpp # JIT implementation
│   ├── Lexer.cpp           # Lexer implementation
│   ├── Parser.cpp          # Parser implementation
│   └── main.cpp            # Entry point and REPL
└── examples/
    ├── fibonacci.ks        # Fibonacci sequence
    ├── mandelbrot.ks       # Mandelbrot set visualization
    └── operators.ks        # User-defined operators demo
```

## Grammar

```ebnf
program        ::= (definition | external | expression)*

definition     ::= 'def' prototype expression
external       ::= 'extern' prototype

prototype      ::= identifier '(' identifier* ')'
                | 'binary' LETTER number? '(' identifier identifier ')'
                | 'unary' LETTER '(' identifier ')'

expression     ::= unary binoprhs
binoprhs       ::= ('+' unary)*
unary          ::= primary | '!' unary

primary        ::= identifierexpr | numberexpr | parenexpr 
                | ifexpr | forexpr | varexpr

identifierexpr ::= identifier | identifier '(' expression* ')'
numberexpr     ::= number
parenexpr      ::= '(' expression ')'
ifexpr         ::= 'if' expression 'then' expression 'else' expression
forexpr        ::= 'for' identifier '=' expr ',' expr (',' expr)? 'in' expression
varexpr        ::= 'var' identifier ('=' expression)? 
                   (',' identifier ('=' expression)?)* 'in' expression
```

## License

This implementation is based on the LLVM Tutorial and is provided for educational purposes.

## References

- [LLVM Tutorial: My First Language Frontend](https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/index.html)
- [LLVM Documentation](https://llvm.org/docs/)
- [LLVM GitHub Repository](https://github.com/llvm/llvm-project)
