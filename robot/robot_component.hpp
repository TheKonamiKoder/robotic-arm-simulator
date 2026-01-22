#pragma once

#include <string>
#include "coordinate.hpp"

using id_t = int;	// Use type alias in case the internal datatype for id changes

class RobotComponent
{
	protected:
		id_t id;

		virtual bool equals(const RobotComponent& other) const;
	public:
		std::string name;
		Coordinate position;
		double angle;
		double mass;
		
		// Front pointer to create a linked list
		RobotComponent* front = nullptr;				
	
		RobotComponent() = default;
		RobotComponent(id_t id, std::string name, Coordinate position, double angle, double mass);
		id_t getId() const;
		friend bool operator==(const RobotComponent& lhs, const RobotComponent& rhs);
};

bool RobotComponent::equals(const RobotComponent& other) const
{
	return this->id == other.id 			&&
		   this->name == other.name 		&&
		   this->position == other.position &&
		   this->angle == other.angle 		&&
		   this->mass == other.mass;
}

RobotComponent::RobotComponent(id_t id, std::string name, Coordinate position, double angle, double mass)
	: id(id), name(name), position(position), angle(angle), mass(mass) {}

id_t RobotComponent::getId() const
{
	return this->id;
}

bool operator==(const RobotComponent& lhs, const RobotComponent& rhs)
{
	return lhs.equals(rhs);
}