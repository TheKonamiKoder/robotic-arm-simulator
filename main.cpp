#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>

#include "robot/coordinate.hpp"
#include "robot/robot.hpp"

int main() 
{
	// Creating a robotic arm manually
	// Robotic arm is at angle of 60 degrees (PI / 3 radians)
    Segment base = Segment(0, "Base", Coordinate(0, 0), 0, 10, 50);
    LinearActuator linearActuator = LinearActuator(1, "Linear Actuator", 
						Coordinate(2.5, 2.5 * sqrt(3)), M_PI / 3, 10, 10, 5);
    Segment middle = Segment(2, "Middle", 
                        Coordinate(7.5, 7.5 * sqrt(3)), M_PI / 3, 10, 5);
    Segment end = Segment(3, "End", 
                        Coordinate(12.5, 12.5 * sqrt(3)), M_PI / 3, 10, 5);
    
    std::cout << middle.position.x << ' ' << middle.position.y << std::endl;
    std::cout << end.position.x << ' ' << end.position.y << std::endl;
    
    base.front = &linearActuator;
    linearActuator.front = &middle;
    middle.front = &end;
    
    linearActuator.translate(4);
    
	std::cout << "TRANSLATION BY 4" << '\n';
    std::cout << "MIDDLE COMPONENT POSITION: " << middle.position.x << ' ' 
              << middle.position.y << '\n';
    std::cout << "END COMPONENT POSITION: " << end.position.x << ' ' 
              << end.position.y << '\n';
    
    linearActuator.translate(4);
    
	std::cout << "TRANSLATION BY 4" << '\n';
    std::cout << "MIDDLE COMPONENT POSITION: " << middle.position.x << ' ' 
              << middle.position.y << '\n';
    std::cout << "END COMPONENT POSITION: " << end.position.x << ' ' 
              << end.position.y << '\n';
    
    linearActuator.translate(-2);
    
	std::cout << "TRANSLATION BY -2" << '\n';
    std::cout << "MIDDLE COMPONENT POSITION: " << middle.position.x << ' ' 
              << middle.position.y << '\n';
    std::cout << "END COMPONENT POSITION: " << end.position.x << ' ' 
              << end.position.y << '\n';
}
