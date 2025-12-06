//===----------------------------------------------------------------------===//
// Parser - Kaleidoscope Parser
// Based on LLVM Tutorial: https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/
//===----------------------------------------------------------------------===//

#ifndef KALEIDOSCOPE_PARSER_H
#define KALEIDOSCOPE_PARSER_H

#include "AST.h"
#include "Lexer.h"
#include <map>
#include <memory>

namespace kaleidoscope {

//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//

class Parser {
public:
    Parser();

    /// Get the next token from the lexer
    int getNextToken();

    /// Get the current token
    int getCurTok() const { return CurTok; }

    /// Get the lexer
    Lexer& getLexer() { return TheLexer; }

    //===------------------------------------------------------------------===//
    // Top-Level Parsing
    //===------------------------------------------------------------------===//

    /// definition ::= 'def' prototype expression
    std::unique_ptr<FunctionAST> ParseDefinition();

    /// toplevelexpr ::= expression
    std::unique_ptr<FunctionAST> ParseTopLevelExpr();

    /// external ::= 'extern' prototype
    std::unique_ptr<PrototypeAST> ParseExtern();

    //===------------------------------------------------------------------===//
    // Binary Operator Precedence
    //===------------------------------------------------------------------===//

    /// GetTokPrecedence - Get the precedence of the pending binary operator token.
    int GetTokPrecedence();

    /// Install standard binary operators with their precedences
    void InstallStandardOperators();

    /// BinopPrecedence - This holds the precedence for each binary operator that is defined.
    std::map<char, int> BinopPrecedence;

private:
    Lexer TheLexer;
    int CurTok = 0;

    //===------------------------------------------------------------------===//
    // Expression Parsing
    //===------------------------------------------------------------------===//

    /// numberexpr ::= number
    std::unique_ptr<ExprAST> ParseNumberExpr();

    /// parenexpr ::= '(' expression ')'
    std::unique_ptr<ExprAST> ParseParenExpr();

    /// identifierexpr
    ///   ::= identifier
    ///   ::= identifier '(' expression* ')'
    std::unique_ptr<ExprAST> ParseIdentifierExpr();

    /// ifexpr ::= 'if' expression 'then' expression 'else' expression
    std::unique_ptr<ExprAST> ParseIfExpr();

    /// forexpr ::= 'for' identifier '=' expr ',' expr (',' expr)? 'in' expression
    std::unique_ptr<ExprAST> ParseForExpr();

    /// varexpr ::= 'var' identifier ('=' expression)?
    //                    (',' identifier ('=' expression)?)* 'in' expression
    std::unique_ptr<ExprAST> ParseVarExpr();

    /// primary
    ///   ::= identifierexpr
    ///   ::= numberexpr
    ///   ::= parenexpr
    ///   ::= ifexpr
    ///   ::= forexpr
    ///   ::= varexpr
    std::unique_ptr<ExprAST> ParsePrimary();

    /// unary
    ///   ::= primary
    ///   ::= '!' unary
    std::unique_ptr<ExprAST> ParseUnary();

    /// binoprhs
    ///   ::= ('+' unary)*
    std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                           std::unique_ptr<ExprAST> LHS);

    /// expression
    ///   ::= unary binoprhs
    std::unique_ptr<ExprAST> ParseExpression();

    /// prototype
    ///   ::= id '(' id* ')'
    ///   ::= binary LETTER number? (id, id)
    ///   ::= unary LETTER (id)
    std::unique_ptr<PrototypeAST> ParsePrototype();
};

/// LogError* - These are little helper functions for error handling.
std::unique_ptr<ExprAST> LogError(const char* Str);
std::unique_ptr<PrototypeAST> LogErrorP(const char* Str);

} // namespace kaleidoscope

#endif // KALEIDOSCOPE_PARSER_H

