#define _USE_MATH_DEFINES

#include "robot_component.hpp"

#include <cmath>
#include <iostream>

double RobotComponent::normalise(double angle) const
{
	while (!((angle >= 0) && (angle < 2 * M_PI)))
	{
		if (angle < 0)
		{
			angle += 2 * M_PI;	// This will add 2 pi to angle until it comes into range
		}
		if (angle >= 2 * M_PI)
		{
			angle -= 2 * M_PI;	// This willl remove 2 pi from angle until it comes into range
		}
	}

	return angle;
}

bool RobotComponent::equals(const RobotComponent& other) const
{
	return this->id == other.id 			&&
		   this->name == other.name 		&&
		   this->position == other.position &&
		   this->angle == other.angle 		&&
		   this->mass == other.mass;
}

RobotComponent::RobotComponent(id_t id, std::string name, Coordinate position, double angle, double mass)
	: id(id), name(name), position(position), mass(mass) 
{
	this->angle = this->normalise(angle);
}

void RobotComponent::setAngle(double angle)
{
	this->angle = this->normalise(angle);
}

void RobotComponent::rotateAngle(double angularDisplacement)
{
	this->angle = this->normalise(this->angle + angularDisplacement);
}

Coordinate RobotComponent::getCentreOfMass() const
{
	return this->position;
}

id_t RobotComponent::getId() const
{
	return this->id;
}

double RobotComponent::getAngle() const
{
	return this->angle;
}

bool operator==(const RobotComponent& lhs, const RobotComponent& rhs)
{
	return lhs.equals(rhs);
}