#include "token.hpp"

Token::Token(TokenType type, std::string lexeme, Literal literal, int line) 
    : type(type), lexeme(lexeme), literal(literal), line(line) {}