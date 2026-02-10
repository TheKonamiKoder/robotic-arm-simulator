#include <iostream>
#include <string>
#include <variant>
#include <vector>

#include "test_utils.hpp"

#include "token.hpp"
#include "lexer.hpp"

// This is a function to help check the expected value of the generateTokenFromWord function
bool checkWord(const std::string& word, const TokenType expectedType, const Literal& literal = Literal(std::monostate()))
{
    Token token = lexer::generateTokenFromWord(word);
    return (token.type == expectedType) && (token.literal == literal);
}

// This is a function to help check the expected value of the generateTokensFromLine function
bool checkLine(std::string line, const std::vector<TokenType>& expectedTypes)
{
    std::vector<Token> tokens = lexer::generateTokensFromLine(line);

    for (int i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].type != expectedTypes[i])
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

    std::cout << "Testing the lexer\n";

    // Aim to check that tokens can be generated from words
    // This will involve passing a selection of tokens and checking that the correct output is given
    if (runAllTests || mode == "--GenerateTokenFromWord")
    {
        std::cout << "\nTesting generate tokens from word function\n";

        // Pass keywords to lexer
        result += test("Passing keyword VAR to lexer", checkWord("VAR", TokenType::Var));
        result += test("Passing keyword IF to lexer", checkWord("IF", TokenType::If));
        result += test("Passing keyword ROTATE to lexer", checkWord("ROTATE", TokenType::Rotate));

        // Pass numbers to lexer
        result += test("Passing 12 to lexer", checkWord("12", TokenType::Number, Literal(12.0)));
        result += test("Passing 0.5 to lexer", checkWord("0.5", TokenType::Number, Literal(0.5)));

        // Pass strings to lexer
        result += test("Passing \"Hello World!\" to lexer", checkWord("\"Hello World!\"", TokenType::String, Literal(std::string("Hello World!"))));
        // Pass operators to lexer
        result += test("Passing == to lexer", checkWord(">=", TokenType::GreaterThanOrEqual));
        result += test("Passing == to lexer", checkWord("==", TokenType::Equals));
        result += test("Passing + to lexer", checkWord("+", TokenType::Plus));
        result += test("Passing = to lexer", checkWord("=", TokenType::Assign));

        // Pass logic to lexer
        result += test("Passing TRUE to lexer", checkWord("TRUE", TokenType::Boolean, Literal(true)));
        result += test("Passing FALSE to lexer", checkWord("FALSE", TokenType::Boolean, Literal(false)));
        result += test("Passing AND to lexer", checkWord("AND", TokenType::And));

        // Pass identifiers to lexer
        result += test("Passing variable to lexer", checkWord("variable", TokenType::Identifier, Literal(std::string("variable"))));

        // Pass errors to lexer
        result += test("Passing \"Open String to lexer", checkWord("\"Open String", TokenType::Unidentified));
        result += test("Passing 1.2.1 to lexer", checkWord("1.2.1", TokenType::Unidentified));
        result += test("Passing variable.with.dots to lexer", checkWord("variable.with.dots", TokenType::Unidentified));
    }

    // Aim to check that tokens can be generated from line
    // This will correspond to tests 1 - 6 in my design
    if (runAllTests || mode == "--GenerateTokensFromLine")
    {
        std::cout << "\nTesting generate tokens from line function\n";

        // Test 1 in design document
        result += test
        (
            "Passing ROTATE motor_1 BY 52 to lexer", 
            checkLine
            (
                std::string("ROTATE motor_1 BY 52"), 
                {TokenType::Rotate, TokenType::Identifier, TokenType::By, TokenType::Number}
            )
        );

        // Test 2 in design document
        result += test
        (
            "Passing \"5+6/3\" to lexer",
            checkLine
            (
                std::string("5+6/3"),
                {TokenType::Number, TokenType::Plus, TokenType::Number, TokenType::Divide, TokenType::Number}
            )
        );

        // Test 3 in design document
        result += test
        (
            "Passing \"5 + 2.5 / 8\" to lexer",
            checkLine
            (
                std::string("5 + 2.5 / 8"),
                {TokenType::Number, TokenType::Plus, TokenType::Number, TokenType::Divide, TokenType::Number}
            )
        );

        // Test 4 in design document
        result += test
        (
            "Passing \"5 + angle\" to lexer",
            checkLine
            (
                std::string("5 + angle"),
                {TokenType::Number, TokenType::Plus, TokenType::Identifier}
            )
        );

        // Test 5 in design document
        result += test
        (
            "Passing \"7.8 + (angle * 3) / x\" to lexer",
            checkLine
            (
                std::string("7.8 + (angle * 3) / x"),
                {
                    TokenType::Number,
                    TokenType::Plus,        
                    TokenType::LeftBracket,   
                    TokenType::Identifier,  
                    TokenType::Multiply,    
                    TokenType::Number,      
                    TokenType::RightBracket,  
                    TokenType::Divide,      
                    TokenType::Identifier   
                }
            )
        );

        // Test 6 in design document
        result += test
        (
            "Passing \"ON 'test event' END\" to lexer",
            checkLine(std::string("ON \"test event\""), {TokenType::On, TokenType::String})
        );

    }
    
    return result;
}