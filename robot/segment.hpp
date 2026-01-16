#pragma once

#include <string>
#include "coordinate.hpp"
#include "robot_component.hpp"

class Segment: public RobotComponent
{
	public:
		double height;
		double width;
	
		Segment();
		Segment(id_t id, std::string name, Coordinate position, double angle, double mass, double height, double width);
};

Segment::Segment() {}

Segment::Segment(id_t id, std::string name, Coordinate position, double angle, double mass, double height, double width)
{
	// Initialising base class attributes
	this->id = id;
	this->name = name;
	this->position = position;
	this->angle = angle;
	this->mass = mass;
	this->front = nullptr;
	
	// Initialising child class attributes
	this->height = height;
	this->width = width;
}