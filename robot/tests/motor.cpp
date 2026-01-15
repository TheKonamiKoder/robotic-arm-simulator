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
        Motor motor = Motor(0, "Motor", Coordinate(0, 0), 0, 5, 2 * M_PI, 5);  // Creating motor with 360 degree rotation

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
        test("Rotating Past 0", isCloseEnough(motor.getRotatedAngle(), 0));
    }

    // Aim to check that the motor can accurately rotate connected segments
    // This corresponds to test 16
    std::cout << "\nTesting motor with one component attached\n";
    {
        Motor motor = Motor(0, "Motor", Coordinate(0, 0), 0, 5, 2 * M_PI, 5);  // Creating motor with 360 degree rotation
        Segment segment = Segment(1, "Segment", Coordinate(10, 0), 0, 5, 10, 5);

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
        motor.rotate(-M_PI_2);
        corners = getCorners(segment);
        test("Clockwise Rotation by 90 for Segment Position", isCloseEnough(segment.position, Coordinate(10, 0)));
        test("Clockwise Rotation by 90 for Top Left Corner", isCloseEnough(corners[TOP_LEFT], Coordinate(15, 2.5)));
        test("Clockwise Rotation by 90 for Top Right Corner", isCloseEnough(corners[TOP_RIGHT], Coordinate(15, -2.5)));
        test("Clockwise Rotation by 90 for Bottom Left Corner", isCloseEnough(corners[BOTTOM_LEFT], Coordinate(5, 2.5)));
        test("Clockwise Rotation by 90 for Bottom Right Corner", isCloseEnough(corners[BOTTOM_RIGHT], Coordinate(5, -2.5)));

        // Test rotating motor by 60 degrees anticlockwise
        motor.rotate(M_PI / 3);
        corners = getCorners(segment);
        test
        (
            "Anticlockwise Rotation by 60 for Segment Position", 
            isCloseEnough(segment.position, Coordinate(5, 5 * sqrt(3)))
        );
        test
        (
            "Anticlockwise Rotation by 60 for Top Left Corner", 
            isCloseEnough(corners[TOP_LEFT], Coordinate(7.5 - (1.25 * sqrt(3)), 7.5 * sqrt(3) + 1.25))
        );
        test
        (
            "Anticlockwise Rotation by 60 for Top Right Corner", 
            isCloseEnough(corners[TOP_RIGHT], Coordinate(7.5 + (1.25 * sqrt(3)), 7.5 * sqrt(3) - 1.25))
        );
        test
        (
            "Anticlockwise Rotation by 60 for Bottom Left Corner", 
            isCloseEnough(corners[BOTTOM_LEFT], Coordinate(2.5 - (1.25 * sqrt(3)), 2.5 * sqrt(3) + 1.25))
        );
        test
        (
            "Anticlockwise Rotation by 60 for Bottom Right Corner", 
            isCloseEnough(corners[BOTTOM_RIGHT], Coordinate(2.5 + (1.25 * sqrt(3)), 2.5 * sqrt(3) - 1.25))
        );
    }

    // Aim to check that the rotation can be applied to all components attached to the front of the motor
    // This corresponds to test 17
    std::cout << "\nTesting motor with 3 components attached\n";
    {
        Motor motor = Motor(0, "Motor", Coordinate(0, 0), 0, 5, 2 * M_PI, 5);  // Creating motor with 360 degree rotation
        Segment first = Segment(1, "First", Coordinate(10, 0), 0, 5, 10, 5);
        Segment second = Segment(1, "Second", Coordinate(20, 0), 0, 5, 10, 5);
        Segment third = Segment(1, "Third", Coordinate(30, 0), 0, 5, 10, 5);
        
        // Manually building the robotic arm
        motor.front = &first;
        first.front = &second;
        second.front = &third;

        // Test rotating motor by 90 degrees anticlockwise
        // Only need to check the position and 1 corner, as the last test should establish that all corners are in the 
        // correct position
        motor.rotate(M_PI_2);
        test("Anticlockwise Rotation by 90 for First Segment Position", isCloseEnough(first.position, Coordinate(0, 10))); 
        test
        (
            "Anticlockwise Rotation by 90 for First Segment Top Left Corner", 
            isCloseEnough(getCorners(first)[TOP_LEFT], Coordinate(-2.5, 15))
        );
        test("Anticlockwise Rotation by 90 for Second Segment Position", isCloseEnough(second.position, Coordinate(0, 20)));
        test
        (
            "Anticlockwise Rotation by 90 for Second Segment Top Left Corner", 
            isCloseEnough(getCorners(second)[TOP_LEFT], Coordinate(-2.5, 25))
        );
        test("Anticlockwise Rotation by 90 for Third Segment Position", isCloseEnough(third.position, Coordinate(0, 30)));
        test
        (
            "Anticlockwise Rotation by 90 for Third Segment Top Left Corner", 
            isCloseEnough(getCorners(third)[TOP_LEFT], Coordinate(-2.5, 35))
        );
        
        // Test rotating motor by 180 degrees clockwise
        motor.rotate(-M_PI);
        test("Anticlockwise Rotation by 90 for First Segment Position", isCloseEnough(first.position, Coordinate(0, -10)));
        test
        (
            "Anticlockwise Rotation by 90 for First Segment Top Left Corner", 
            isCloseEnough(getCorners(first)[TOP_LEFT], Coordinate(2.5, -15))
        );
        test("Anticlockwise Rotation by 90 for Second Segment Position", isCloseEnough(second.position, Coordinate(0, -20)));
        test
        (
            "Anticlockwise Rotation by 90 for Second Segment Top Left Corner", 
            isCloseEnough(getCorners(second)[TOP_LEFT], Coordinate(2.5, -25))
        );
        test("Anticlockwise Rotation by 90 for Third Segment Position", isCloseEnough(third.position, Coordinate(0, -30)));
        test
        (
            "Anticlockwise Rotation by 90 for Third Segment Top Left Corner", 
            isCloseEnough(getCorners(third)[TOP_LEFT], Coordinate(2.5, -35))
        );
    }
}