#define _USE_MATH_DEFINES

#include <string>
#include <cmath>

#include "coordinate.hpp"

#pragma once

class RobotComponent
{
	protected:
		long int id;
	public:
		std::string name;
		Coordinate position;
		double angle;
		
		// Front pointer to create a linked list
		RobotComponent* front;				
	
		RobotComponent();
		RobotComponent(long int id, std::string name, Coordinate position, double angle);
		long int getId();
};

RobotComponent::RobotComponent() {}

RobotComponent::RobotComponent(long int id, std::string name, Coordinate position, double angle)
{
	this->id = id;
	this->name = name;
	this->position = position;
	this->angle = angle;

	// Initialised to null pointers as they don't point to anything at the start
	this->front = nullptr;
}

long int RobotComponent::getId()
{
	return this->id;
}

class Segment: public RobotComponent
{
	public:
		double height;
		double width;
	
		Segment();
		Segment(long int id, std::string name, Coordinate position, double angle, double height,double width);
};

Segment::Segment() {}

Segment::Segment(long int id, std::string name, Coordinate position, double angle, double height, double width)
{
	// Initialising base class attributes
	this->id = id;
	this->name = name;
	this->position = position;
	this->angle = angle;
	this->front = nullptr;
	
	// Initialising child class attributes
	this->height = height;
	this->width = width;
}

class LinearActuator: public RobotComponent
{
	private:
		double extension;
	public:
		double maximumExtension;
		double height;
		double width;
		
		LinearActuator();
		LinearActuator(
			long int id, 
			std::string name, 
			Coordinate position, 
			double angle, 
			double maximumExtension, 
			double height, 
			double width
		);
		void translate(double displacement);
		double getExtension();
};

LinearActuator::LinearActuator() {}

LinearActuator::LinearActuator
(
	long int id, 
	std::string name, 
	Coordinate position, 
	double angle,
	double maximumExtension, 
	double height, 
	double width
)
{
	// Initialising base class attributes
	this->id = id;
	this->name = name;
	this->position = position;
	this->angle = angle;
	this->front = nullptr;
	
	// Initialising child class attributes
	this->extension = 0;    // Setting extension to 0 at the beginning
	this->maximumExtension = maximumExtension;
	this->height = height;
	this->width = width;
}

void LinearActuator::translate(double displacement)
{	
	double pastExtension = this->extension;
	double newExtension = this->extension + displacement;

	// Ensures that the extension will not become negative
	if (newExtension < 0)
	{
		this->extension = 0;
	}
	else if (newExtension > maximumExtension)
	{
		this->extension = maximumExtension;
	}
	else
	{
		this->extension = newExtension;
	}
	
	// Changing the characteristics of the linear actuator
	this->height += this->extension - pastExtension;
	
	double dx = (this->extension - pastExtension) * cos(this->angle);
	double dy = (this->extension - pastExtension) * sin(this->angle);
	
	RobotComponent* currentComponent = this->front;
	
	while (currentComponent != nullptr)
	{
		currentComponent->position.x += dx;
		currentComponent->position.y += dy;
		
		currentComponent = currentComponent->front;
	}
}

double LinearActuator::getExtension()
{
	return this->extension;
}

class Motor: public RobotComponent
{
	public:
		double rotatedAngle;
		double maximumRotation;
		double radius;
	
		Motor();
		Motor
		(
			long int id, 
			std::string name, 
			Coordinate position, 
			double angle, 
			double maximumRotation, 
			double radius			
		);
		void rotate(double angularDisplacement);
};

Motor::Motor() {}

Motor::Motor
(
	long int id, 
	std::string name, 
	Coordinate position, 
	double angle,
	double maximumRotation, 
	double radius
)
{
	// Initialising base class attributes
	this->id = id;
	this->name = name;
	this->position = position;
	this->angle = angle;
	this->front = nullptr;
	
	// Initialising child class attributes
	this->rotatedAngle = 0;    // Setting starting rotated angle to 0 at the beginning
	this->maximumRotation = maximumRotation;
	this->radius = radius;
}

void Motor::rotate(double angularDisplacement)
{
	double pastAngle = this->rotatedAngle;
	double newAngle = this->rotatedAngle + angularDisplacement;

	// Ensure that the newAngle is between 0 and 2 pi radians
	// This is to be able to check if the motor has rotated past its maximum rotation
	// A while loop is necessary in case the magnitude of the angular displacement is greater than 2 pi
	while (!((newAngle >= 0) && (newAngle < 2 * M_PI)))
	{
		if (newAngle < 0)
		{
			newAngle += 2 * M_PI;	// This will add 2 pi to angle until it comes into range
		}
		if (newAngle >= 2 * M_PI)
		{
			newAngle -= 2 * M_PI;	// This willl remove 2 pi from angle until it comes into range
		}
	}

	// Ensures that the motor is not rotated beyond its maximum limit
	// No need to check that the angle is below 0 as we have ensured that the newAngle is between 0 and 2 pi
	if (newAngle > maximumRotation)
	{
		newAngle = maximumRotation;
	}
	this->rotatedAngle = newAngle;

	RobotComponent* currentComponent = this->front;

	while (currentComponent != nullptr)
	{
		double x_rel = currentComponent->position.x - this->position.x;
		double y_rel = currentComponent->position.y - this->position.y;

		// Apply the rotation matrix
		double dx = x_rel * cos(this->rotatedAngle - pastAngle) - y_rel * sin(this->rotatedAngle - pastAngle);
		double dy = x_rel * sin(this->rotatedAngle - pastAngle) + y_rel * cos(this->rotatedAngle - pastAngle);

		currentComponent->position.x += dx;
		currentComponent->position.y += dy;

		currentComponent = currentComponent->front;
	}
}