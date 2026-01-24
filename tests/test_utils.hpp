#include <string>

#define DEFAULT_EPSILON 0.1

class Coordinate;

// This function is responsible for checking if 2 doubles are close together
// This is needed because floating point arithmetic is not exact and has rounding errors
// Therefore it is better to check if 2 doubles are close together rather than if they are exactly the same
bool isCloseEnough(double a, double b, double epsilon = DEFAULT_EPSILON);

// This is overloading the function, and this is useful when comparing 2 coordinates
// This is necessary to make the testing cleaner and reduce redundancy and make the code less tedious to write
bool isCloseEnough(const Coordinate& a, const Coordinate& b, double epsilon = DEFAULT_EPSILON);

// This function is responsible for running the tests
// If the condition is true, then the test name will be printed as PASSED or else it will be printed as FAILED
void test(const std::string& name, bool condition);