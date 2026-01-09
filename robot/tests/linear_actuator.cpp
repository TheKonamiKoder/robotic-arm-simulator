#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>

#include "test_utils.hpp"
#include "../coordinate.hpp"
#include "../robot.hpp"

int main() 
{
    std::cout << "Testing the linear actuator\n";

    // Aim to check how the extension of the linear actuator varies
    // Will check that the the actuator does not extend past its maximum value or compress into itself
    // This links to tests 9 and 10 within the design (for Robot section)
    std::cout << "\nTesting linear actuator by itself\n";
    {
        LinearActuator linearActuator = LinearActuator(0, "Linear Actuator", Coordinate(0, 0), M_PI_2, 10, 10, 5);
        
        // Test valid input of translating by 5, which is within the acceptable range
        linearActuator.translate(5);
        test("Displacement Within Range", isCloseEnough(linearActuator.getExtension(), 5));
        
        // Test if it is possible to "add" the extension by translating it several times
        linearActuator.translate(1);
        test("Modifying Extension", isCloseEnough(linearActuator.getExtension(), 6));

        // Test that the extension does not surpass max extension (10)
        linearActuator.translate(6);
        test("Surpassing Maximum Extension", isCloseEnough(linearActuator.getExtension(), 10));

        // Test that the extension can be reduced
        linearActuator.translate(-3);
        test("Decreasing Extension", isCloseEnough(linearActuator.getExtension(), 7));

        // Test that the extension won't go below 0
        linearActuator.translate(-10);
        test("Extending Below 0", isCloseEnough(linearActuator.getExtension(), 0));
    }

    // Aim to check if the extension can cause connected robotic segments to change their position
    // This corresponds to test 11
    std::cout << "\nTesting linear actuator with 1 component attached\n";
    {
        LinearActuator linearActuator = LinearActuator(0, "Linear Actuator", Coordinate(0, 0), M_PI_2, 10, 10, 5);
        Segment segment = Segment(1, "Segment", Coordinate(0, 15), M_PI_2, 10, 5);

        linearActuator.front = &segment;    // Manually "attaching" the segment to the front of the linear actuator

        // Test increasing the extension
        linearActuator.translate(5);
        test("Increasing Extension", isCloseEnough(segment.position.y, 20));

        // Test decreasing the extension
        linearActuator.translate(-2);
        test("Decreasing Extension", isCloseEnough(segment.position.y, 18));
    }

    // Aim to check if the extension can propogate through multiple connected components and change their position
    // This corresponds to test 12
    std::cout << "\nTesting linear actuator with 3 components attached\n";
    {
        LinearActuator linearActuator = LinearActuator(0, "Linear Actuator", Coordinate(0, 0), M_PI_2, 10, 10, 5);
        Segment first = Segment(1, "First", Coordinate(0, 15), M_PI_2, 10, 5);
        Segment second = Segment(1, "Second", Coordinate(0, 25), M_PI_2, 10, 5);
        Segment third = Segment(1, "Third", Coordinate(0, 35), M_PI_2, 10, 5);
        
        // Manually building the robotic arm
        linearActuator.front = &first;
        first.front = &second;
        second.front = &third;

        // Test increasing the extension and checking the effect on each segment
        linearActuator.translate(5);
        test("Increasing Extension for First Segment", isCloseEnough(first.position.y, 20));
        test("Increasing Extension for Second Segment", isCloseEnough(second.position.y, 30));
        test("Increasing Extension for Third Segment", isCloseEnough(third.position.y, 40));

        // Test decreasing the extension and checking the effect on each segment
        linearActuator.translate(-3);
        test("Decreasing Extension for First Segment", isCloseEnough(first.position.y, 17));
        test("Decreasing Extension for Second Segment", isCloseEnough(second.position.y, 27));
        test("Decreasing Extension for Third Segment", isCloseEnough(third.position.y, 37));    
    }

    // Aim to check if the extension method works for angles that are not 90 degrees
    // This corresponds to test 13
    std::cout << "\nTesting linear actuator at an angle\n";
    {
        LinearActuator linearActuator = LinearActuator(0, "Linear Actuator", Coordinate(2.5, 2.5 * sqrt(3)), M_PI / 3, 
                                                        10, 10, 5);
        Segment first = Segment(1, "First", Coordinate(7.5, 7.5 * sqrt(3)), M_PI_2, 10, 5);
        Segment second = Segment(1, "Second", Coordinate(12.5, 12.5 * sqrt(3)), M_PI_2, 10, 5);
        Segment third = Segment(1, "Third", Coordinate(17.5, 17.5 * sqrt(3)), M_PI_2, 10, 5);

        // Manually building the robotic arm
        linearActuator.front = &first;
        first.front = &second;
        second.front = &third;
        
        // Test increasing the extension and checking the effect on each segment
        linearActuator.translate(4);
        test("Increasing Extension for First Segment x", isCloseEnough(first.position.x, 9.5));
        test("Increasing Extension for First Segment y", isCloseEnough(first.position.y, 9.5 * sqrt(3)));
        test("Increasing Extension for Second Segment x", isCloseEnough(second.position.x, 14.5));
        test("Increasing Extension for Second Segment y", isCloseEnough(second.position.y, 14.5 * sqrt(3)));
        test("Increasing Extension for Third Segment x", isCloseEnough(third.position.x, 19.5));
        test("Increasing Extension for Third Segment y", isCloseEnough(third.position.y, 19.5 * sqrt(3)));

        // Test decreasing the extension and checking the effect on each segment
        linearActuator.translate(-2);
        test("Decreasing Extension for First Segment x", isCloseEnough(first.position.x, 8.5));
        test("Decreasing Extension for First Segment y", isCloseEnough(first.position.y, 8.5 * sqrt(3)));
        test("Decreasing Extension for Second Segment x", isCloseEnough(second.position.x, 13.5));
        test("Decreasing Extension for Second Segment y", isCloseEnough(second.position.y, 13.5 * sqrt(3)));
        test("Decreasing Extension for Third Segment x", isCloseEnough(third.position.x, 18.5));
        test("Decreasing Extension for Third Segment y", isCloseEnough(third.position.y, 18.5 * sqrt(3)));
    }
}   