#include <iostream>
#include <vector>

#include "test_utils.hpp"

#include "lexer.hpp"
#include "parser.hpp"
#include "token.hpp"

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
        std::cout << "Testing that postfix is being correctly generated from infix";
        
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

    return result;
}