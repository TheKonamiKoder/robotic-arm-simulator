#include "parser.hpp"

#include <iostream>
#include <memory>
#include <unordered_map>

#include "ast.hpp"
#include "behaviour.hpp"
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

std::unique_ptr<Expression> parseNextExpression(std::vector<Token>& tokens)
{
    std::vector<Token> infix;
    
    Token previous;
    previous.type = TokenType::Unknown;
    Token current;

    bool partOfExpression = true;
    while (partOfExpression && !tokens.empty())
    {
        current = tokens.front();

        if 
        (
            (current.type == TokenType::Identifier) || 
            (current.type == TokenType::Number)     || 
            (current.type == TokenType::String)     ||
            (current.type == TokenType::Boolean)    ||
            (current.type == TokenType::LeftBracket)
        )
        {
            // Operands can only come after an operator, a left bracket or at the beginning
            // The same also goes for left brackets as well
            if 
            (
                isOperator(previous.type) || 
                (previous.type == TokenType::LeftBracket) || 
                (previous.type == TokenType::Unknown)
            )
            {
                infix.push_back(current);
                tokens.erase(tokens.begin());                   
            }
            else
            {
                partOfExpression = false;
            }
        }
        else if (isOperator(current.type))
        {
            // Operators can come after anything
            infix.push_back(current);
            tokens.erase(tokens.begin());
        }
        else if (current.type == TokenType::RightBracket)
        {
            // Right brackets can only come after an operand or after another right bracket
            if 
            (
                (previous.type == TokenType::Identifier)  ||
                (previous.type == TokenType::Number)      ||
                (previous.type == TokenType::String)      ||
                (previous.type == TokenType::Boolean)     ||
                (previous.type == TokenType::RightBracket)
            )
            {
                infix.push_back(current);
                tokens.erase(tokens.begin());
            }
            else
            {
                partOfExpression = false;
            }
        }
        else
        {
            partOfExpression = false;
        }
        
        previous = current;
    }
    
    std::vector<Token> postfix = convertInfixToPostfix(infix);
    return generateExpressionFromPostfixTokens(postfix);
}

std::vector<Behaviour> generateBehavioursFromTokens(std::vector<Token>& tokens)
{
    std::vector<Behaviour> behaviours;

    std::string event = "Global";
    std::vector<Token> behaviourTokens;

    bool nextIsEventName = false;

    for (int i = 0; i < tokens.size(); i++)
    {
        Token token = tokens[i];

        if (token.type == TokenType::Endon)
        {
            behaviours.push_back(Behaviour(event, behaviourTokens));
            behaviourTokens.clear();
        }
        else if (token.type == TokenType::On)
        {
            nextIsEventName = true;
        }
        else if (nextIsEventName)
        {
            if (token.type != TokenType::String)
            {
                throw SyntaxError("String", token.toString(), token.sourceLocation);
            }
            
            event = token.literal.asString();
            nextIsEventName = false;
        }
        else
        {
            behaviourTokens.push_back(token);
        }
    }
    
    return behaviours;
}

std::unique_ptr<BlockStatement> parseStatements(std::vector<Token>& tokens)
{
    auto blockStatement = std::make_unique<BlockStatement>();

    while (!tokens.empty())
    {
        blockStatement->statements.push_back(std::move(parseNextStatement(tokens)));
    }
    
    return blockStatement;
}

std::unique_ptr<Statement> parseNextStatement(std::vector<Token>& tokens)
{
    std::unique_ptr<Statement> left = parseNextSingleStatement(tokens);

    // Chain THEN statements
    while (!tokens.empty() && tokens.front().type == TokenType::Then)
    {
        Token thenToken = tokens.front();
        tokens.erase(tokens.begin());

        if (tokens.empty())
        {
            throw SyntaxError("Statement", "", thenToken.sourceLocation);
        }

        std::unique_ptr<Statement> right = parseNextSingleStatement(tokens);
        
        auto chain = std::make_unique<ThenStatement>();
        chain->sourceLocation = thenToken.sourceLocation;
        chain->first = std::move(left);
        chain->next = std::move(right);

        left = std::move(chain);
    }

    return left;
}

std::unique_ptr<Statement> parseNextSingleStatement(std::vector<Token>& tokens)
{
    Token token = tokens.front();

    switch (token.type)
    {
    case TokenType::Var:
        return parseNextVariableDeclarationStatement(tokens);
    case TokenType::Identifier:
        return parseNextAssignmentStatement(tokens);
    case TokenType::Rotate:
        return parseNextRotateStatement(tokens);
    case TokenType::Translate:
        return parseNextTranslateStatement(tokens);
    case TokenType::Raise:
        return parseNextRaiseStatement(tokens);
    case TokenType::Log:
        return parseNextLogStatement(tokens);
    case TokenType::If:
        return parseNextIfStatement(tokens);
    default:
        throw SyntaxError(token.lexeme, token.sourceLocation);
        break;
    }
}

