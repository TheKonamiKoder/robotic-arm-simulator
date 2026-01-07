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
		RobotComponent(
			long int id, std::string name, Coordinate position, double angle);
		long int getId();
};

RobotComponent::RobotComponent() {}

RobotComponent::RobotComponent(
	long int id, std::string name, Coordinate position, double angle)
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
		Segment
		(	
			long int id, 
			std::string name, 
			Coordinate position, 
			double angle,
			double height,
			double width
		);
};

Segment::Segment() {}

Segment::Segment
(	
	long int id, 
	std::string name, 
	Coordinate position, 
	double angle,
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
		LinearActuator
		(
			long int id, 
			std::string name, 
			Coordinate position, 
			double angle, 
			double maximumExtension, 
			double height, 
			double width
		);
		void translate(double displacement);
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
	
	this->position.x += dx / 2;
	this->position.y += dy / 2;
	
	RobotComponent* currentComponent = this->front;
	
	while (currentComponent != nullptr)
	{
		currentComponent->position.x += dx;
		currentComponent->position.y += dy;
		
		currentComponent = currentComponent->front;
	}
}