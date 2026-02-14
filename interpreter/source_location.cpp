#include "source_location.hpp"

std::string SourceLocation::toString() const
{
    return "line " + std::to_string(this->line) + ", column " + std::to_string(this->column);
}