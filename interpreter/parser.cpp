#include "parser.hpp"

#include <memory>
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
        if 
        (
            (token.type == TokenType::Identifier) ||
            (token.type == TokenType::Number)     ||
            (token.type == TokenType::String)     ||
            (token.type == TokenType::Boolean) 
        ) 
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

std::unique_ptr<Expression> generateExpressionFromPostfixTokens(std::vector<Token>& tokens)
{
    static const std::unordered_map<TokenType, int> arityMap =
    {
#define X(OperatorName, AssociativityType, Precedence, Arity) {TokenType::OperatorName, Arity},
        OPERATOR_TOKEN_LIST
#undef X
    };

    auto throwIfTokensEmpty = [](const std::vector<Token>& tokens, const SourceLocation& sourceLocation)
    {
        if (tokens.empty())
        {
            throw MalformedExpressionError(sourceLocation);
        }
    };

    // If the code is empty, return an empty expression
    if (tokens.empty())
    {
        return nullptr;
    }

    // Pop the last token from the vectors
    Token token = tokens.back();
    tokens.pop_back();

    if (!isOperator(token.type))
    {
        switch (token.type)
        {
        case TokenType::Identifier:     // Variables
        {
            auto expression = std::make_unique<VariableExpression>();
            expression->name = token.literal.asString();
            expression->sourceLocation = token.sourceLocation;
            return expression;
        }
        case TokenType::Number:         // Number literals
        {
            auto expression = std::make_unique<NumberLiteralExpression>();
            expression->value = token.literal.asNumber();
            expression->sourceLocation = token.sourceLocation;
            return expression;
        }
        case TokenType::String:
        {
            auto expression = std::make_unique<StringLiteralExpression>();
            expression->value = token.literal.asString();
            expression->sourceLocation = token.sourceLocation;
            return expression;
        }
        case TokenType::Boolean:
        {
            auto expression = std::make_unique<BooleanLiteralExpression>();
            expression->value = token.literal.asBoolean();
            expression->sourceLocation = token.sourceLocation;
            return expression;
        }
        default:
            throw MalformedExpressionError(token.sourceLocation);
        }
    }
    else
    {
        throwIfTokensEmpty(tokens, token.sourceLocation);
        
        switch (arityMap.at(token.type))
        {
        case 1:     // Handle unary operators
        {
            std::unique_ptr<UnaryExpression> expression;
            
            std::unique_ptr<Expression> operand = generateExpressionFromPostfixTokens(tokens);

            switch (token.type)
            {
            case TokenType::UnaryPlus:
                expression = std::make_unique<UnaryPlusExpression>();
                break;
            case TokenType::UnaryMinus:
                expression = std::make_unique<UnaryMinusExpression>();
                break;
            case TokenType::Not:
                expression = std::make_unique<NotExpression>();
                break;
            default:
                throw MalformedExpressionError(token.sourceLocation);       // This part should not be reached
                break;
            }

            expression->sourceLocation = token.sourceLocation;
            expression->operand = std::move(operand);

            return expression;
        }
        case 2:         // Handle binary operators
        {
            std::unique_ptr<BinaryExpression> expression;

            std::unique_ptr<Expression> right = generateExpressionFromPostfixTokens(tokens);
            throwIfTokensEmpty(tokens, token.sourceLocation);
            std::unique_ptr<Expression> left = generateExpressionFromPostfixTokens(tokens);
            
            switch (token.type)
            {
            case TokenType::Plus:
                expression = std::make_unique<AdditionExpression>(); break;
            case TokenType::Minus:
                expression = std::make_unique<SubtractionExpression>(); break;
            case TokenType::Multiply:
                expression = std::make_unique<MultiplicationExpression>(); break;
            case TokenType::Divide:
                expression = std::make_unique<DivisionExpression>(); break;
            case TokenType::GreaterThan:
                expression = std::make_unique<GreaterThanExpression>(); break;
            case TokenType::GreaterThanOrEqual:
                expression = std::make_unique<GreaterThanOrEqualExpression>(); break;
            case TokenType::LessThan:
                expression = std::make_unique<LessThanExpression>(); break;
            case TokenType::LessThanOrEqual:
                expression = std::make_unique<LessThanOrEqualExpression>(); break;
            case TokenType::Equals:
                expression = std::make_unique<EqualsExpression>(); break;
            case TokenType::NotEqual:
                expression = std::make_unique<NotEqualExpression>(); break;
            case TokenType::And:
                expression = std::make_unique<AndExpression>(); break;
            case TokenType::Or:
                expression = std::make_unique<OrExpression>(); break;
            default:    // This shouldn't ever be reached, so there is a serious problem if it is
                throw MalformedExpressionError(token.sourceLocation);
                break;
            }

            expression->sourceLocation = token.sourceLocation;
            expression->right = std::move(right);
            expression->left = std::move(left);

            return expression;
        }
        default:        // Since (at the moment) only binary and unary operators are used, code shouldn't reach here
            throw MalformedExpressionError(token.sourceLocation);  
            break;
        }
    }
}