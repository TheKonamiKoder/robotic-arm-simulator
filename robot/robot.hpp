#pragma once

#define _USE_MATH_DEFINES

#include <string>
#include <unordered_map>

#include "coordinate.hpp"
#include "robot_component.hpp"

class Robot
{
private:
	id_t id;
	std::unordered_map<id_t, RobotComponent*> robotComponentsMap;	// Stores internal data structures
	RobotComponent* base = nullptr;
	RobotComponent* end = nullptr;
public:
	std::string name = "";

	Robot() = default;
	Robot(id_t id);
	Robot(id_t id, std::string name);
	~Robot();

	// Generates a unique id for new robot components
	id_t generateId() const;

	// Attaches a robot component to the end of the robot
	void attachToEnd(RobotComponent* robotComponent);

	// Attaches component b to a in the robot
	void attachTo(RobotComponent* a, RobotComponent* b);

	// Attaches the base to robotComponent
	void attachBaseTo(RobotComponent* robotComponent);

	// Removes robotComponent from the robot - and deletes it
	void remove(RobotComponent* robotComponent);

	// Uses CCD to move the end effector of the robot to the target position
	void moveTo(const Coordinate& target, int maximum_cylces = 10);

	// This method will store the data in the file
	//
	// The format of the file is like so:
	// First line: Robot Name (the id is not stored as the id for the robot is only important when there are multiple
	// robots which will happen in Worlds)
	// Each of the next lines will store the components of the robot in order of how they are connected to each other
	// Format of each line differs depending on the component
	// For a Segment, the line format is: ID | Name | Position x | Position y | Angle | Mass | SEGMENT | Height | Width
	// For a LinearActuator, hte line format is: ID | Name | Position x | Position y | Angle | Mass | LINEAR ACTUATOR |
	// Extension | Maximum Extension | Height | Width
	// For a Motor, the line format is: ID | Name | Position x | Position y | Angle | Mass | MOTOR | Rotated Angle |
	// Maximum Rotation | Radius
	void saveToFile(const std::string& filename) const;

	// It will iterate through all the robot components in the file (line by line) and then attach these robot
	// components to the robot
	void loadFromFile(const std::string& filename);

	// Will reset the robot - including deleting all the robot components from the robot, clearing the mass and the name
	void reset();

	// This method returns the position of the centre of mass of the robot
	// If the robot is "empty", then the function will return (0, 0)
	Coordinate getCentreOfMass() const;

	// Calculates the end effector position
	// By default, the end effector is the end of the robot, but a specific end effector can be passed in by user
	Coordinate getEndEffectorPosition(RobotComponent* endEffector = nullptr) const;
	id_t getId() const;
	double getMass() const;
	RobotComponent* getComponentFromId(id_t id);
	RobotComponent* getBase() const;
	RobotComponent* getEnd() const;
};