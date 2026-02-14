#pragma once

#include <stdexcept>
#include <string>

#include "source_location.hpp"

class RuntimeError : public std::exception
{
protected:
    std::string message;
    SourceLocation sourceLocation;
public:
    RuntimeError(std::string message, SourceLocation sourceLocation = {});
    
    const char* what() const override;

    SourceLocation getSourceLocation() const;
};

class UndefinedIdentifierError : public RuntimeError
{
private:
    std::string identifier;
public:
    UndefinedIdentifierError(std::string identifier, SourceLocation sourceLocation = {});

    std::string getIdentifier() const;
};

class DivisionByZeroError : public RuntimeError
{
public:
    DivisionByZeroError(SourceLocation sourceLocation = {});
};

class TypeError : public RuntimeError
{
private:
    std::string expected;
    std::string actual;
public:
    TypeError(std::string expected, std::string actual, SourceLocation sourceLocation);

    std::string getExpected() const;
    std::string getActual() const;
};
