//===----------------------------------------------------------------------===//
// Lexer Implementation - Kaleidoscope Lexer
// Based on LLVM Tutorial: https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/
//===----------------------------------------------------------------------===//

#include "Lexer.h"
#include <cctype>
#include <cstdio>
#include <cstdlib>

namespace kaleidoscope {

/// gettok - Return the next token from standard input.
int Lexer::gettok() {
    // Skip any whitespace.
    while (isspace(LastChar))
        LastChar = getchar();

    // Identifier: [a-zA-Z][a-zA-Z0-9]*
    if (isalpha(LastChar)) {
        IdentifierStr = LastChar;
        while (isalnum((LastChar = getchar())))
            IdentifierStr += LastChar;

        if (IdentifierStr == "def")
            return tok_def;
        if (IdentifierStr == "extern")
            return tok_extern;
        if (IdentifierStr == "if")
            return tok_if;
        if (IdentifierStr == "then")
            return tok_then;
        if (IdentifierStr == "else")
            return tok_else;
        if (IdentifierStr == "for")
            return tok_for;
        if (IdentifierStr == "in")
            return tok_in;
        if (IdentifierStr == "binary")
            return tok_binary;
        if (IdentifierStr == "unary")
            return tok_unary;
        if (IdentifierStr == "var")
            return tok_var;
        return tok_identifier;
    }

    // Number: [0-9.]+
    if (isdigit(LastChar) || LastChar == '.') {
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), nullptr);
        return tok_number;
    }

    // Comment until end of line.
    if (LastChar == '#') {
        do
            LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF)
            return gettok();
    }

    // Check for end of file. Don't eat the EOF.
    if (LastChar == EOF)
        return tok_eof;

    // Otherwise, just return the character as its ASCII value.
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}

} // namespace kaleidoscope

