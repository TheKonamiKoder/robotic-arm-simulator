#pragma once

#include <ostream>
#include <string>
#include <variant>

enum class TokenType
{
    // Literals
    Number,
    String,
    Boolean,
    Identifier,

    // Operators
    Plus,
    Minus,
    Divide,
    Multiply,
    Assign,                 // =
    Equals,                 // ==
    NotEqual,               // !=
    GreaterThan,            // >
    GreaterThanOrEqual,     // >=
    LessThan,               // <
    LessThanOrEqual,        // <=

    // Logic
    Or,
    And,
    Not,

    // Punctuation
    LeftBracket,
    RightBracket,

    // Keywords
    Var,
    On,
    Endon,
    Raise,
    If,
    Elseif,
    Else,
    Endif,
    Rotate,
    Translate,
    By,
    Move,
    To,
    Then,

    // Special
    Unidentified,
    EndOfFile
};


using Literal = std::variant<std::monostate, double, std::string, bool>;

class Token
{
    public:
        TokenType type;
        std::string lexeme;     // Will store raw text
        Literal literal;        // Will store the value
        int lineNumber;         // Will store line number (for raising errors)

        Token() = default;
        Token(TokenType type, std::string lexeme, Literal literal, int lineNumber);
};

// This is for pretty printing the tokens, which is useful in debugging
std::ostream& operator<<(std::ostream& os, TokenType type);