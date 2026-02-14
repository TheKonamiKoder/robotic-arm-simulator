#include "lexer.hpp"

#include <cctype>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "token.hpp"

Token generateTokenFromWord(const std::string& word, SourceLocation sourceLocation)
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
        {"NOT",         TokenType::Not},
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
        bool validString = word[word.length() - 1] == '"';
        if (validString)
        {
            return Token(TokenType::String, word, word.substr(1, word.length() - 2), sourceLocation);
        }
        else
        {
            return Token(TokenType::Unknown, word, Value(), sourceLocation);
        }
        
    }
    
    // Generates a number token
    if (std::strchr(numbers, word[0]))
    {
        bool validNumber = true;
        int dotCount = 0;
        for (int i = 0; i < word.length(); i++)
        {
            // Ensures there is only one decimal place in the number   
            if (word[i] == '.')
            {
                dotCount += 1;
                if (dotCount > 1)
                {
                    validNumber = false;
                }
            }
            else if (!strchr(numbers, word[i]))
            {
                validNumber = false;
                break;
            }
        }
        
        if (validNumber)
        {
            return Token(TokenType::Number, word, std::stod(word), sourceLocation);
        }
        else
        {
            return Token(TokenType::Unknown, word, Value(), sourceLocation);
        }
    }

    // Generates an operator token
    if (operators.find(word) != operators.end())
    {
        return Token(operators.find(word)->second, word, Value(), sourceLocation);
    }

    // Generates a punctuation token
    if (punctuation.find(word) != punctuation.end())
    {
        return Token(punctuation.find(word)->second, word, Value(), sourceLocation);
    }

    // Generates a logic token
    if (logic.find(word) != logic.end())
    {
        return Token(logic.find(word)->second, word, Value(), sourceLocation);
    }

    // Generates True/False token
    if (word == "TRUE")
    {
        return Token(TokenType::Boolean, word, true, sourceLocation);
    }
    else if (word == "FALSE")
    {
        return Token(TokenType::Boolean, word, false, sourceLocation);
    }
    
    // Generates a keyword token
    if (keywords.find(word) != keywords.end())
    {
        return Token(keywords.find(word)->second, word, Value(), sourceLocation);
    }

    // Generates an identifier token
    bool validIdentifier = true;
    for (int i = 0; i < word.length(); i++)
    {
        if (!(std::isalnum(word[i]) || word[i] == '_'))
        {
            validIdentifier = false;
            break;
        }
    }

    if (validIdentifier)
    {
        return Token(TokenType::Identifier, word, word, sourceLocation);
    }

    return Token(TokenType::Unknown, word, Value(), sourceLocation);
}

std::vector<Token> generateTokensFromLine(const std::string& line, int lineNumber)
{
    static const char* whitespace = " \t\n\r\f\v";
    static const char* operators = "+-*/=!><";
    static const char* punctuation = "()";

    std::vector<Token> tokens;

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
                tokens.push_back(generateTokenFromWord(word, {lineNumber, i}));
            }
            word = "";
            word += c;

            partOfStringLiteral = true;
        }
        else if (c == '"' && partOfStringLiteral)
        {
            word += c;
            tokens.push_back(generateTokenFromWord(word, {lineNumber, i}));
            word = "";

            partOfStringLiteral = false;
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
                    tokens.push_back(generateTokenFromWord(word, {lineNumber, i}));
                }
                word = "";
                word += c;

                partOfOperator = true;
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
                    Token token = generateTokenFromWord(word.substr(0, 2), {lineNumber, i});
                    
                    // If it is not an operator, the token type returned would be an identifier
                    if (token.type != TokenType::Unknown)
                    {
                        tokens.push_back(token);
                        word.erase(0, 2);
                    }
                    else
                    {
                        token = generateTokenFromWord(std::string(1, word[0]), {lineNumber, i});
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
                        tokens.push_back(generateTokenFromWord(word, {lineNumber, i}));
                    }

                    // Punctuation is its own word
                    // This should help solve issues such as (a), and split into 3 tokens instead of identifier
                    tokens.push_back(generateTokenFromWord(std::string(1, c), {lineNumber, i}));
                    word = "";
                }
                else if (std::strchr(whitespace, c))
                {
                    if (word.length() > 0)
                    {
                        tokens.push_back(generateTokenFromWord(word, {lineNumber, i}));
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

std::vector<Token> generateTokensFromFile(const std::string& filename)
{
    std::vector<Token> tokens;

    std::ifstream file(filename);
    std::string line;
    
    while (std::getline(file, line))
    {
        std::vector<Token> lineTokens = generateTokensFromLine(line);

        for (int i = 0; i < lineTokens.size(); i++)
        {
            tokens.push_back(lineTokens[i]);
        }
    }

    return tokens; 
}