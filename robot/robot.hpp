#pragma once

#define _USE_MATH_DEFINES

#include <string>
#include <unordered_map>
#include <random>
#include <iostream>
#include <fstream>
#include <array>
#include "coordinate.hpp"
#include "robot_component.hpp"
#include "segment.hpp"
#include "linear_actuator.hpp"
#include "motor.hpp"

// Defining following macros to be used when dealing files (reading and writing to file to store the robot)
// Using pipes as the delimiter as it is unlikely that the name of the robot will require a pipe
#define DELIMITER '|'	
#define SEGMENT_IDENTIFIER "SEGMENT"
#define LINEAR_ACTUATOR_IDENTIFIER "LINEAR ACTUATOR"
#define MOTOR_IDENTIFIER "MOTOR"

// The following macros are used to define the positions of each of the parameters in the parameter arrays
// By using macros, this means that even if the positions change, I won't have to change the individual numbers
#define ID 0
#define NAME 1
#define POSITION_X 2
#define POSITION_Y 3
#define ANGLE 4
#define MASS 5

#define SEGMENT_HEIGHT 0
#define SEGMENT_WIDTH 1

#define LINEAR_ACTUATOR_EXTENSION 0
#define LINEAR_ACTUATOR_MAXIMUM_EXTENSION 1
#define LINEAR_ACTUATOR_HEIGHT 2
#define LINEAR_ACTUATOR_WIDTH 3

#define MOTOR_ROTATED_ANGLE 0
#define MOTOR_MAXIMUM_ROTATION 1
#define MOTOR_RADIUS 2

class Robot
{
private:
	id_t id;
	double mass;
	std::unordered_map<id_t, RobotComponent*> robotComponentsMap;
	RobotComponent* base;
	RobotComponent* end;
public:
	std::string name;

	Robot();
	Robot(id_t id, std::string name);
	~Robot();
	id_t generateId();
	void attachToEnd(RobotComponent* robotComponent);
	void attachTo(RobotComponent* to, RobotComponent* next);
	void attachBaseTo(RobotComponent* robotComponent);
	void remove(RobotComponent* robotComponent);
	Coordinate calculateCentreOfMass();
	void saveToFile(std::string filename);
	static Robot loadFromFile(id_t id, std::string filename);
	id_t getId();
	double getMass();
	RobotComponent* getComponentFromId(id_t id);
	RobotComponent* getBase();
	RobotComponent* getEnd();
};

Robot::Robot() {}

Robot::Robot(id_t id, std::string name)
{
	this->id = id;
	this->name = name;

	this->base = nullptr;
	this->end = nullptr;
}

// Since pointers have been used, the destructor needs to be manually defined and all RobotComponents need to be
// removed from the heap manually
// If this is not done, this can cause memory leak issues
Robot::~Robot() 
{
	// Following code loops through the map and deletes all of the robot components
	for (auto pair : this->robotComponentsMap)
	{
		delete pair.second;
	}
}

// This method will be used to generate an id for the robot components
// It will generate a unique id that is not already part of the robot
// This is important as each component will need to have its own id and this id needs to be unique for each component
// in the robot
id_t Robot::generateId()
{
	static std::random_device random_device;	// Static variables used so that it does not need to be re-declared
	static std::mt19937 generator(random_device());
	static std::uniform_int_distribution<id_t> distribution;

	return distribution(generator);
}

void Robot::attachToEnd(RobotComponent* robotComponent)
{
	this->robotComponentsMap[robotComponent->getId()] = robotComponent;
	this->mass += robotComponent->mass;		// Increase mass of robot by mass of robot component
	
	// Check if the robot has a base
	if (this->base == nullptr)
	{
		// If the robot doesn't have a base, then the component is the first component and as such will be the base
		this->base = robotComponent;
	} 
	else
	{
		// If the robot doesn't have a base, then the component is not the first component, so attach it to the end
		this->end->front = robotComponent;
	}

	// Regardless of what happens, the end component will be the new robot component
	this->end = robotComponent;
}

// Attaches the second component to the first component
void Robot::attachTo(RobotComponent* a, RobotComponent* b)
{
	// Check if a is end as slightly different logic will need to be implemented
	if (a == this->end)
	{
		this->attachToEnd(b);
	}
	else
	{
		this->robotComponentsMap[b->getId()] = b;
		this->mass += b->mass;

		// Inserts b in front of a
		b->front = a->front;	// The component in front of a is now in front of b
		a->front = b;			// The component in front of a is now b
	}
}

void Robot::attachBaseTo(RobotComponent* robotComponent)
{
	this->robotComponentsMap[robotComponent->getId()] = robotComponent;
	this->mass += robotComponent->mass;

	robotComponent->front = this->base;
	this->base = robotComponent;
}

