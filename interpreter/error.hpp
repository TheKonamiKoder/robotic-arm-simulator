#pragma once

#include <stdexcept>

enum class ErrorType 
{
    DivisionByZero
};

class EvaluationError : public std::runtime_error 
{
public:
    ErrorType type;
    int line;

    EvaluationError(const std::string& msg, ErrorType type, int line);
};
