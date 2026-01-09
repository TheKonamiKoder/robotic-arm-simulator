#include <cmath>
#include <iostream>

#include "../robot/coordinate.hpp"

#define EPSILON 0.1 // This is a small error value that is allowed when checking floating point numbers

// This function is responsible for checking if 2 doubles are close together
// This is needed because floating point arithmetic is not exact and has rounding errors
// Therefore it is better to check if 2 doubles are close together rather than if they are exactly the same
bool isCloseEnough(double a, double b)
{
    if (abs(a - b) < EPSILON)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// This is overloading the function, and this is useful when comparing 2 coordinates
// This is necessary to make the testing cleaner and reduce redundancy and make the code less tedious to write
bool isCloseEnough(Coordinate a, Coordinate b)
{
    if ((abs(a.x - b.x) < EPSILON) && (abs(a.y - b.y) < EPSILON))
    {
        return true;
    }
    else
    {
        return false;
    }
    
}

void test(std::string name, bool condition)
{
    if (condition)
    {
        std::cout << "TEST " << name << " PASSED.\n";
    }
    else
    {
        std::cout << "TEST " << name << " FAILED.\n"; 
    }
}