#include "test_utils.hpp"

#include <cmath>
#include <iostream>

#include "coordinate.hpp"

bool isCloseEnough(double a, double b, double epsilon)
{
    return abs(a - b) < epsilon;
}

bool isCloseEnough(const Coordinate& a, const Coordinate& b, double epsilon)
{
    return (abs(a.x - b.x) < epsilon) && (abs(a.y - b.y) < epsilon);
}

int test(const std::string& name, bool condition)
{
    if (condition)
    {
        std::cout << "\033[32mTEST " << name << " PASSED\033[0m\n";
        return 0;
    }
    else
    {
        std::cout << "\033[31mTEST " << name << " FAILED\033[0m\n";
        return 1;
    }
}