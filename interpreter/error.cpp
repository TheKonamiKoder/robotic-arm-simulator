#include "error.hpp"

RPLError::RPLError(std::string message, SourceLocation sourceLocation) 
    : message(message + " at " + sourceLocation.toString()), sourceLocation(sourceLocation) {}

SourceLocation RPLError::getSourceLocation() const
{
    return this->sourceLocation;
}

const char* RPLError::what() const
{
    return message.c_str();
}

MalformedExpressionError::MalformedExpressionError(SourceLocation sourceLocation)
    : RPLError("Malformed Expression", sourceLocation) {}

MismatchedBracketsError::MismatchedBracketsError(SourceLocation sourceLocation)
    : RPLError("Mismatched Brackets", sourceLocation) {}

UndefinedIdentifierError::UndefinedIdentifierError(std::string identifier, SourceLocation sourceLocation)
    : RPLError("Undefined Identifier: " + identifier, sourceLocation) {}

std::string UndefinedIdentifierError::getIdentifier() const
{
    return this->identifier;
}

DivisionByZeroError::DivisionByZeroError(SourceLocation sourceLocation) 
    : RPLError("Division By Zero", sourceLocation) {}

TypeError::TypeError(std::string expected, std::string actual, SourceLocation sourceLocation)
    : RPLError("Type Error: Expected " + expected + " but got " + actual, sourceLocation) {}

std::string TypeError::getExpected() const
{
    return this->expected;
}

std::string TypeError::getActual() const
{
    return this->actual;
}