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

    // for (int i = 0; i < postfixTokens.size(); i++)
    // {
    //     std::cout << postfixTokens[i].type << '\t';
    // }
    // std::cout << '\n';

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

        result += test("Numeric Literals", checkEvaluator("50", 50.0));
        result += test("String Literals", checkEvaluator("\"Hello World\"", "Hello World"));
        result += test("Boolean Literal True",  checkEvaluator("TRUE",  true));
        result += test("Boolean Literal False", checkEvaluator("FALSE", false));
    }
    
    if (runAllTests || mode == "--NumericExpressions")
    {
        try
        {
            std::cout << "Testing that evaluator can evaluate numeric expressions\n";

            result += test("Unary Negative Expression", checkEvaluator("-1", -1.0));
            result += test("Simple Expression", checkEvaluator("8 - 6", 2.0));
            result += test("Expression with Multiple Operators", checkEvaluator("(4 + 5) * 2", 18.0));
            result += test("Expression with Variable", checkEvaluator("4 * x", 8.0, {{"x", 2.0}}));
            result += test("Complex expression", checkEvaluator("4 + (x * (y - z))", 7.0, {{"x", 3.0}, {"y", 2.0}, {"z", 1.0}}));
        }
        catch (const std::exception& e)
        {
            std::cerr << "Unhandled exception: " << e.what() << "\n";
        }
        catch (...)
        {
            std::cerr << "Unknown exception occurred\n";
        }
    }

    if (runAllTests || mode == "--LogicalExpressions")
    {
        try
        {
            std::cout << "Testing that the evaluator can evaluate logical expressions\n";

            result += test("Logical NOT TRUE",  checkEvaluator("NOT TRUE",  false));
            result += test("Logical NOT FALSE", checkEvaluator("NOT FALSE", true));

            result += test("Logical OR (true)", checkEvaluator("TRUE OR TRUE", true));
            result += test("Logical OR (false)", checkEvaluator("FALSE OR FALSE", false));
            result += test("Logical AND (true)", checkEvaluator("TRUE AND TRUE", true));
            result += test("Logical AND (false)", checkEvaluator("FALSE AND TRUE", false));

            result += test("Comparison Equals (True)",  checkEvaluator("5 == 5", true));
            result += test("Comparison Equals (False)", checkEvaluator("5 == 3", false));

            result += test("Comparison NotEqual (True)",  checkEvaluator("5 != 3", true));
            result += test("Comparison NotEqual (False)", checkEvaluator("5 != 5", false));

            result += test("Comparison GreaterThan",         checkEvaluator("3 > 5", false));
            result += test("Comparison GreaterThanOrEqual",  checkEvaluator("5 >= 5", true));
            result += test("Comparison LessThan",            checkEvaluator("6 < 5", false));
            result += test("Comparison LessThanOrEqual",     checkEvaluator("5 <= 5", true));

            result += test("Logical AND with Comparison", checkEvaluator("(5 > 3) AND (5 < 4)", false));
            result += test("Logical OR with Comparison",  checkEvaluator("(5 < 3) OR (2 < 4)", true));

            result += test("Mixed Arithmetic and Logic", checkEvaluator("((4 + 5) == 9) AND ((10 - 3) == 7)", true));

            result += test
            (
                "Logical Expression with Variables",
                checkEvaluator("x > 0 AND y < 10", true, {{"x", 5.0}, {"y", 3.0}})
            );
        }
        catch (const std::exception& e)
        {
            std::cerr << "Unhandled exception: " << e.what() << "\n";
        }
        catch (...)
        {
            std::cerr << "Unknown exception occurred\n";
        }
    }
    

    return result;
}