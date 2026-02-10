#include "token.hpp"

#include <ostream>

Token::Token(TokenType type, std::string lexeme, Literal literal, int lineNumber) 
    : type(type), lexeme(lexeme), literal(literal), lineNumber(lineNumber) {}

std::ostream& operator<<(std::ostream& os, TokenType type)
{
    switch (type)
    {
        // Literals
        case TokenType::Number:              return os << "Number";
        case TokenType::String:              return os << "String";
        case TokenType::Boolean:             return os << "Boolean";
        case TokenType::Identifier:          return os << "Identifier";

        // Operators
        case TokenType::Plus:                return os << "Plus";
        case TokenType::Minus:               return os << "Minus";
        case TokenType::Divide:              return os << "Divide";
        case TokenType::Multiply:            return os << "Multiply";
        case TokenType::Assign:              return os << "Assign";
        case TokenType::Equals:              return os << "Equals";
        case TokenType::NotEqual:            return os << "NotEqual";
        case TokenType::GreaterThan:         return os << "GreaterThan";
        case TokenType::GreaterThanOrEqual:  return os << "GreaterThanOrEqual";
        case TokenType::LessThan:            return os << "LessThan";
        case TokenType::LessThanOrEqual:     return os << "LessThanOrEqual";

        // Logic
        case TokenType::Or:                  return os << "Or";
        case TokenType::And:                 return os << "And";
        case TokenType::Not:                 return os << "Not";

        // Punctuation
        case TokenType::LeftBracket:         return os << "LeftBracket";
        case TokenType::RightBracket:        return os << "RightBracket";

        // Keywords
        case TokenType::Var:                 return os << "Var";
        case TokenType::On:                  return os << "On";
        case TokenType::Endon:               return os << "Endon";
        case TokenType::Raise:               return os << "Raise";
        case TokenType::If:                  return os << "If";
        case TokenType::Elseif:              return os << "Elseif";
        case TokenType::Else:                return os << "Else";
        case TokenType::Endif:               return os << "Endif";
        case TokenType::Rotate:              return os << "Rotate";
        case TokenType::Translate:           return os << "Translate";
        case TokenType::By:                  return os << "By";
        case TokenType::Move:                return os << "Move";
        case TokenType::To:                  return os << "To";
        case TokenType::Then:                return os << "Then";

        // Special
        case TokenType::Unidentified:        return os << "Unidentified";
        case TokenType::EndOfFile:           return os << "EndOfFile";
    }

    return os << "<Unknown TokenType>";
}
