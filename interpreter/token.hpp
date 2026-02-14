#pragma once

#include <ostream>
#include <string>
#include <variant>

#include "source_location.hpp"
#include "value.hpp"

#define TOKEN_TYPE_LIST \
    X(Number) \
    X(String) \
    X(Boolean) \
    X(Identifier) \
    X(Plus) \
    X(Minus) \
    X(Multiply) \
    X(Divide) \
    X(Assign) \
    X(Equals) \
    X(NotEqual) \
    X(GreaterThan) \
    X(GreaterThanOrEqual) \
    X(LessThan) \
    X(LessThanOrEqual) \
    X(Or) \
    X(And) \
    X(Not) \
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
    X(Unknown)

enum class TokenType
{
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
};

// This is for pretty printing the tokens, which is useful in debugging
std::ostream& operator<<(std::ostream& os, TokenType type);