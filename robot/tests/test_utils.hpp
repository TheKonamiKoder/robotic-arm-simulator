#include <cmath>
#include <iostream>

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