#define _USE_MATH_DEFINES

#include <cmath>
#include <fstream>
#include <iostream>

#include "test_utils.hpp"
#include "coordinate.hpp"
#include "segment.hpp"
#include "linear_actuator.hpp"
#include "motor.hpp"
#include "linear_actuator.hpp"
#include "robot.hpp"

int main(int argc, char* argv[])
{
    int result = 0;

    bool runAllTests = (argc == 1);
    std::string mode = runAllTests ? "" : argv[1];
    
    if (runAllTests)
    {
        std::cout << "Testing the robot\n";
    }

    if (runAllTests || mode == "--Attach")
    {
        std::cout << "\nTesting attaching function of the robot\n";

        Robot robot = Robot(0, "Robot");

        // Aim to see if the robot will attach the first component to the base
        // Check if both the end and base pointers point to the first component
        Segment* base = new Segment(0, "Base", Coordinate(0, 0), M_PI_2, 5, 10, 5);
        robot.attachToEnd(base);
        result += test
        (
            "Attaching Component to Empty Robot", 
            (robot.getBase() == base)                                   && 
            (robot.getEnd() == base)                                    &&
            (robot.getComponentFromId(base->getId()) == base)           &&
            (isCloseEnough(robot.getMass(), 5))
        );

        // Aim to see if robot will attach component to the last component added
        // End pointer should change but base pointer should remain the same
        Segment* end = new Segment(1, "End", Coordinate(0, 10), M_PI_2, 10, 10, 5);
        robot.attachToEnd(end);
        result += test
        (
            "Attaching Component to End of Robot", 
            (robot.getBase() == base)                                   && 
            (robot.getEnd() == end)                                     &&
            (end->back == base)                                         &&
            (robot.getComponentFromId(end->getId()) == end)             &&
            (isCloseEnough(robot.getMass(), 15)) 
        );

        // Aim to test if attaching component between two components will work
        Segment* middle = new Segment(2, "Middle", Coordinate(0, 20), M_PI_2, 15, 10, 5);
        robot.attachTo(base, middle);
        result += test
        (
            "Attaching Component in the Middle", 
            (robot.getBase()->front == middle)                          && 
            (robot.getEnd() == middle->front)                           &&
            (middle->back == base)                                      &&
            (end->back == middle)                                       &&
            (robot.getComponentFromId(middle->getId()) == middle)       &&
            (isCloseEnough(robot.getMass(), 30))
        );

        // Aim to test if using the attaching component to end specifically will cause the end pointer to update
        Segment* new_end = new Segment(3, "New End", Coordinate(0, 30), M_PI_2, 20, 10, 5);
        robot.attachTo(end, new_end);
        result += test
        (
            "Attaching Component in the End Explicitly", 
            (robot.getEnd() == new_end)                                 && 
            (end->front == new_end)                                     &&
            (new_end->back == end)                                      &&
            (robot.getComponentFromId(new_end->getId()) == new_end)     &&
            (isCloseEnough(robot.getMass(), 50))
        );

        // Aim to test if the base can be attached to a component
        Segment* new_base = new Segment(4, "New Base", Coordinate(0, 40), M_PI_2, 25, 10, 5);
        robot.attachBaseTo(new_base);
        result += test
        (
            "Attaching Base to Component", 
            (robot.getBase() == new_base)                               && 
            (new_base->front == base)                                   &&
            (base->back == new_base)                                    &&
            (robot.getComponentFromId(new_base->getId()) == new_base)   &&
            (isCloseEnough(robot.getMass(), 75))
        );
    }

    if (runAllTests || mode == "--Remove")
    {
        std::cout << "\nTesting the removing function of the robot\n";

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
        result += test
        (
            "Removing Component from the End", 
            (robot.getEnd() == fourth)                                  && 
            (fourth->front == nullptr)                                  &&
            (robot.getComponentFromId(fifth->getId()) == nullptr)       &&
            (isCloseEnough(robot.getMass(), 50))
        );

        // Aim to test if component can be removed from the middle of the robot successfully
        robot.remove(third);
        result += test
        (
            "Removing Component from the Middle", 
            (second->front == fourth)                                   &&
            (fourth->back == second)                                    &&
            (robot.getComponentFromId(third->getId()) == nullptr)       &&
            (isCloseEnough(robot.getMass(), 35))                       
        );

        // Aim to test if component can be removed from the base of the robot successfully
        robot.remove(first);
        result += test
        (
            "Removing Component from the Base", 
            (robot.getBase() == second)                                 &&
            (second->back == nullptr)                                   &&
            (robot.getComponentFromId(first->getId()) == nullptr)       &&
            (isCloseEnough(robot.getMass(), 30))
        );
    }
    
    if (runAllTests || "--SaveToFile")
    {
        std::cout << "\nTesting save to file function of the robot\n";

        std::string filename = std::string(TEST_DATA_DIR) + "/save_file.rbt";
        Robot robot = Robot(0, "Robot");
        
        // Test Robot can save file when there are no components
        // This corresponds to test 1 in my design section
        robot.saveToFile(filename);
        std::ifstream file(filename);
        
        std::string robot_name;
	    std::getline(file, robot_name);
	    robot_name.erase(robot_name.find_last_not_of(" \t\n\r\f\v") + 1);
        
        result += test("Saving Empty Robot", robot_name == "Robot");
        
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
            
            result += test("Saving Robot with 1 Component", line == "0|Segment|0|0|0|5|SEGMENT|10|5|");

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

            result += test
            (
                "Saving Robot with 3 Components - Linear Actuator", 
                linear_actuator == "1|Linear Actuator|10|0|0|5|LINEAR ACTUATOR|0|10|10|5|"
            );

            result += test
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

            result += test
            (
                "Saving Robot with 5 Components",
                (segment_2 == "3|Segment 2|30|0|0|5|SEGMENT|10|5|") &&
                (segment_3 == "4|Segment 3|40|0|0|5|SEGMENT|10|5|")
            );

            file.close();
        }
    }
 
    if (runAllTests || mode == "--LoadFromFile")   
    {
        std::cout << "\nTesting loading from file function of robot\n";
        // Test if robot can load from file which has 0 components in it
        // This corresponds to test 5 from the design section
        {
            Robot robot(0);
            robot.loadFromFile(std::string(TEST_DATA_DIR) + "/robot_with_0_components.rbt");

            result += test("Loading from File with 0 Components", robot.name == "Robot");
        }

        // Test if robot can be loaded from file which has 1 component in it
        // This corresponds to test 6 from the design section
        {
            Robot robot(0);
            robot.loadFromFile(std::string(TEST_DATA_DIR) + "/robot_with_1_component.rbt");

            result += test
            (
                "Loading from File with 1 Component", 
                *static_cast<Segment*>(robot.getBase()) == Segment(0, "Segment", Coordinate(0, 0), 0, 5, 10, 5)
            );
        }

        // Test if robot can be loaded from file which has 3 components in it
        // Test that all types of components can be loaded
        // This corresponds to test 7 from the design section
        {
            Robot robot(0);
            robot.loadFromFile(std::string(TEST_DATA_DIR) + "/robot_with_3_components.rbt");

            result += test
            (
                "Loading from File with 3 Components - Linear Actuator",
                *static_cast<LinearActuator*>(robot.getBase()->front) == 
                LinearActuator(1, "Linear Actuator", Coordinate(10, 0), 0, 5, 10, 10, 5)
            );

            result += test
            (
                "Loading from File with 3 Components - Motor",
                *static_cast<Motor*>(robot.getEnd()) == Motor(2, "Motor", Coordinate(20, 0), 0, 5, 2, 5)
            );
        }

        // Test if robot can be loaded from file which has 5 components in it
        // Test that all types of components can be loaded
        // This corresponds to test 8 from the design section
        {
            Robot robot(0);
            robot.loadFromFile(std::string(TEST_DATA_DIR) + "/robot_with_5_components.rbt");

            result += test
            (
                "Loading from File with 5 Components",
                (*static_cast<Segment*>(robot.getBase()->front->front->front) == 
                Segment(3, "Segment 2", Coordinate(30, 0), 0, 5, 10, 5)) &&
                (*static_cast<Segment*>(robot.getEnd()) == Segment(4, "Segment 3", Coordinate(40, 0), 0, 5, 10, 5))
            );
        }
    }

    if (runAllTests || mode == "--GetCentreOfMass")
    {
        std::cout << "\nTesting the Centre of Mass\n";

        // Aim to test centre of mass with one component
        // This corresponds to test 23 from my design section
        Robot robot(0, "Robot");
        robot.attachToEnd(new LinearActuator(0, "Linear Actuator", Coordinate(0, 0), M_PI_2, 10, 10, 10, 5));
        result += test("Caclulate Centre of Mass with 1 Component", isCloseEnough(robot.getCentreOfMass(), Coordinate(0, 0)));
        static_cast<LinearActuator*>(robot.getEnd())->translate(5);
        result += test
        (
            "Calculate Centre of Mass when Linear Actuator Moves", 
            isCloseEnough(robot.getCentreOfMass(), Coordinate(0, 2.5))
        );

        // Aim to test centre of mass with 3 components
        // This corresponds to test 24 from my design section
        robot.attachToEnd(new Segment(1, "Segment 1", Coordinate(0, 15), M_PI_2, 20, 10, 5));
        robot.attachToEnd(new Segment(2, "Segment 2", Coordinate(0, 25), M_PI_2, 30, 10, 5));
        result += test
        (
            "Calculate Centre of Mass with 3 Components", 
            isCloseEnough(robot.getCentreOfMass(), Coordinate(0, (215.0 / 12.0)))
        );
    }

    if (runAllTests || mode == "--MoveTo")
    {
        std::cout << "\nTesting Inverse Kinematics\n";

        // Test inverse kinematics algorithm with only 1 linear actuator
        // This corresponds to test 19 in my design section
        {
            Robot robot(0, "Robot");
            robot.attachToEnd(new LinearActuator(0, "Linear Actuator", Coordinate(0, 0), M_PI_2, 5, 10, 10, 5));
            robot.moveTo(Coordinate(0, 10));
            result += test
            (
                "Inverse Kinematics with 1 Linear Actuator", 
                isCloseEnough(robot.getEndEffectorPosition(), Coordinate(0, 10), 1)
            );
        }

        // Test IK algorithm with 2 linear actuators
        // This corresponds to test 21 in my design section
        {
            Robot robot(0, "Robot");
            robot.attachToEnd(new LinearActuator(0, "Linear Actuator 1", Coordinate(0, 0), M_PI_2, 5, 10, 10, 5));
            robot.attachToEnd(new LinearActuator(1, "Linear Actuator 2", Coordinate(5, 5), 0, 5, 10, 10, 5));
            robot.moveTo(Coordinate(15, 10));
            result += test
            (
                "Inverse Kinematics with 2 Linear Actuators", 
                isCloseEnough(robot.getEndEffectorPosition(), Coordinate(15, 10), 1)
            );
        }

        // Test inverse kinematics algorithm with only 1 motor
        // This corresponds to test 18 in my design section
        {
            Robot robot(0, "Robot");
            robot.attachToEnd(new Motor(0, "Motor", Coordinate(0, 0), 0, 5, 2 * M_PI, 5));
            robot.moveTo(Coordinate(0, 5));
            result += test
            (
                "Inverse Kinematics with 1 Motor", 
                isCloseEnough(robot.getEndEffectorPosition(), Coordinate(0, 5), 1)
            );
        }

        // Test inverse kinematics algorithm with 2 motors
        // This corresponds to test 20 in my design section
        {
            Robot robot(0, "Robot");
            robot.attachToEnd(new Motor(0, "Motor 1", Coordinate(0, 0), 0, 5, 2 * M_PI, 1));
            robot.attachToEnd(new Segment(1, "Segment 1", Coordinate(5, 0), 0, 5, 8, 2));
            robot.attachToEnd(new Motor(2, "Motor 2", Coordinate(10, 0), 0, 5, 2 * M_PI, 1));
            robot.attachToEnd(new Segment(3, "Segment 2", Coordinate(15, 0), 0, 5, 8, 2));
            robot.moveTo(Coordinate(19 / 2, 19 * sqrt(3) / 2));
            result += test
            (
                "Inverse Kinematics with 2 Motors", 
                isCloseEnough(robot.getEndEffectorPosition(), Coordinate(19 / 2, 19 * sqrt(3) / 2), 1)
            ); 
        }

        // Test inverse kinematics algorithm with motors that cannot rotate fully
        {
            Robot robot(0, "Robot");
            robot.attachToEnd(new Motor(0, "Motor", Coordinate(0, 0), 0, 5, M_PI_2, 5));
            
            robot.moveTo(Coordinate(5 / sqrt(2), 5 / sqrt(2)));
            result += test
            (
                "Inverse Kinematics with Bounded Motor - Within Bounds",
                isCloseEnough(static_cast<Motor*>(robot.getBase())->getRotatedAngle(), M_PI_4)
            );

            robot.moveTo(Coordinate(-5 / sqrt(2), 5 / sqrt(2)));
            result += test
            (
                "Inverse Kinematics with Bounded Motor - Beyond Maximum Rotation",
                isCloseEnough(static_cast<Motor*>(robot.getBase())->getRotatedAngle(), M_PI_2)
            );

            robot.moveTo(Coordinate(5 / sqrt(2), -5 / sqrt(2)));
            result += test
            (
                "Inverse Kinematics with Bounded Motor - Below 0",
                isCloseEnough(static_cast<Motor*>(robot.getBase())->getRotatedAngle(), 0)
            );
        }

        // Test inverse kinematics algorithm with both motor and linear actuator
        // This corresponds to test 22 in my design section
        {
            Robot robot(0, "Robot");
            robot.attachToEnd(new Motor(0, "Motor", Coordinate(0, 0), 0, 5, 2 * M_PI, 1));
            robot.attachToEnd(new LinearActuator(1, "Linear Actuator", Coordinate(5, 0), 0, 5, 4, 8, 2));

            robot.moveTo(Coordinate(0, 12));
            result += test
            (
                "Inverse Kinematics with Motor and Linear Actuator", 
                isCloseEnough(robot.getEndEffectorPosition(), Coordinate(0, 12), 1)
            ); 
        }
    }

    return result;
}