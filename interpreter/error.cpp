#include "error.hpp"

EvaluationError::EvaluationError(const std::string& msg, ErrorType type, int line)
    : std::runtime_error(msg), type(type), line(line) {}