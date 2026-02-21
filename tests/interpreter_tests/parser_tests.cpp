#include <iostream>
#include <memory>
#include <vector>

#include "test_utils.hpp"

#include "ast.hpp"
#include "environment.hpp"
#include "evaluator.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "token.hpp"
#include "symbol_table.hpp"
#include "value.hpp"

bool checkInfixToPostfix(const std::string& infix, const std::string& postfix)
{
    std::vector<Token> infixTokens = generateTokensFromLine(infix);
    std::vector<Token> postfixTokens = convertInfixToPostfix(infixTokens);
    std::vector<Token> expectedPostfixTokens = generateTokensFromLine(postfix);

    for (int i = 0; i < postfixTokens.size(); i++)
    {
        if (postfixTokens[i].type != expectedPostfixTokens[i].type)
        {
            return false;
        }
    }
    
    return true;
}

// This will check that the correct number of tokens are being removed
bool checkParseNextExpression(const std::string& code, int tokensRemoved)
{
    std::vector<Token> tokens = generateTokensFromLine(code);
    int initialSize = tokens.size();
    std::unique_ptr<Expression> expression = parseNextExpression(tokens);
    return (initialSize - tokens.size()) == tokensRemoved;
}

bool checkParseVariableDeclarationExpression(const std::string& code, const std::string& variableName, const Value& expectedValue)
{
    std::vector<Token> tokens = generateTokensFromLine(code);
    std::unique_ptr<VariableDeclarationStatement> statement = parseNextVariableDeclarationStatement(tokens);

    Environment environment{SymbolTable()};
    Evaluator evaluator(environment);
    statement->value->accept(evaluator);

    return (statement->name == variableName) && (evaluator.result == expectedValue);
}

int main(int argc, char* argv[])
{
    int result = 0;

    bool runAllTests = (argc == 1);
    std::string mode = (runAllTests) ? "" : argv[1];

    if (runAllTests)
    {
        std::cout << "Testing the Parser\n";
    }

    if (runAllTests || mode == "--ConvertInfixToPostfix")
    {
        std::cout << "Testing that postfix is being correctly generated from infix\n";
        
        result += test("Passing \"5 + 5\" to infix to postfix generator", checkInfixToPostfix("5 + 5", "5 5 +"));
        result += test("Passing \"5 + 5\" to infix to postfix generator", checkInfixToPostfix("5 + 5", "5 5 +"));
        result += test("Passing \"5 + 6 * 3\" to infix to postfix generator", checkInfixToPostfix("5 + 6 * 3", "5 6 3 * +"));
        result += test("Passing \"5 * (3 + 2)\" to infix to postfix generator", checkInfixToPostfix("5 * (3 + 2)", "5 3 2 + *"));
        result += test("Passing \"5 + y\" to infix to postfix generator", checkInfixToPostfix("5 + y", "5 y +"));
        result += test
        (
            "Passing \"8 + (6 * (3 + x)) - z\" to infix to postfix generator",
            checkInfixToPostfix("8 + (6 * (3 + x)) - z", "8 6 3 x + * + z -")
        );
    }

    if (runAllTests || mode == "--ParseNextExpression")
    {
        std::cout << "Testing that the next expression is being parsed correctly\n";

        result += test("Operand", checkParseNextExpression("\"Hello World\"", 1));
        result += test("Operand, Operand", checkParseNextExpression("x y", 1));
        result += test("Operand, Operator, Operand", checkParseNextExpression("TRUE AND FALSE", 3));
        result += test("Operator Chain", checkParseNextExpression("5 + - 5", 4));
        result += test("Simple Brackets", checkParseNextExpression("(x)", 3));
        result += test("Double Brackets", checkParseNextExpression("((3))", 5));
        result += test("Operator After Bracket", checkParseNextExpression("( + 6 ) THEN", 4));
    }

    if (runAllTests || mode == "--ParseVariableDeclarationStatement")
    {
        std::cout << "Testing that the variable declaration statement is being parsed correctly\n";

        result += test("Number", checkParseVariableDeclarationExpression("VAR x = 53", "x", 53.0));
        result += test("String", checkParseVariableDeclarationExpression("VAR x = \"Hello World\"", "x", "Hello World"));
        result += test("Boolean", checkParseVariableDeclarationExpression("VAR x = TRUE", "x", true));
    }
    
    return result;
}