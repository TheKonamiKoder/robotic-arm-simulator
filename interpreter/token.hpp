#pragma once

#include <ostream>
#include <string>
#include <variant>

#include "source_location.hpp"
#include "value.hpp"

// Operators are defined with their name, their associativity, their precedence and their arity in that order
#define OPERATOR_TOKEN_LIST \
    X(Plus,                Left,  50, 2) \
    X(Minus,               Left,  50, 2) \
    X(Multiply,            Left,  60, 2) \
    X(Divide,              Left,  60, 2) \
    X(UnaryPlus,           Right, 70, 1) \
    X(UnaryMinus,          Right, 70, 1) \
    X(Not,                 Right, 70, 1) \
    X(GreaterThan,         Left,  40, 2) \
    X(GreaterThanOrEqual,  Left,  40, 2) \
    X(LessThan,            Left,  40, 2) \
    X(LessThanOrEqual,     Left,  40, 2) \
    X(Equals,              Left,  30, 2) \
    X(NotEqual,            Left,  30, 2) \
    X(And,                 Left,  20, 2) \
    X(Or,                  Left,  10, 2)

#define TOKEN_TYPE_LIST \
    X(Number) \
    X(String) \
    X(Boolean) \
    X(Identifier) \
    X(Assign) \
    X(LeftBracket) \
    X(RightBracket) \
    X(Var) \
    X(On) \
    X(Endon) \
    X(Raise) \
    X(If) \
    X(Elseif) \
    X(Else) \
    X(Endif) \
    X(Rotate) \
    X(Translate) \
    X(By) \
    X(Move) \
    X(To) \
    X(Then) \
    X(Log) \
    X(Unknown)

enum class TokenType
{
    // For operators, we can discard the last values
#define X(OperatorName, ...) OperatorName,
    OPERATOR_TOKEN_LIST
#undef X

#define X(TypeName) TypeName,
    TOKEN_TYPE_LIST
#undef X


};

class Token
{
    public:
        TokenType type;
        std::string lexeme;     // Will store raw text
        Value literal;          // Will store the value
        SourceLocation sourceLocation;

        Token() = default;
        Token(TokenType type, std::string lexeme, Value literal, SourceLocation sourceLocation = {});

        std::string toString() const;
};

// This is for pretty printing the tokens, which is useful in debugging
std::ostream& operator<<(std::ostream& os, TokenType type);

// Will return true if the type is one of the operators, or otherwise it will return false
bool isOperator(TokenType type);