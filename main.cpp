#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>

#include "robot/coordinate.hpp"
#include "robot/robot.hpp"

int main() 
{
    Segment base = Segment(0, "Base", Coordinate(0, 0), 0, 10, 50);
    LinearActuator linearActuator = LinearActuator(1, "Linear Actuator", 
										Coordinate(0, 5), M_PI / 2, 10, 10, 5);
    Segment middle = Segment(2, "Middle", Coordinate(0, 15), M_PI / 2, 10, 5);
    Segment end = Segment(3, "End", Coordinate(0, 25), M_PI / 2, 10, 5);
    
    std::cout << middle.position.x << ' ' << middle.position.y << std::endl;
    std::cout << end.position.x << ' ' << end.position.y << std::endl;
    
    base.front = &linearActuator;
    linearActuator.front = &middle;
    middle.front = &end;
    
    linearActuator.translate(5);
    
	std::cout << "TRANSLATION BY 5" << std::endl;
    std::cout << "MIDDLE COMPONENT Y:" << ' ' << middle.position.y << std::endl;
    std::cout << "END COMPONENT Y:" << ' ' << end.position.y << std::endl;
    
    linearActuator.translate(6);
    
	std::cout << "TRANSLATION BY 6" << std::endl;
    std::cout << "MIDDLE COMPONENT Y:" << ' ' << middle.position.y << std::endl;
    std::cout << "END COMPONENT Y:" << ' ' << end.position.y << std::endl;
    
    linearActuator.translate(-2);
    
	std::cout << "TRANSLATION BY -2" << std::endl;
    std::cout << "MIDDLE COMPONENT Y:" << ' ' << middle.position.y << std::endl;
    std::cout << "END COMPONENT Y:" << ' ' << end.position.y << std::endl;
    
    linearActuator.translate(3);
    
	std::cout << "TRANSLATION BY 3" << std::endl;
    std::cout << "MIDDLE COMPONENT Y:" << ' ' << middle.position.y << std::endl;
    std::cout << "END COMPONENT Y:" << ' ' << end.position.y << std::endl;
    
    linearActuator.translate(-50);
    
	std::cout << "TRANSLATION BY -50" << std::endl;
    std::cout << "MIDDLE COMPONENT Y:" << ' ' << middle.position.y << std::endl;
    std::cout << "END COMPONENT Y:" << ' ' << end.position.y << std::endl;
    
}
