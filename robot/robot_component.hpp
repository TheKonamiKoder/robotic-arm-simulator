#pragma once

#include <string>
#include "coordinate.hpp"

using id_t = int;	// Use type alias in case the internal datatype for id changes

class RobotComponent
{
	protected:
		id_t id;
	public:
		std::string name;
		Coordinate position;
		double angle;
		double mass;
		
		// Front pointer to create a linked list
		RobotComponent* front;				
	
		RobotComponent();
		RobotComponent(id_t id, std::string name, Coordinate position, double angle, double mass);
		virtual ~RobotComponent() = default;	// This enables RTTI, which will allow me to use dynamic cast
		id_t getId();
};

RobotComponent::RobotComponent() {}

RobotComponent::RobotComponent(id_t id, std::string name, Coordinate position, double angle, double mass)
{
	this->id = id;
	this->name = name;
	this->position = position;
	this->angle = angle;
	this->mass = mass;

	// Initialised to null pointers as they don't point to anything at the start
	this->front = nullptr;
}

id_t RobotComponent::getId()
{
	return this->id;
}