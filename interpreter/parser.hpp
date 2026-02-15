#pragma once

#include <memory>
#include <vector>

class Expression;
class Token;

std::unique_ptr<Expression> generateExpressionFromTokens(std::vector<Token> tokens);

// This function will return tokens in postfix rather than in infix, which makes it easy to generate the AST
std::vector<Token> convertInfixToPostfix(std::vector<Token>& tokens);

// This will check whether the next token should be unary
// If the token is at the beginning of the file, a token with type Unknown should be passed in
bool isUnaryContext(const Token& previous);