#include "lexer.hpp"

#include <cctype>
#include <string>
#include <unordered_map>
#include <vector>

#include "token.hpp"


namespace lexer
{
    Token generateTokenFromWord(const std::string& word, int lineNumber)
    {
        static const char* numbers = "0123456789";

        static const std::unordered_map<std::string, TokenType> operators = 
        {
            {"+",           TokenType::Plus},
            {"-",           TokenType::Minus},
            {"/",           TokenType::Divide},
            {"*",           TokenType::Multiply},
            {"=",           TokenType::Assign},
            {"==",          TokenType::Equals},
            {"!=",          TokenType::NotEqual},
            {">",           TokenType::GreaterThan},
            {">=",          TokenType::GreaterThanOrEqual},
            {"<",           TokenType::LessThan},
            {"<=",          TokenType::LessThanOrEqual}
        };

        static const std::unordered_map<std::string, TokenType> logic = 
        {
            {"OR",          TokenType::Or},
            {"AND",         TokenType::And},
            {"NOT",         TokenType::Not}
        };

        static const std::unordered_map<std::string, TokenType> punctuation
        {
            {"(",           TokenType::LeftBracket},
            {")",           TokenType::RightBracket},
        };

        static const std::unordered_map<std::string, TokenType> keywords = {
            {"VAR",         TokenType::Var},
            {"ON",          TokenType::On},
            {"ENDON",       TokenType::Endon},
            {"RAISE",       TokenType::Raise},
            {"IF",          TokenType::If},
            {"ELSEIF",      TokenType::Elseif},
            {"ELSE",        TokenType::Else},
            {"ENDIF",       TokenType::Endif},
            {"ROTATE",      TokenType::Rotate},
            {"TRANSLATE",   TokenType::Translate},
            {"BY",          TokenType::By},
            {"MOVE",        TokenType::Move},
            {"TO",          TokenType::To},
            {"THEN",        TokenType::Then}
        };

        // Generates a string token
        if (word[0] == '"')
        {
            return Token(TokenType::String, word, word.substr(1, word.length() - 2), lineNumber);
        }
        
        // Generates a number token
        if (std::strchr(numbers, word[0]))
        {
            return Token(TokenType::Number, word, std::stod(word), lineNumber);
        }

        // Generates an operator token
        if (operators.find(word) != operators.end())
        {
            return Token(operators.find(word)->second, word, std::monostate(), lineNumber);
        }

        // Generates a punctuation token
        if (punctuation.find(word) != punctuation.end())
        {
            return Token(punctuation.find(word)->second, word, std::monostate(), lineNumber);
        }
        
        // Generates a keyword token
        if (keywords.find(word) != keywords.end())
        {
            return Token(keywords.find(word)->second, word, std::monostate(), lineNumber);
        }

        // Generates an identifier token
        bool validIdentifier = true;
        for (int i = 0; i < word.length(); i++)
        {
            if (!std::isalnum(word[i]))
            {
                validIdentifier = false;
                break;
            }
        }

        if (validIdentifier)
        {
            return Token(TokenType::Identifier, word, word, lineNumber);
        }
    
        return Token(TokenType::Unidentified, word, std::monostate(), lineNumber);
    }

    std::vector<Token> generateTokensFromLine(std::string& line, int lineNumber)
    {
        static const char* whitespace = " \t\n\r\f\v";
        static const char* operators = "+-*/=!><";
        static const char* punctuation = "()";

        std::vector<Token> tokens;

        // Trim the line of any leading and trailing whitespace
        line.erase(line.find_last_not_of(whitespace) + 1);
        line.erase(0, line.find_first_not_of(whitespace));

        std::string word = "";
        char c;

        bool partOfStringLiteral = false;
        bool partOfOperator = false;

        for (int i = 0; i < line.length(); i++)
        {
            c = line[i];

            // Handle string literals
            if (c == '"' && !partOfStringLiteral)
            {
                if (word.length() > 0)
                {
                    tokens.push_back(generateTokenFromWord(word, lineNumber));
                }
                word = "";
                word += c;
            }
            else if (c == '"' && partOfStringLiteral)
            {
                word += c;
                tokens.push_back(generateTokenFromWord(word, lineNumber));
                word = "";
            }
            else if (c != '"' && partOfStringLiteral)
            {
                word += c;
            }
            else if (!partOfStringLiteral)
            {
                // Handle operators
                // Needed because possible to have something like x==-y where different operators are next to each other
                // without any spaces
                if (std::strchr(operators, c) && !partOfOperator)
                {
                    if (word.length() > 0)
                    {
                        tokens.push_back(generateTokenFromWord(word, lineNumber));
                    }
                    word = "";
                    word += c;
                }
                else if (std::strchr(operators, c) && partOfOperator)
                {
                    word += c;
                }
                else if (!std::strchr(operators, c)  && partOfOperator)
                {
                    // Split operator string into tokens
                    while (word.length() > 0)
                    {
                        // Try to find a two character operator first, such as ==
                        Token token = generateTokenFromWord(word.substr(0, 2), lineNumber);
                        
                        // If it is not an operator, the token type returned would be an identifier
                        if (token.type != TokenType::Unidentified)
                        {
                            tokens.push_back(token);
                            word.erase(0, 2);
                        }
                        else
                        {
                            token = generateTokenFromWord(std::string(1, word[0]), lineNumber);
                            tokens.push_back(token);
                            word.erase(0, 1);
                        }
                    }

                    partOfOperator = false;
                }
                
                if (!partOfOperator)
                {
                    // Handle punctuation
                    if (std::strchr(punctuation, c))
                    {
                        // Push back the word up to now
                        if (word.length() > 0)
                        {
                            tokens.push_back(generateTokenFromWord(word, lineNumber));
                        }

                        // Punctuation is its own word
                        // This should help solve issues such as (a), and split into 3 tokens instead of identifier
                        tokens.push_back(generateTokenFromWord(std::string(1, c), lineNumber));
                        word = "";
                    }
                    else if (std::strchr(whitespace, c))
                    {
                        if (word.length() > 0)
                        {
                            tokens.push_back(generateTokenFromWord(word, lineNumber));
                        }
                        
                        word = "";
                    }
                    else
                    {
                        word += c;
                    }
                }
            }
        }

        // Add the remaining word
        if (word.length() != 0)
        {
            tokens.push_back(generateTokenFromWord(word));
        }

        return tokens;
    }
}