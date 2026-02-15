#include <iostream>
#include <string>
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

bool checkEvaluator
(
    const std::string& infix, 
    const Value& result, 
    std::initializer_list<std::pair<const std::string, Value>> symbolTableMap = {}
)
{
    std::vector<Token> infixTokens = generateTokensFromLine(infix);
    std::vector<Token> postfixTokens = convertInfixToPostfix(infixTokens);

    std::unique_ptr<Expression> expression = generateExpressionFromPostfixTokens(postfixTokens);

    Environment environment{SymbolTable(symbolTableMap)};
    Evaluator evaluator(environment);

    expression->accept(evaluator);

    return evaluator.result == result;    
}

int main(int argc, char* argv[])
{
    int result = 0;

    bool runAllTests = (argc == 1);
    std::string mode = (runAllTests) ? "" : argv[1];

    if (runAllTests)
    {
        std::cout << "Testing the Evaluator\n";
    }

    if (runAllTests || mode == "--Literals")
    {
        std::cout << "Testing that evaluator can evaluate literals\n";

        result += test("Numeric Literals", checkEvaluator("5", 5.0));
    }
    
    if (runAllTests || mode == "--NumericExpressions")
    {
        std::cout << "Testing that evaluator can evaluate numeric expressions\n";

        result += test("Unary Negative Expression", checkEvaluator("-1", -1.0));
        result += test("Simple Expression", checkEvaluator("8 - 6", 2.0));
        result += test("Expression with Multiple Operators", checkEvaluator("(4 + 5) * 2", 18.0));
        result += test("Expression with Variable", checkEvaluator("4 * x", 8.0, {{"x", 2.0}}));
        result += test("Complex expression", checkEvaluator("4 + (x * (y - z))", 7.0, {{"x", 3.0}, {"y", 2.0}, {"z", 1.0}}));
    }

    return result;
}