#include <string>

#include "../robot/coordinate.hpp"

bool isCloseEnough(double a, double b);
bool isCloseEnough(const Coordinate& a, const Coordinate& b);
void test(const std::string& name, bool condition);