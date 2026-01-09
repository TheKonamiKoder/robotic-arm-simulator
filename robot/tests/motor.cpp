#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <array>

#include "test_utils.hpp"
#include "../coordinate.hpp"
#include "../robot.hpp"

// Defining the following macros so that positions of corners in array is clear
#define TOP_LEFT 0
#define TOP_RIGHT 1
#define BOTTOM_LEFT 2
#define BOTTOM_RIGHT 3

// This function is used to get the corners of a segment and returns it in an array
// This can be used for testing
std::array<Coordinate, 4> getCorners(Segment segment)
{
    std::array<Coordinate, 4> corners;

    // In order to get the angle, we first see what the position of the corner would be in the case where angle is 0
    // However, the angle is not 0 so we transform the coordinate by rotating it around the centre of the segment,
    // which is segment.position, and by the angle that the segment is at
    // This should then give the corner's position
    // When the angle is "0", the segment can be thought as being horizontal

    // First find positions of the corners in the case that the angle of the segment is 0
    corners[TOP_LEFT] = Coordinate
    (
        segment.position.x + (segment.height / 2), 
        segment.position.y + (segment.width / 2)
    );
    corners[TOP_RIGHT] = Coordinate
    (
        segment.position.x + (segment.height / 2), 
        segment.position.y - (segment.width / 2)
    );
    corners[BOTTOM_LEFT] = Coordinate
    (
        segment.position.x - (segment.height / 2),
        segment.position.y + (segment.width / 2)
    );
    corners[BOTTOM_RIGHT] = Coordinate
    (
        segment.position.x - (segment.height / 2),
        segment.position.y - (segment.width / 2)
    );

    // Loop through and rotate all the corners so they are in their actual positions
    for (int i = 0; i < 4; i++)
    {
        corners[i].rotateAbout(segment.position, segment.angle);
    }
    
    return corners;
}

int main()
{
    std::cout << "Testing the motor\n";

    // Aim to test the motor by itself
    // Check that the normalisation of the angle is working correctly
    // Check that the rotation does not exceed the maximum rotation range of the motor
    // This corresponds to test
    std::cout << "\nTesting motor by itself\n";
    {

    }
}