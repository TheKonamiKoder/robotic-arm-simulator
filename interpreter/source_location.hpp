#pragma once

#include <string>

// The source location is important as it is used for reporting errors
class SourceLocation
{
public:
    int line = 0;
    int column = 0;

    std::string toString() const;
};