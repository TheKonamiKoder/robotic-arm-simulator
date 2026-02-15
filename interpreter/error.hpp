#pragma once

#include <stdexcept>
#include <string>

#include "source_location.hpp"

class RPLError : public std::exception
{
protected:
    std::string message;
    SourceLocation sourceLocation;
public:
    RPLError(std::string message, SourceLocation sourceLocation = {});
    
    const char* what() const override;

    SourceLocation getSourceLocation() const;
};

class MalformedExpressionError : public RPLError
{
public:
    MalformedExpressionError(SourceLocation sourceLocation = {});
};

class MismatchedBracketsError : public RPLError
{
public:
    MismatchedBracketsError(SourceLocation sourceLocation = {});
};

class UndefinedIdentifierError : public RPLError
{
private:
    std::string identifier;
public:
    UndefinedIdentifierError(std::string identifier, SourceLocation sourceLocation = {});

    std::string getIdentifier() const;
};

class DivisionByZeroError : public RPLError
{
public:
    DivisionByZeroError(SourceLocation sourceLocation = {});
};

class TypeError : public RPLError
{
private:
    std::string expected;
    std::string actual;
public:
    TypeError(std::string expected, std::string actual, SourceLocation sourceLocation);

    std::string getExpected() const;
    std::string getActual() const;
};
