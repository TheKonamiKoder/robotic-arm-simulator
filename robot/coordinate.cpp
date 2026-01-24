#include "coordinate.hpp"

#include <cmath>

Coordinate::Coordinate(double x, double y) : x(x), y(y) {}

void Coordinate::rotateAbout(const Coordinate& origin, double angle)
{
    // Calculate x and y relative to the origin coordinate, as we are rotating around the origin
    double x_rel = this->x - origin.x;
    double y_rel = this->y - origin.y;

    // Apply the rotation matrix
    double new_x_rel = x_rel * cos(angle) - y_rel * sin(angle);
    double new_y_rel = x_rel * sin(angle) + y_rel * cos(angle);

    this->x = origin.x + new_x_rel;
    this->y = origin.y + new_y_rel;
}

bool operator==(const Coordinate& lhs, const Coordinate& rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

double distance(const Coordinate& a, const Coordinate& b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}