#pragma once

#include <memory>
#include <vector>

#include "behaviour.hpp"

class Expression;
class Statement;
class BlockStatement;
class VariableDeclarationStatement;
class AssignmentStatement;
class RotateStatement;
class TranslateStatement;
class RaiseStatement;
class LogStatement;
class IfStatement;
class ThenStatement;
class Token;

// This will check whether the next token should be unary
// If the token is at the beginning of the file, a token with type Unknown should be passed in
bool isUnaryContext(const Token& previous);

// This function will return tokens in postfix rather than in infix, which makes it easy to generate the AST
std::vector<Token> convertInfixToPostfix(std::vector<Token>& tokens);

// This function will build the AST from a group of postfix expressions
std::unique_ptr<Expression> generateExpressionFromPostfixTokens(std::vector<Token>& tokens);

// This function will return true or false depending on the last token, suggesting if the next token is part of the same
// expression
bool isExpressionContext(const Token& previous);

// This function will parse the next expression from the tokens (it will also consume any tokens that are used)
std::unique_ptr<Expression> parseNextExpression(std::vector<Token>& tokens);

std::vector<Behaviour> generateBehavioursFromTokens(std::vector<Token>& tokens);

std::unique_ptr<BlockStatement> parseStatements(std::vector<Token>& tokens);

// This will handle chaining THEN statements together as well
std::unique_ptr<Statement> parseNextStatement(std::vector<Token>& tokens);

std::unique_ptr<Statement> parseNextSingleStatement(std::vector<Token>& tokens);

std::unique_ptr<VariableDeclarationStatement> parseNextVariableDeclarationStatement(std::vector<Token>& tokens);

std::unique_ptr<AssignmentStatement> parseNextAssignmentStatement(std::vector<Token>& tokens);

std::unique_ptr<RotateStatement> parseNextRotateStatement(std::vector<Token>& tokens);

std::unique_ptr<TranslateStatement> parseNextTranslateStatement(std::vector<Token>& tokens);

std::unique_ptr<RaiseStatement> parseNextRaiseStatement(std::vector<Token>& tokens);

std::unique_ptr<LogStatement> parseNextLogStatement(std::vector<Token>& tokens);

std::unique_ptr<IfStatement> parseNextIfStatement(std::vector<Token>& tokens);