std::unique_ptr<VariableDeclarationStatement> parseNextVariableDeclarationStatement(std::vector<Token>& tokens)
{
    auto statement = std::make_unique<VariableDeclarationStatement>();

    if (tokens.empty())
    {
        throw SyntaxError("VAR", "");
    }
    
    Token var = tokens.front();
    tokens.erase(tokens.begin());

    if (var.type != TokenType::Var)
    {
        throw SyntaxError("VAR", var.lexeme, var.sourceLocation);
    }

    statement->sourceLocation = var.sourceLocation;

    if (tokens.empty())
    {
        throw SyntaxError("Identifier", "", var.sourceLocation);
    }
    
    Token name = tokens.front();
    tokens.erase(tokens.begin());

    if (name.type != TokenType::Identifier)
    {
        throw SyntaxError("Identifier", name.toString());
    }
    
    statement->name = name.literal.asString();

    if (tokens.empty())
    {
        throw SyntaxError("=", "", name.sourceLocation);
    }

    Token equals = tokens.front();
    tokens.erase(tokens.begin());

    if (equals.type != TokenType::Assign)
    {
        throw SyntaxError("=", equals.lexeme, equals.sourceLocation);
    }

    if (tokens.empty())
    {
        throw SyntaxError("Expression", "", equals.sourceLocation);
    }
    
    std::unique_ptr<Expression> expression = parseNextExpression(tokens);

    statement->value = std::move(expression);

    return statement;
}

std::unique_ptr<AssignmentStatement> parseNextAssignmentStatement(std::vector<Token>& tokens)
{
    auto statement = std::make_unique<AssignmentStatement>();

    if (tokens.empty())
    {
        throw SyntaxError("Identifier", "");
    }
    
    Token name = tokens.front();
    tokens.erase(tokens.begin());

    if (name.type != TokenType::Identifier)
    {
        throw SyntaxError("Identifier", name.toString());
    }

    statement->sourceLocation = name.sourceLocation;
    statement->name = name.literal.asString();

    if (tokens.empty())
    {
        throw SyntaxError("=", "", name.sourceLocation);
    }

    Token equals = tokens.front();
    tokens.erase(tokens.begin());

    if (equals.type != TokenType::Assign)
    {
        throw SyntaxError("=", equals.lexeme, equals.sourceLocation);
    }

    if (tokens.empty())
    {
        throw SyntaxError("Expression", "", equals.sourceLocation);
    }
    
    std::unique_ptr<Expression> expression = parseNextExpression(tokens);

    statement->value = std::move(expression);

    return statement;    
}

std::unique_ptr<RotateStatement> parseNextRotateStatement(std::vector<Token>& tokens)
{
    auto statement = std::make_unique<RotateStatement>();

    if (tokens.empty())
    {
        throw SyntaxError("ROTATE", "");
    }

    Token rotate = tokens.front();
    tokens.erase(tokens.begin());
    
    if (rotate.type != TokenType::Rotate)
    {
        throw SyntaxError("ROTATE", rotate.lexeme, rotate.sourceLocation);
    }

    statement->sourceLocation = rotate.sourceLocation;

    if (tokens.empty())
    {
        throw SyntaxError("String Expression", "");
    }

    std::unique_ptr<Expression> motorName = parseNextExpression(tokens);

    statement->motorName = std::move(motorName);

    if (tokens.empty())
    {
        throw SyntaxError("BY", "", motorName->sourceLocation);
    }

    Token by = tokens.front();
    tokens.erase(tokens.begin());

    if (by.type != TokenType::By)
    {
        throw SyntaxError("BY", by.lexeme, by.sourceLocation);
    }

    if (tokens.empty())
    {
        throw SyntaxError("Numeric Expression", "", by.sourceLocation);
    }

    std::unique_ptr<Expression> angularDisplacement = parseNextExpression(tokens);

    statement->angularDisplacement = std::move(angularDisplacement);

    return statement;
}

