//===----------------------------------------------------------------------===//
// Lexer - Kaleidoscope Lexer
// Based on LLVM Tutorial: https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/
//===----------------------------------------------------------------------===//

#ifndef KALEIDOSCOPE_LEXER_H
#define KALEIDOSCOPE_LEXER_H

#include <string>

namespace kaleidoscope {

//===----------------------------------------------------------------------===//
// Token Types
//===----------------------------------------------------------------------===//

enum Token {
    tok_eof = -1,

    // Commands
    tok_def = -2,
    tok_extern = -3,

    // Primary
    tok_identifier = -4,
    tok_number = -5,

    // Control flow
    tok_if = -6,
    tok_then = -7,
    tok_else = -8,
    tok_for = -9,
    tok_in = -10,

    // Operators
    tok_binary = -11,
    tok_unary = -12,

    // Var definition
    tok_var = -13,
};

//===----------------------------------------------------------------------===//
// Lexer Class
//===----------------------------------------------------------------------===//

class Lexer {
public:
    Lexer() = default;

    /// gettok - Return the next token from standard input.
    int gettok();

    /// Get the current identifier string (for tok_identifier)
    const std::string& getIdentifierStr() const { return IdentifierStr; }

    /// Get the current number value (for tok_number)
    double getNumVal() const { return NumVal; }

    /// Get the last character read
    int getLastChar() const { return LastChar; }

private:
    std::string IdentifierStr; // Filled in if tok_identifier
    double NumVal = 0.0;       // Filled in if tok_number
    int LastChar = ' ';        // Last character read
};

} // namespace kaleidoscope

#endif // KALEIDOSCOPE_LEXER_H

