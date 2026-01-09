#pragma once

class Coordinate 
{
    public:
        double x;
        double y;
        
        Coordinate();
        Coordinate(double x, double y);
        void rotateAbout(Coordinate origin, double angle);
};

Coordinate::Coordinate() 
{
    this->x = 0;
    this->y = 0;
}

Coordinate::Coordinate(double x, double y) 
{
    this->x = x;
    this->y = y;
}

void Coordinate::rotateAbout(Coordinate origin, double angle)
{
    // Calculate x and y relative to the origin coordinate, as we are rotating around the origin
    double x_rel = this->x - origin.x;
    double y_rel = this->y - origin.y;

    // Apply the rotation matrix
    double dx = x_rel * cos(angle) - y_rel * sin(angle);
    double dy = x_rel * sin(angle) + y_rel * cos(angle);

    this->x += dx;
    this->y += dy;
}