std::unique_ptr<TranslateStatement> parseNextTranslateStatement(std::vector<Token>& tokens)
{
    auto statement = std::make_unique<TranslateStatement>();

    if (tokens.empty())
    {
        throw SyntaxError("TRANSLATE", "");
    }

    Token translate = tokens.front();
    tokens.erase(tokens.begin());

    if (translate.type != TokenType::Translate)
    {
        throw SyntaxError("TRANSLATE", translate.lexeme, translate.sourceLocation);
    }

    statement->sourceLocation = translate.sourceLocation;

    if (tokens.empty())
    {
        throw SyntaxError("String Expression", "", translate.sourceLocation);
    }

    std::unique_ptr<Expression> linearActuatorName = parseNextExpression(tokens);
    statement->linearActuatorName = std::move(linearActuatorName);

    if (tokens.empty())
    {
        throw SyntaxError("BY", "", linearActuatorName->sourceLocation);
    }

    Token by = tokens.front();
    tokens.erase(tokens.begin());

    if (by.type != TokenType::By)
    {
        throw SyntaxError("BY", by.lexeme, by.sourceLocation);
    }

    if (tokens.empty())
    {
        throw SyntaxError("Numeric Expression", "", by.sourceLocation);
    }

    std::unique_ptr<Expression> displacement = parseNextExpression(tokens);
    statement->displacement = std::move(displacement);

    return statement;
}

std::unique_ptr<RaiseStatement> parseNextRaiseStatement(std::vector<Token>& tokens)
{
    auto statement = std::make_unique<RaiseStatement>();

    if (tokens.empty())
    {
        throw SyntaxError("RAISE", "");
    }

    Token raise = tokens.front();
    tokens.erase(tokens.begin());

    if (raise.type != TokenType::Raise)
    {
        throw SyntaxError("RAISE", raise.lexeme, raise.sourceLocation);
    }

    statement->sourceLocation = raise.sourceLocation;

    if (tokens.empty())
    {
        throw SyntaxError("String", "", raise.sourceLocation);
    }

    Token event = tokens.front();
    tokens.erase(tokens.begin());

    if (event.type != TokenType::String)
    {
        throw SyntaxError("String", event.lexeme, event.sourceLocation);
    }

    statement->event = event.literal.asString();

    return statement;
}

std::unique_ptr<LogStatement> parseNextLogStatement(std::vector<Token>& tokens)
{
    auto statement = std::make_unique<LogStatement>();

    if (tokens.empty())
    {
        throw SyntaxError("LOG", "");
    }

    Token log = tokens.front();
    tokens.erase(tokens.begin());

    if (log.type != TokenType::Log)
    {
        throw SyntaxError("LOG", log.lexeme, log.sourceLocation);
    }

    statement->sourceLocation = log.sourceLocation;

    if (tokens.empty())
    {
        throw SyntaxError("Expression", "", log.sourceLocation);
    }

    std::unique_ptr<Expression> message = parseNextExpression(tokens);

    statement->message = std::move(message);

    return statement;
}

std::unique_ptr<IfStatement> parseNextIfStatement(std::vector<Token>& tokens)
{
    auto statement = std::make_unique<IfStatement>();

    if (tokens.empty())
    {
        throw SyntaxError("IF", "");
    }

    Token ifToken = tokens.front();
    tokens.erase(tokens.begin());

    if (ifToken.type != TokenType::If)
    {
        throw SyntaxError("IF", ifToken.lexeme, ifToken.sourceLocation);
    }

    statement->sourceLocation = ifToken.sourceLocation;

    if (tokens.empty())
    {
        throw SyntaxError("Condition", "", ifToken.sourceLocation);
    }

    statement->condition = parseNextExpression(tokens);

    statement->ifTrueStatements = std::make_unique<BlockStatement>();

    while (!tokens.empty() && ((tokens.front().type != TokenType::Else) || (tokens.front().type != TokenType::Endif)))
    {
        statement->ifTrueStatements->statements.push_back
        (
            std::move(parseNextStatement(tokens))
        );
    }

    if (tokens.empty())
    {
        throw SyntaxError("ELSE", "", statement->condition->sourceLocation);
    }

    Token elseToken = tokens.front();
    tokens.erase(tokens.begin());

    if (elseToken.type != TokenType::Else)
    {
        throw SyntaxError("ELSE", elseToken.lexeme, elseToken.sourceLocation);
    }

    statement->ifFalseStatements = std::make_unique<BlockStatement>();

    while (!tokens.empty() && tokens.front().type != TokenType::Endif)
    {
        statement->ifFalseStatements->statements.push_back
        (
            std::move(parseNextStatement(tokens))
        );
    }

    if (tokens.empty())
    {
        throw SyntaxError("ENDIF", "", elseToken.sourceLocation);
    }

    Token endIfToken = tokens.front();
    tokens.erase(tokens.begin());

    if (endIfToken.type != TokenType::Endif)
    {
        throw SyntaxError("ENDIF", endIfToken.lexeme, endIfToken.sourceLocation);
    }

    return statement;
}
