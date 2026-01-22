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
        test
        (
            "Attaching Component to Empty Robot", 
            (robot.getBase() == base)                               && 
            (robot.getEnd() == base)                                &&
            (robot.getComponentFromId(base->getId()) == base)       &&
            (isCloseEnough(robot.getMass(), 5))
        );

        // Aim to see if robot will attach component to the last component added
        // End pointer should change but base pointer should remain the same
        Segment* end = new Segment(1, "End", Coordinate(0, 10), M_PI_2, 10, 10, 5);
        robot.attachToEnd(end);
        test
        (
            "Attaching Component to End of Robot", 
            (robot.getBase() == base)                                   && 
            (robot.getEnd() == end)                                     &&
            (robot.getComponentFromId(end->getId()) == end)             &&
            (isCloseEnough(robot.getMass(), 15)) 
        );

        // Aim to test if attaching component between two components will work
        Segment* middle = new Segment(2, "Middle", Coordinate(0, 20), M_PI_2, 15, 10, 5);
        robot.attachTo(base, middle);
        test
        (
            "Attaching Component in the Middle", 
            (robot.getBase()->front == middle)                          && 
            (robot.getEnd() == middle->front)                           &&
            (robot.getComponentFromId(middle->getId()) == middle)       &&
            (isCloseEnough(robot.getMass(), 30))
        );

        // Aim to test if using the attaching component to end specifically will cause the end pointer to update
        Segment* new_end = new Segment(3, "New End", Coordinate(0, 30), M_PI_2, 20, 10, 5);
        robot.attachTo(end, new_end);
        test
        (
            "Attaching Component in the End Explicitly", 
            (robot.getEnd() == new_end)                                 && 
            (end->front == new_end)                                     &&
            (robot.getComponentFromId(new_end->getId()) == new_end)     &&
            (isCloseEnough(robot.getMass(), 50))
        );

        // Aim to test if the base can be attached to a component
        Segment* new_base = new Segment(4, "New Base", Coordinate(0, 40), M_PI_2, 25, 10, 5);
        robot.attachBaseTo(new_base);
        test
        (
            "Attaching Base to Component", 
            (robot.getBase() == new_base)                               && 
            (new_base->front == base)                                   &&
            (robot.getComponentFromId(new_base->getId()) == new_base)   &&
            (isCloseEnough(robot.getMass(), 75))
        );
    }

    std::cout << "\nTesting the removing function of the robot\n";
    {
        Robot robot(0, "Robot");
        Segment* first = new Segment(1, "First", Coordinate(0, 10), M_PI_2, 5, 10, 5);
        Segment* second = new Segment(2, "Second", Coordinate(0, 20), M_PI_2, 10, 10, 5);
        Segment* third = new Segment(3, "Third", Coordinate(0, 30), M_PI_2, 15, 10, 5);
        Segment* fourth = new Segment(4, "Fourth", Coordinate(0, 30), M_PI_2, 20, 10, 5);
        Segment* fifth = new Segment(5, "Fifth", Coordinate(0, 30), M_PI_2, 25, 10, 5);
        
        robot.attachToEnd(first);
        robot.attachToEnd(second);
        robot.attachToEnd(third);
        robot.attachToEnd(fourth);
        robot.attachToEnd(fifth);

        // Aim to test if component can be removed from the end of robot successfully
        robot.remove(fifth);
        test
        (
            "Removing Component from the End", 
            (robot.getEnd() == fourth)                                  && 
            (fourth->front == nullptr)                                  &&
            (robot.getComponentFromId(fifth->getId()) == nullptr)       &&
            (isCloseEnough(robot.getMass(), 50))
        );

        // Aim to test if component can be removed from the middle of the robot successfully
        robot.remove(third);
        test
        (
            "Removing Component from the Middle", 
            (second->front == fourth)                                   &&
            (robot.getComponentFromId(third->getId()) == nullptr)       &&
            (isCloseEnough(robot.getMass(), 35))                       
        );

        // Aim to test if component can be removed from the base of the robot successfully
        robot.remove(first);
        test
        (
            "Removing Component from the Base", 
            (robot.getBase() == second)                                 &&
            (robot.getComponentFromId(first->getId()) == nullptr)       &&
            (isCloseEnough(robot.getMass(), 30))
        );
    }
    
    std::cout << "\nTesting save to file function of the robot\n";
    {

        std::string filename = "robot_test_files/save_file.rbt";
        Robot robot = Robot(0, "Robot");
        
        // Test Robot can save file when there are no components
        // This corresponds to test 1 in my design section
        robot.saveToFile(filename);
        std::ifstream file(filename);
        
        std::string robot_name;
	    std::getline(file, robot_name);
	    robot_name.erase(robot_name.find_last_not_of(" \t\n\r\f\v") + 1);
        
        test("Saving Empty Robot", robot_name == "Robot");
        
        file.close();

        // Test robot can save file when there is one component
        // This corresponds to test 2 in my design section
        robot.attachToEnd(new Segment(0, "Segment", Coordinate(0, 0), 0, 5, 10, 5));
        robot.saveToFile(filename);

        {
            std::ifstream file(filename);
            std::string line;
            std::getline(file, line);   // This is the name of the robot, which we have already tested
            std::getline(file, line);   // This is the first component
            line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
            
            test("Saving Robot with 1 Component", line == "0|Segment|0|0|0|5|SEGMENT|10|5|");

            file.close();
        }

        // Test robot can save the file when there are three components
        // This will test that all the robot component types are being saved correctly
        // This corresponds to test 3 in my design seciton
        robot.attachToEnd(new LinearActuator(1, "Linear Actuator", Coordinate(10, 0), 0, 5, 10, 10, 5));
        robot.attachToEnd(new Motor(2, "Motor", Coordinate(20, 0), 0, 5, 2, 5));
        robot.saveToFile(filename);

        {
            std::ifstream file(filename);
            std::string line;
            std::string linear_actuator;
            std::string motor;
            std::getline(file, line);   
            std::getline(file, line);               
            std::getline(file, linear_actuator);
            std::getline(file, motor);
            linear_actuator.erase(linear_actuator.find_last_not_of(" \t\n\r\f\v") + 1);
            motor.erase(motor.find_last_not_of(" \t\n\r\f\v") + 1);

            test
            (
                "Saving Robot with 3 Components - Linear Actuator", 
                linear_actuator == "1|Linear Actuator|10|0|0|5|LINEAR ACTUATOR|0|10|10|5|"
            );

            test
            (
                "Saving Robot with 3 Components - Motor",
                motor == "2|Motor|20|0|0|5|MOTOR|0|2|5|"
            );

            file.close();
        }

        // Test robot can save file when there are 5 components
        // This corresponds to test 4 in my design section
        robot.attachToEnd(new Segment(3, "Segment 2", Coordinate(30, 0), 0, 5, 10, 5));
        robot.attachToEnd(new Segment(4, "Segment 3", Coordinate(40, 0), 0, 5, 10, 5));
        robot.saveToFile(filename);

        {
            std::ifstream file(filename);
            std::string line;
            std::string segment_2;
            std::string segment_3;
            std::getline(file, line);   
            std::getline(file, line);
            std::getline(file, line);
            std::getline(file, line);               
            std::getline(file, segment_2);
            std::getline(file, segment_3);
            segment_2.erase(segment_2.find_last_not_of(" \t\n\r\f\v") + 1);
            segment_3.erase(segment_3.find_last_not_of(" \t\n\r\f\v") + 1);

            test
            (
                "Saving Robot with 5 Components",
                (segment_2 == "3|Segment 2|30|0|0|5|SEGMENT|10|5|") &&
                (segment_3 == "4|Segment 3|40|0|0|5|SEGMENT|10|5|")
            );

            file.close();
        }
    }
 
    std::cout << "\nTesting loading from file function of robot\n";
    {
        // Test if robot can load from file which has 0 components in it
        // This corresponds to test 5 from the design section
        {
            Robot robot = Robot::loadFromFile(0, "robot_test_files/robot_with_0_components.rbt");

            test("Loading from File with 0 Components", robot.name == "Robot");
        }

        // Test if robot can be loaded from file which has 1 component in it
        // This corresponds to test 6 from the design section
        {
            Robot robot = Robot::loadFromFile(0, "robot_test_files/robot_with_1_component.rbt");

            test
            (
                "Loading from File with 1 Component", 
                *static_cast<Segment*>(robot.getBase()) == Segment(0, "Segment", Coordinate(0, 0), 0, 5, 10, 5)
            );
        }

        // Test if robot can be loaded from file which has 3 components in it
        // Test that all types of components can be loaded
        // This corresponds to test 7 from the design section
        {
            Robot robot = Robot::loadFromFile(0, "robot_test_files/robot_with_3_components.rbt");

            test
            (
                "Loading from File with 3 Components - Linear Actuator",
                *static_cast<LinearActuator*>(robot.getBase()->front) == 
                LinearActuator(1, "Linear Actuator", Coordinate(10, 0), 0, 5, 10, 10, 5)
            );

            test
            (
                "Loading from File with 3 Components - Motor",
                *static_cast<Motor*>(robot.getEnd()) == Motor(2, "Motor", Coordinate(20, 0), 0, 5, 2, 5)
            );
        }

        // Test if robot can be loaded from file which has 5 components in it
        // Test that all types of components can be loaded
        // This corresponds to test 8 from the design section
        {
            Robot robot = Robot::loadFromFile(0, "robot_test_files/robot_with_3_components.rbt");
            
            test
            (
                "Loading from File with 5 Components",
                (*static_cast<Segment*>(robot.getBase()->front->front->front) == 
                Segment(3, "Segment 2", Coordinate(30, 0), 0, 5, 10, 5)) &&
                (*static_cast<Segment*>(robot.getEnd()) == Segment(4, "Segment 3", Coordinate(40, 0), 0, 5, 10, 5))
            );
        }
    }
    // TODO: Test centre of mass calculations
    // TODO: Test inverse kinematics

}