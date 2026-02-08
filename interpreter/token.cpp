#include "token.hpp"

Token::Token(TokenType type, std::string lexeme, Literal literal, int lineNumber) 
    : type(type), lexeme(lexeme), literal(literal), lineNumber(lineNumber) {}