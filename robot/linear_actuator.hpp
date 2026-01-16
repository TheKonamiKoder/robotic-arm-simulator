#pragma once

#include <string>
#include "coordinate.hpp"
#include "robot_component.hpp"

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
			id_t id, 
			std::string name, 
			Coordinate position, 
			double angle,
			double mass, 
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
	id_t id, 
	std::string name, 
	Coordinate position, 
	double angle,
	double mass,
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
	this->mass = mass;
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