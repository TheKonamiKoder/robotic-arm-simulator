#include "token.hpp"

#include <ostream>

Token::Token(TokenType type, std::string lexeme, Value literal, SourceLocation sourceLocation) 
    : type(type), lexeme(lexeme), literal(literal), sourceLocation(sourceLocation) {}

std::string Token::toString() const
{
    switch (type)
    {
#define X(OperatorName, ...) case TokenType::OperatorName: return #OperatorName;
    OPERATOR_TOKEN_LIST
#undef X
#define X(TypeName) case TokenType::TypeName: return #TypeName;
    TOKEN_TYPE_LIST
#undef X
    }

    return "<Unknown TokenType>";
}

std::ostream& operator<<(std::ostream& os, TokenType type)
{
    switch (type)
    {
#define X(OperatorName, ...) case TokenType::OperatorName: return os << #OperatorName;
    OPERATOR_TOKEN_LIST
#undef X
#define X(TypeName) case TokenType::TypeName: return os << #TypeName;
    TOKEN_TYPE_LIST
#undef X
    }

    return os << "<Unknown TokenType>";
}

bool isOperator(TokenType type)
{
    return
#define X(OperatorName, ...) (type == TokenType::OperatorName) ||
    OPERATOR_TOKEN_LIST
#undef X
    0;
}
