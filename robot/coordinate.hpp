#pragma once

class Coordinate 
{
    public:
        double x;
        double y;
        
        Coordinate();
        Coordinate(double x, double y);
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