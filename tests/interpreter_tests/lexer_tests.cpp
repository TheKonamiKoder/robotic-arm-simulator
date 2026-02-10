#include <iostream>
#include <variant>

#include "test_utils.hpp"

#include "token.hpp"
#include "lexer.hpp"

// This is a helper function that will test if the word gnerateTokenFrom word is working correctly
bool testWord(const std::string& word, const TokenType expectedType, const Literal& literal = Literal(std::monostate()))
{
    Token token = lexer::generateTokenFromWord(word);
    return (token.type == expectedType) && (token.literal == literal);
}

int main(int argc, char* argv[])
{
    int result = 0;

    bool runAllTests = (argc == 1);
    std::string mode = (runAllTests) ? "" : argv[1];

    std::cout << "Testing the lexer\n";

    // Aim to check that tokens can be generated from words
    // This will involve passing a selection of tokens and checking that the correct output is given
    if (runAllTests || mode == "--GenerateTokenFromWord")
    {
        std::cout << "\nTesting generate tokens from word function\n";

        // Pass keywords to lexer
        result += test("Passing keyword VAR to lexer", testWord("VAR", TokenType::Var));
        result += test("Passing keyword IF to lexer", testWord("IF", TokenType::If));
        result += test("Passing keyword ROTATE to lexer", testWord("ROTATE", TokenType::Rotate));

        // Pass numbers to lexer
        result += test("Passing 12 to lexer", testWord("12", TokenType::Number, Literal(12.0)));
        result += test("Passing 0.5 to lexer", testWord("0.5", TokenType::Number, Literal(0.5)));

        // Pass strings to lexer
        result += test("Passing \"Hello World!\" to lexer", testWord("\"Hello World!\"", TokenType::String, Literal(std::string("Hello World!"))));
        // Pass operators to lexer
        result += test("Passing == to lexer", testWord(">=", TokenType::GreaterThanOrEqual));
        result += test("Passing == to lexer", testWord("==", TokenType::Equals));
        result += test("Passing + to lexer", testWord("+", TokenType::Plus));
        result += test("Passing = to lexer", testWord("=", TokenType::Assign));

        // Pass logic to lexer
        result += test("Passing TRUE to lexer", testWord("TRUE", TokenType::Boolean, Literal(true)));
        result += test("Passing FALSE to lexer", testWord("FALSE", TokenType::Boolean, Literal(false)));
        result += test("Passing AND to lexer", testWord("AND", TokenType::And));

        // Pass identifiers to lexer
        result += test("Passing variable to lexer", testWord("variable", TokenType::Identifier, Literal(std::string("variable"))));

        // Pass errors to lexer
        result += test("Passing \"Open String to lexer", testWord("\"Open String", TokenType::Unidentified));
        result += test("Passing 1.2.1 to lexer", testWord("1.2.1", TokenType::Unidentified));
        result += test("Passing variable.with.dots to lexer", testWord("variable.with.dots", TokenType::Unidentified));
    }

    return result;
}