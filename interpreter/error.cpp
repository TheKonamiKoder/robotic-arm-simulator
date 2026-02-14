#include "error.hpp"

RuntimeError::RuntimeError(std::string message, SourceLocation sourceLocation) 
    : message(message + " at " + sourceLocation.toString()), sourceLocation(sourceLocation) {}

SourceLocation RuntimeError::getSourceLocation() const
{
    return this->sourceLocation;
}

const char* RuntimeError::what() const
{
    return message.c_str();
}

UndefinedIdentifierError::UndefinedIdentifierError(std::string identifier, SourceLocation sourceLocation)
    : RuntimeError("Undefined Identifier: " + identifier, sourceLocation) {}

std::string UndefinedIdentifierError::getIdentifier() const
{
    return this->identifier;
}

DivisionByZeroError::DivisionByZeroError(SourceLocation sourceLocation) 
    : RuntimeError("Division By Zero", sourceLocation) {}

TypeError::TypeError(std::string expected, std::string actual, SourceLocation sourceLocation)
    : RuntimeError("Type Error: Expected " + expected + " but got " + actual, sourceLocation) {}

std::string TypeError::getExpected() const
{
    return this->expected;
}

std::string TypeError::getActual() const
{
    return this->actual;
}