// Removes the robot component from the robot
void Robot::remove(RobotComponent* robotComponent)
{
	// First we need to check that the robot component is actually in the list
	if (this->robotComponentsMap.find(robotComponent->getId()) != this->robotComponentsMap.end())
	{
		// Check if it is the first component
		if (this->base == robotComponent)
		{
			this->base = robotComponent->front;		// The new base is now the component in front of the old base
		}
		else
		{
			// If it isn't the base, we have to find which component points to it
			RobotComponent* currentComponent = this->base;

			while (currentComponent->front != robotComponent)
			{
				currentComponent = currentComponent->front;
			}

			// Check if component removed is at end to update the end pointer so it points to the new end of the robot
			if (this->end == robotComponent)
			{
				this->end = currentComponent;
			}
			
			currentComponent->front = robotComponent->front;
		}

		this->mass -= robotComponent->mass;

		this->robotComponentsMap.erase(robotComponent->getId());	// Remove the robot component from the map
		delete robotComponent;										// Delete the robot component from the heap
	}
}

// This method returns the position of the centre of mass of the robot
// If the robot is "empty", then the function will return (0, 0)
Coordinate Robot::calculateCentreOfMass()
{
	Coordinate centreOfMass(0, 0);

	RobotComponent* currentComponent = this->base;

	while (currentComponent != nullptr)
	{
		centreOfMass.x += currentComponent->position.x * currentComponent->mass;
		centreOfMass.y += currentComponent->position.y * currentComponent->mass;
	}
	
	if (this->mass != 0)
	{
		centreOfMass.x /= this->mass;
		centreOfMass.y /= this->mass;
	}

	return centreOfMass;
}

// This method will store the data in the file
//
// The format of the file is like so:
// First line: Robot Name (the id is not stored as the id for the robot is only important when there are multiple robots
// which will happen in Worlds)
// Each of the next lines will store the components of the robot in order of how they are connected to each other
// Format of each line differs depending on the component
// For a Segment, the line format is: ID | Name | Position x | Position y | Angle | Mass | SEGMENT | Height | Width
// For a LinearActuator, hte line format is: ID | Name | Position x | Position y | Angle | Mass | LINEAR ACTUATOR |
// Extension | Maximum Extension | Height | Width
// For a Motor, the line format is: ID | Name | Position x | Position y | Angle | Mass | MOTOR | Rotated Angle | Maximum
// Rotation | Radius
void Robot::saveToFile(std::string filename)
{
	std::ofstream file(filename);

	file << this->name << '\n';

	RobotComponent* currentComponent = this->base;

	while (currentComponent != nullptr)
	{
		// These attributes are common for all components, so this will run first no matter what
		file << currentComponent->getId()				<< DELIMITER
			 << currentComponent->name					<< DELIMITER
			 << currentComponent->position.x			<< DELIMITER
			 << currentComponent->position.y			<< DELIMITER
			 << currentComponent->angle					<< DELIMITER
			 << currentComponent->mass					<< DELIMITER;

		// Need to check what the type of components is for the next part, since they have different attributes
		// I have decided to use dynamic_cast instead of polymorphism for this because I did not want the various
		// components to be responsible for storing themselves in the file, and this behaviour makes more sense within
		// the Robot class as the Robot class is responsible for saving and loading the components into/out of files

		if (auto segment = dynamic_cast<Segment*>(currentComponent))
		{
			file << SEGMENT_IDENTIFIER 					<< DELIMITER
				 << segment->height						<< DELIMITER
				 << segment->width						<< DELIMITER;
		}
		else if (auto linear_actuator = dynamic_cast<LinearActuator*>(currentComponent))
		{
			file << LINEAR_ACTUATOR_IDENTIFIER			<< DELIMITER
				 << linear_actuator->getExtension()		<< DELIMITER
				 << linear_actuator->maximumExtension	<< DELIMITER
				 << linear_actuator->height				<< DELIMITER
				 << linear_actuator->width				<< DELIMITER;
		}
		else if (auto motor = dynamic_cast<Motor*>(currentComponent))
		{
			file << MOTOR_IDENTIFIER					<< DELIMITER
				 << motor->getRotatedAngle()			<< DELIMITER
				 << motor->maximumRotation				<< DELIMITER
				 << motor->radius						<< DELIMITER;
		}
		
		file << '\n';	// Add a new line at the end of each component

		currentComponent = currentComponent->front;
	}

	file.close();
}

