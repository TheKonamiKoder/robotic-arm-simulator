#include "parser.hpp"

#include <unordered_map>

#include "ast.hpp"
#include "error.hpp"
#include "token.hpp"

bool isUnaryContext(const Token& previous)
{
    return isOperator(previous.type) 
           || (previous.type == TokenType::LeftBracket)
           || (previous.type == TokenType::Unknown);
}

std::vector<Token> convertInfixToPostfix(std::vector<Token>& tokens)
{
    enum class Associativity {Left, Right};

    static const std::unordered_map<TokenType, Associativity> associativityMap = 
    {
#define X(OperatorName, AssociativityType, ...) {TokenType::OperatorName, Associativity::AssociativityType},
        OPERATOR_TOKEN_LIST
#undef X
    };

    static const std::unordered_map<TokenType, int> precedenceMap = 
    {
#define X(OperatorName, AssociativityType, Precedence, ...) {TokenType::OperatorName, Precedence},
        OPERATOR_TOKEN_LIST
#undef X
    };
    
    std::vector<Token> output;
    std::vector<Token> operatorStack;

    Token previous;
    previous.type = TokenType::Unknown;

    for (int i = 0; i < tokens.size(); i++) 
    {
        Token token = tokens[i];

        // Fix unary operators
        if (isUnaryContext(previous) && token.type == TokenType::Plus)
        {
            token.type = TokenType::UnaryPlus;
        }
        else if (isUnaryContext(previous) && token.type == TokenType::Minus)
        {
            token.type = TokenType::UnaryMinus;
        }

        // Case 1: operand
        if ((token.type == TokenType::Number) || (token.type == TokenType::Identifier)) 
        {
            output.push_back(token);
        }
        else if (token.type == TokenType::LeftBracket) 
        {
            operatorStack.push_back(token);
        }
        else if (token.type == TokenType::RightBracket) 
        {
            if (operatorStack.empty())
            {
                throw MismatchedBracketsError(token.sourceLocation);
            }
            
            Token operatorToken = operatorStack.back();
            operatorStack.pop_back();

            while (operatorToken.type != TokenType::LeftBracket) 
            {
                output.push_back(operatorToken);

                if (operatorStack.empty())
                {
                    throw MismatchedBracketsError(token.sourceLocation);
                }
                
                operatorToken = operatorStack.back();
                operatorStack.pop_back();
            }
        }
        else if (isOperator(token.type)) 
        {
            while (!operatorStack.empty() && isOperator(operatorStack.back().type)) {
                bool shouldPop =
                    (associativityMap.at(token.type) == Associativity::Left && 
                    precedenceMap.at(token.type) <= precedenceMap.at(operatorStack.back().type)) ||
                    (associativityMap.at(token.type) == Associativity::Right && 
                    precedenceMap.at(token.type) < precedenceMap.at(operatorStack.back().type));

                if (!shouldPop)
                {
                    break;
                }

                // Popping last element from operatorStack onto the output
                output.push_back(operatorStack.back());
                operatorStack.pop_back();
            }

            operatorStack.push_back(token);
        }

        previous = token;
    }

    // Pop remaining operators
    while (!operatorStack.empty()) {
        if ((operatorStack.back().type == TokenType::LeftBracket) || (operatorStack.back().type == TokenType::RightBracket))
        {
            throw MismatchedBracketsError(operatorStack.back().sourceLocation);
        }

        output.push_back(operatorStack.back());
        operatorStack.pop_back();
    }

    return output;
}