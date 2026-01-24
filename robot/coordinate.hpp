#pragma once

class Coordinate 
{
    public:
        double x = 0;
        double y = 0;
        
        Coordinate() = default;
        Coordinate(double x, double y);

        void rotateAbout(const Coordinate& origin, double angle);
};

bool operator==(const Coordinate& lhs, const Coordinate& rhs);
double distance(const Coordinate& a, const Coordinate& b);