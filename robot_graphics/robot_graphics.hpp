#pragma once

#include <SFML/Graphics.hpp>

class RobotComponent;
class Coordinate;
class Segment;
class LinearActuator;
class Motor;
class Robot;

// This function converts the Coordinates as defined in robot to positions for SFML
// This is necessary as SFML defines the coordinate system such that (0, 0) is at the top left of the screen, and -y is 
// above and +y is below, however when writing code for Coordinate, I have imagined +y to be above (0, 0)
// Therefore, this function will essentially reflect all coordinates in the x axis
sf::Vector2f coordinateToSFMLPosition(const Coordinate& coordinate);

// This function will draw a segment to the window
// If the segment is selected, then the selected argument should be passed as true, and an outline will be drawn
void drawSegment(const Segment& segment, sf::RenderWindow& window, bool selected = false);

// This function will draw a linear actuator to the window
// If the linear actuator is selected, then a blue outline will be drawn
void drawLinearActuator(const LinearActuator& linearActuator, sf::RenderWindow& window, bool selected = false);

// This function will draw a motor to the window
// If the motor is selected, then a blue outline will be drawn
void drawMotor(const Motor& motor, sf::RenderWindow& window, bool selected = false);

// This function will draw a robot to the window
// An optional selectedComponent parameter can be passed in - this will highlight one selected component
void drawRobot(const Robot& robot, sf::RenderWindow& window, const RobotComponent* selectedComponent = nullptr);

// This function will draw the end effectors of each component to the window
// This is mainly used for debugging to see if robot components are being drawn correctly
void drawEndEffectors(const Robot& robot, sf::RenderWindow& window);

// This function will display the position of the centre of mass to the window
// This is used so that the user is aware where the centre of mass of the robot is - and whether it will tip
void drawCentreOfMass(const Robot& robot, sf::RenderWindow& window);
