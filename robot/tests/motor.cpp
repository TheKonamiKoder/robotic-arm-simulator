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
    // This corresponds to tests 14 and 15 in the design document (in the Robot section)
    std::cout << "\nTesting motor by itself\n";
    {
        Motor motor = Motor(0, "Motor", Coordinate(0, 0), 0, 2 * M_PI, 5);  // Creating motor with 360 degree rotation

        // Test rotating motor when it is in range
        motor.rotate(M_PI);
        test("Angular Displacement Within Range", isCloseEnough(motor.getRotatedAngle(), M_PI));

        // Test if it is possible to "add" the extension by rotating motor more than once
        motor.rotate(M_PI / 2);
        test("Anticlockwise Rotation", isCloseEnough(motor.getRotatedAngle(), 1.5 * M_PI));

        // Test if it is possible to rotate motor clockwise
        motor.rotate(-1.5 * M_PI);
        test("Clockwise Rotation", isCloseEnough(motor.getRotatedAngle(), 0));

        // Test normalisation of positive angles
        motor.rotate(5 * M_PI_2);
        test("Normalisation of Positive Angles", isCloseEnough(motor.getRotatedAngle(), M_PI_2));

        // Test normalisation of negative angles
        motor.rotate(-M_PI);
        test("Normalisation of Negative Angles", isCloseEnough(motor.getRotatedAngle(), 3 * M_PI_2));

        motor.rotate(M_PI_2);   // Reset motor to 0
        motor.maximumRotation = M_PI;

        // Test that motor's rotation does not exceed maximum rotation angle
        motor.rotate(1.5 * M_PI);
        test("Surpassing Maximum Rotation", isCloseEnough(motor.getRotatedAngle(), M_PI));

        // Test that motor's rotation does not decrease past 0
        motor.rotate(-1.5 * M_PI);
        test("Rotatingi Past Rotation", isCloseEnough(motor.getRotatedAngle(), M_PI));
    }

    // Aim to check that the motor can accurately rotate connected segments
    // This corresponds to test 16
    std::cout << "\nTesting motor with one component attached\n";
    {
        Motor motor = Motor(0, "Motor", Coordinate(0, 0), 0, 2 * M_PI, 5);  // Creating motor with 360 degree rotation
        Segment segment = Segment(1, "Segment", Coordinate(10, 0), 0, 10, 5);

        motor.front = &segment;     // Manually attaching the segment to the motor
        
        // Test rotating motor by 90 degrees anticlockwise
        motor.rotate(M_PI_2);
        std::array<Coordinate, 4> corners = getCorners(segment);
        test("Anticlockwise Rotation by 90 for Segment Position", isCloseEnough(segment.position, Coordinate(0, 10)));
        test("Anticlockwise Rotation by 90 for Top Left Corner", isCloseEnough(corners[TOP_LEFT], Coordinate(-2.5, 15)));
        test("Anticlockwise Rotation by 90 for Top Right Corner", isCloseEnough(corners[TOP_RIGHT], Coordinate(2.5, 15)));
        test("Anticlockwise Rotation by 90 for Bottom Left Corner", isCloseEnough(corners[BOTTOM_LEFT], Coordinate(-2.5, 5)));
        test("Anticlockwise Rotation by 90 for Bottom Right Corner", isCloseEnough(corners[BOTTOM_RIGHT], Coordinate(2.5, 5)));

        // Test rotating motor by 90 degrees clockwise
        motor.rotate(M_PI_2);
        std::array<Coordinate, 4> corners = getCorners(segment);
        test("Clockwise Rotation by 90 for Segment Position", isCloseEnough(segment.position, Coordinate(10, 0)));
        test("Clockwise Rotation by 90 for Top Left Corner", isCloseEnough(corners[TOP_LEFT], Coordinate(15, 2.5)));
        test("Clockwise Rotation by 90 for Top Right Corner", isCloseEnough(corners[TOP_RIGHT], Coordinate(15, -2.5)));
        test("Clockwise Rotation by 90 for Bottom Left Corner", isCloseEnough(corners[BOTTOM_LEFT], Coordinate(5, 2.5)));
        test("Clockwise Rotation by 90 for Bottom Right Corner", isCloseEnough(corners[BOTTOM_RIGHT], Coordinate(5, -2.5)));
    }
}