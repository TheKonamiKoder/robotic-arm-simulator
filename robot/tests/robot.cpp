#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>

#include "test_utils.hpp"
#include "../coordinate.hpp"
#include "../segment.hpp"
#include "../linear_actuator.hpp"
#include "../motor.hpp"
#include "../linear_actuator.hpp"
#include "../robot.hpp"

int main()
{
    std::cout << "Testing the robot\n";

    std::cout << "\nTesting attaching function of the robot\n";
    {
        Robot robot = Robot(0, "Robot");

        // Aim to see if the robot will attach the first component to the base
        // Check if both the end and base pointers point to the first component
        Segment* base = new Segment(0, "Base", Coordinate(0, 0), M_PI_2, 5, 10, 5);
        robot.attachToEnd(base);
        test("Attaching Component to Empty Robot", (robot.getBase() == base) && (robot.getEnd() == base));

        // Aim to see if robot will attach component to the last component added
        // End pointer should change but base pointer should remain the same
        Segment* end = new Segment(1, "End", Coordinate(0, 10), M_PI_2, 5, 10, 5);
        robot.attachToEnd(end);
        test("Attaching Component to End of Robot", (robot.getBase() == base) && (robot.getEnd() == end));

        // Aim to test if attaching component between two components will work
        Segment* middle = new Segment(2, "Middle", Coordinate(0, 20), M_PI_2, 5, 10, 5);
        robot.attachTo(base, middle);
        test("Attaching Component in the Middle", (robot.getBase()->front == middle) && (robot.getEnd() == middle->front));

        // Aim to test if using the attaching component to end specifically will cause the end pointer to update
        Segment* new_end = new Segment(3, "New End", Coordinate(0, 30), M_PI_2, 5, 10, 5);
        robot.attachTo(end, new_end);
        test("Attaching Component in the End Explicitly", (robot.getEnd() == new_end) && (end->front == new_end));

        // Aim to test if the base can be attached to a component
        Segment* new_base = new Segment(4, "New Base", Coordinate(0, 40), M_PI_2, 5, 10, 5);
        robot.attachBaseTo(new_base);
        test("Attaching Base to Component", (robot.getBase() == new_base) && (new_base->front == base));
    }

    std::cout << "\nTesting the removing function of the robot\n";
    {
        Robot robot(0, "Robot");
        Segment* first = new Segment(1, "First", Coordinate(0, 10), M_PI_2, 5, 10, 5);
        Segment* second = new Segment(2, "Second", Coordinate(0, 20), M_PI_2, 5, 10, 5);
        Segment* third = new Segment(3, "Third", Coordinate(0, 30), M_PI_2, 5, 10, 5);
        Segment* fourth = new Segment(4, "Fourth", Coordinate(0, 30), M_PI_2, 5, 10, 5);
        Segment* fifth = new Segment(5, "Fifth", Coordinate(0, 30), M_PI_2, 5, 10, 5);
        
        robot.attachToEnd(first);
        robot.attachToEnd(second);
        robot.attachToEnd(third);
        robot.attachToEnd(fourth);
        robot.attachToEnd(fifth);

        // Aim to test if component can be removed from the end of robot successfully
        robot.remove(fifth);
        test("Removing Component from the End", (robot.getEnd() == fourth) && (fourth->front == nullptr));

        // Aim to test if component can be removed from the middle of the robot successfully
        robot.remove(third);
        test("Removing Component from the Middle", second->front == fourth);

        // Aim to test if component can be removed from the base of the robot successfully
        robot.remove(first);
        test("Removing Component from the Base", robot.getBase() == second);
    }
}