// This static method will return a robot after reading it from a file
// It will iterate through all the robot components in the file (line by line) and then attach these robot components
// to the robot
Robot Robot::loadFromFile(id_t id, std::string filename)
{
	std::ifstream file(filename);
	
	std::string robot_name;
	std::getline(file, robot_name);
	robot_name.erase(robot_name.find_last_not_of(" \t\n\r\f\v"));	// Trim the end of the name 
	Robot robot(id, robot_name);									// Create robot class

	std::string line;
	while (std::getline(file, line))
	{
		std::string data = "";

		// The parameter counter stores the current parameter as they get appended to the list
		int parameter_counter = 0;
		std::string component_type = "";
		std::array<std::string, 7> common_parameters;
		std::array<std::string, 2> segment_parameters;
		std::array<std::string, 4> linear_actuator_parameters;
		std::array<std::string, 3> motor_parameters;

		char c;
		for (int i = 0; i < line.length(); i++)
		{
			c = line[i];

			if (c != DELIMITER)
			{
				data += c;
			}
			else
			{
				// Trim string
				data.erase(data.find_last_not_of(" ") + 1);
				data.erase(0, data.find_first_not_of(" "));

				// Check if the data is the segment type, in which case the next parameters will not be common
				if ((data == SEGMENT_IDENTIFIER) || (data == LINEAR_ACTUATOR_IDENTIFIER) || (data == MOTOR_IDENTIFIER))
				{
					component_type = data;
					parameter_counter = 0;
				}
				else
				{
					// Add the parameter to the correct parameter list

					if (component_type == SEGMENT_IDENTIFIER)
					{
						segment_parameters[parameter_counter] = data;
					}
					else if (component_type == LINEAR_ACTUATOR_IDENTIFIER)
					{
						linear_actuator_parameters[parameter_counter] = data;
					}
					else if (component_type == MOTOR_IDENTIFIER)
					{
						motor_parameters[parameter_counter] = data;
					}
					else
					{
						common_parameters[parameter_counter] = data;
					}
					
					parameter_counter += 1;
				}
				
				data = "";
			}
		}

		if (component_type == SEGMENT_IDENTIFIER)
		{
			robot.attachToEnd
			(
				new Segment
				(
					std::stoi(common_parameters[ID]),
					common_parameters[NAME],
					Coordinate(std::stod(common_parameters[POSITION_X]), std::stod(common_parameters[POSITION_Y])),
					std::stod(common_parameters[ANGLE]),
					std::stod(common_parameters[MASS]),
					std::stod(segment_parameters[SEGMENT_HEIGHT]),
					std::stod(segment_parameters[SEGMENT_WIDTH])
				)
			);
		}
		else if (component_type == LINEAR_ACTUATOR_IDENTIFIER)
		{
			robot.attachToEnd
			(
				new LinearActuator
				(
					std::stoi(common_parameters[ID]),
					common_parameters[NAME],
					Coordinate(std::stod(common_parameters[POSITION_X]), std::stod(common_parameters[POSITION_Y])),
					std::stod(common_parameters[ANGLE]),
					std::stod(common_parameters[MASS]),
					std::stod(linear_actuator_parameters[LINEAR_ACTUATOR_MAXIMUM_EXTENSION]),
					std::stod(linear_actuator_parameters[LINEAR_ACTUATOR_HEIGHT]),
					std::stod(linear_actuator_parameters[LINEAR_ACTUATOR_WIDTH])
				)
			);

			// Use the translate method to extend the linear actuator. This is necessary because the extension can
			// only be modified by the public method translate since it is a private attribute and can't be modified
			// directly
			dynamic_cast<LinearActuator*>(robot.getEnd())->translate
			(
				std::stod(linear_actuator_parameters[LINEAR_ACTUATOR_EXTENSION])
			);
		}
		else if (component_type == MOTOR_IDENTIFIER)
		{
			robot.attachToEnd
			(
				new Motor
				(
					std::stoi(common_parameters[ID]),
					common_parameters[NAME],
					Coordinate(std::stod(common_parameters[POSITION_X]), std::stod(common_parameters[POSITION_Y])),
					std::stod(common_parameters[ANGLE]),
					std::stod(common_parameters[MASS]),
					std::stod(motor_parameters[MOTOR_MAXIMUM_ROTATION]),
					std::stod(motor_parameters[MOTOR_RADIUS])
				)
			);

			// Use the rotate method in order to chang ethe rotated angle private attribute by the motor
			dynamic_cast<Motor*>(robot.getEnd())->rotate(std::stod(motor_parameters[MOTOR_ROTATED_ANGLE]));
		}
	}
	
	file.close();

	return robot;
}

id_t Robot::getId()
{
	return this->id;
}

double Robot::getMass()
{
	return this->mass;
}

RobotComponent* Robot::getComponentFromId(id_t id)
{
	if (this->robotComponentsMap.find(id) != this->robotComponentsMap.end())
	{
		return this->robotComponentsMap[id];
	}
	else
	{
		return nullptr;
	}	
}

RobotComponent* Robot::getBase()
{
	return this->base;
}

RobotComponent* Robot::getEnd()
{
	return this->end;
}